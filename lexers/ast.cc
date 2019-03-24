#include <stack>
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>

#include "lexers/lexers.h"

using namespace std;

std::shared_ptr<AST_node> lines_to_ASTnodes
(
    std::shared_ptr<std::string> ptr_s, 
    size_t offset, 
    size_t len, 
    std::shared_ptr<Scope> parent_scope,
    std::shared_ptr<Scope> current_scope
)
{
    /* First off we collect all separate statements. I.e. a line without escaped line break. */
    vector< pair< size_t /* offset */, size_t /* length */>> v_statements_offsets;
    size_t start = 0;
    bool is_escaped;
    for(size_t i = 0; i < ptr_s->length(); i++){
        char c = (*ptr_s)[i];
        if(c == '\\')
            is_escaped = !is_escaped; /* Toggle the escape. So that two escapes in a row is handled. */
        else if(c == '\n' && !is_escaped){
            pair<size_t, size_t> p = make_pair<>(start, (size_t)(i - start + 1));/* VS Code's stupid autocomplete parser need the cast. */
            v_statements_offsets.push_back(p); 
            start = i + 1;
        } else
            is_escaped = false;
        
        if(i == ptr_s->length() - 1){ /* Last character */
            if(is_escaped) /* Bad input if the string ends with an escape. */
                throw runtime_error("Escaped at last char");
            if(start < ptr_s->length()){ /* start will be at length if the string ends with a newline. */
                pair<size_t, size_t> p = make_pair<>(start, (size_t)(i - start + 1));/* VS Code's stupid autocomplete parser need the cast. */
                v_statements_offsets.push_back(p); 
            }
        }
    }
    if(!v_statements_offsets.size())
        throw runtime_error("No statements in string");

    /* Set the scopes */
    if(current_scope == nullptr)
        current_scope = make_shared<Scope>();
    if(parent_scope != nullptr)
        current_scope->parent_scope = parent_scope;

    auto root_statement = make_shared<AST_statement_node>();
    auto marker = root_statement;
    /* Iterate over the statements. */
    for(size_t i = 0; i < v_statements_offsets.size(); i++){
        auto p = v_statements_offsets[i];
        auto node = string_to_ASTnodes(ptr_s, p.first, p.second, parent_scope, current_scope);
        auto ast_root = sort_shunting_yard_ASTnodes(node);

        marker->scope = current_scope;
        marker->child = ast_root;
        marker->ptr_s = ptr_s;
        marker->offset = p.first;
        marker->length = p.second;

        if(i < v_statements_offsets.size() - 1){
            marker->next = make_shared<AST_statement_node>();
            marker = dynamic_pointer_cast<AST_statement_node>(marker->next);
        }
    }

    return root_statement;
}

std::shared_ptr<AST_node> string_to_ASTnodes(std::shared_ptr<std::string> ptr_s, size_t offset, size_t len)
{
    return string_to_ASTnodes(ptr_s, offset, len, nullptr, nullptr);
}

std::shared_ptr<AST_node> string_to_ASTnodes(   
                                                std::shared_ptr<std::string> ptr_s, 
                                                size_t offset, 
                                                size_t len, 
                                                std::shared_ptr<Scope> parent_scope
                                            )
{
    return string_to_ASTnodes(ptr_s, offset, len, parent_scope, nullptr);
}


/** Parse a string and return a list of nodes */
std::shared_ptr<AST_node> string_to_ASTnodes
(
    std::shared_ptr<std::string> ptr_s, 
    size_t offset, 
    size_t len, 
    std::shared_ptr<Scope> parent_scope, 
    std::shared_ptr<Scope> current_scope
)
{
    if(current_scope == nullptr)
        current_scope = make_shared<Scope>();
    if(parent_scope != nullptr)
        current_scope->parent_scope = parent_scope;
    shared_ptr<AST_node> AST_root = make_shared<AST_node>();
    shared_ptr<AST_node> marker = AST_root;
    marker->token_type = Token_type::START;
    marker->scope = current_scope;

    if(offset == ptr_s->length()){
        throw runtime_error("Invalid chain of tokens:\n" + *ptr_s);
    }
    string sub_s = ptr_s->substr(offset, len);
    auto v_tokens = lex_tokens(sub_s.c_str());

    for(auto token : v_tokens){
        if(token.token_type == Token_type::INVALID){
            throw runtime_error("Invalid token at offset:" + to_string(offset + token.begin) + " Text:" + sub_s.substr(offset));
        }

        if(token.token_type == Token_type::LIST_LITERAL){
            auto node = make_shared<AST_node>();
            node->child = string_to_ASTnodes(ptr_s, offset + token.begin + 1, token.length() - 2, nullptr, current_scope);
            node->token_type = Token_type::LIST_LITERAL;
            node->offset = token.begin + offset;
            node->length = token.length();
            node->ptr_s = ptr_s;
            marker->next = node;
            marker = node;
        } else if(token.token_type == Token_type::NUMBER_LITERAL){
            string ss = token.from_string(sub_s);
            auto node = make_shared<AST_number_node>();
            node->number = lex_number(ss.c_str());
            if(node->number.number_type == Number::Number_type::INVALID){
                throw runtime_error("Invalid number token:" + ss);
            }
            node->ptr_s = ptr_s;
            node->offset = token.begin + offset;
            node->length = token.length();
            marker->next = node;
            marker = node;
        } else if(token.token_type == Token_type::BI_OPERATOR){
            string ss = token.from_string(sub_s);
            auto node = lex_operator_token(ss.c_str());
            if(node->token_type == Token_type::INVALID){
                throw runtime_error("Invalid token:" + ss);
            }
            node->ptr_s = ptr_s;
            node->offset = token.begin + offset;
            node->length = token.length();
            marker->next = node;
            marker = node;
        } else if(token.token_type == Token_type::SYMBOL){
            string ss = token.from_string(sub_s);
            auto node = make_shared<AST_symbol_node>();

            /* Check if the symbol allready is in scope. */
            auto symbol_ptr = current_scope->get_symbol(ss);

            if(symbol_ptr == nullptr){
                /* If the symbol doesn't exist in the scope we add it to it. */
                node->symbol_ptr = make_shared<Symbol>();
                node->symbol_ptr->name = ss;
                current_scope->add_symbol(node->symbol_ptr);
            } else {
                node->symbol_ptr = symbol_ptr;
            }

            node->ptr_s = ptr_s;
            node->offset = token.begin + offset;
            node->length = token.length();
            marker->next = node;
            marker = node;
        } else {
            auto node = make_shared<AST_node>();
            node->ptr_s = ptr_s;
            node->token_type = token.token_type;
            node->offset = token.begin + offset;
            node->length = token.length();
            marker->next = node;
            marker = node;
        }
        /* Assign the top scope to all nodes for now */
        marker->scope = current_scope;
    }

    return AST_root;
}

/** Sort AST node list to a tree with Shunting yard algorithm. */
std::shared_ptr<AST_node> sort_shunting_yard_ASTnodes(std::shared_ptr<AST_node> root_node){
    stack<shared_ptr<AST_node>> operator_stack;
    stack<shared_ptr<AST_node>> operand_stack;
    stack<shared_ptr<AST_node>> output_stack;
    stack<shared_ptr<AST_node>> tree_stack;

    auto marker = root_node;
    auto old_marker = marker;
    
    while(marker){

        if(marker->token_type == Token_type::LIST_LITERAL){
            /* For paranthesis we just do this recursively. */
            auto sorted = sort_shunting_yard_ASTnodes(marker->child);
            operand_stack.push(sorted);
        }else if(marker->token_type == Token_type::NUMBER_LITERAL){
            operand_stack.push(marker);
        } else if(marker->token_type == Token_type::SYMBOL){
            operand_stack.push(marker);
        } else if(marker->token_type == Token_type::FNC){
            operator_stack.push(marker);
        } else if(marker->token_type == Token_type::BI_OPERATOR){
            auto marker_biop = dynamic_pointer_cast<AST_bioperator_node>(marker);
            shared_ptr<AST_bioperator_node> operator_top = nullptr;
            if(operator_stack.size())
                operator_top = dynamic_pointer_cast<AST_bioperator_node>(operator_stack.top());
            /* If there are higher priority operators or functions on the operator stack, they need to be
             * poped and pushed to the operand stack before we push the marker operator to the operator stack. */
            while(operator_top && (
                        
                        /*There is an function on the top of the operator stack*/
                        (operator_top->token_type == Token_type::FNC) ||
                        /* or there is an operator at the top of the operator stack with greater precedence */
                        (operator_top->get_priority() < marker_biop->get_priority()) ||
                        /* or the operator at the top of the operator stack has equal precedence and is right associative */
                        (
                            operator_top->get_priority() == marker_biop->get_priority() &&
                            operator_top->get_associativity() == Operator_associativity::RIGHT
                        )
                    )
                )
            {
                
                /* Take operator node from the operator staack and pop operands and add as children to it. */
                operator_stack.pop();
                if(!operand_stack.top())
                    throw runtime_error("Bad tree, no operand 1 to pop");
                operator_top->child = operand_stack.top();
                operand_stack.pop();
                if(!operand_stack.top())
                    throw runtime_error("Bad tree, no operand 2 to pop");
                operator_top->child->next = operand_stack.top();
                operand_stack.pop();
                /* The operator is now an "operand", i.e. it will evaluate, so push it to the operand stack. */
                operand_stack.push(operator_top);

                /* Prepare the next operator for the wile-loop or break. */
                if(operator_stack.size())
                    operator_top = dynamic_pointer_cast<AST_bioperator_node>(operator_stack.top());
                else
                    break;
            }
            operator_stack.push(marker_biop);
        }

        old_marker = marker;
        marker = old_marker->next;
        old_marker->next = nullptr;
        old_marker->child = nullptr;
    }

    while(operator_stack.size()){
        auto operator_top = operator_stack.top();
        operator_stack.pop();

        if(!operand_stack.top())
            throw runtime_error("Bad tree, no operand 1 to pop");
        operator_top->child = operand_stack.top();
        operand_stack.pop();
        if(!operand_stack.top())
            throw runtime_error("Bad tree, no operand 2 to pop");
        operator_top->child->next = operand_stack.top();
        operand_stack.pop();
        /* The operator is now an "operand", i.e. it will evaluate, so push it to the operand stack. */
        operand_stack.push(operator_top);
    }

    return operand_stack.top(); /* The top of the operand stack is the AST root. */
}