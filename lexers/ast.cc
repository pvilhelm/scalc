#include <stack>
#include <string>
#include <iostream>
#include <stdexcept>

#include "lexers/lexers.h"

using namespace std;

/** Parse a string and return a list of nodes */
std::shared_ptr<AST_node> string_to_ASTnodes(std::shared_ptr<std::string> ptr_s, size_t offset, size_t len)
{
    shared_ptr<AST_node> AST_root = make_shared<AST_node>();
    shared_ptr<AST_node> marker = AST_root;
    marker->token_type = Token_type::START;

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
            node->child = string_to_ASTnodes(ptr_s, offset + token.begin + 1, token.length() - 2);
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
        }else {
            auto node = make_shared<AST_node>();
            node->ptr_s = ptr_s;
            node->token_type = token.token_type;
            node->offset = token.begin + offset;
            node->length = token.length();
            marker->next = node;
            marker = node;
        }
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
            while(  operator_top &&(
                        
                        /*There is an function on the top of the operator stack*/
                        (operator_top->token_type == Token_type::FNC) ||
                        /* or there is an operator at the top of the operator stack with greater precedence */
                        (operator_top->get_priority() < marker_biop->get_priority()) ||
                        /* or the operator at the top of the operator stack has equal precedence and is left associative */
                        (
                            operator_top->get_priority() == marker_biop->get_priority() &&
                            operator_top->get_associativity() == Operator_associativity::LEFT
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