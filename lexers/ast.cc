#include <stack>
#include <string>
#include <iostream>

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
            node->token_type == Token_type::LIST_LITERAL;
            node->offset = token.begin + offset;
            node->length = token.length();
            node->ptr_s = ptr_s;
            marker->next = node;
            marker = node;
        } else if(token.token_type == Token_type::BI_OPERATOR){
            string ss = token.from_string(sub_s);
            auto node = lex_operator_token(ss.c_str());
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
    }

    return AST_root;
}

/** Sort AST node list to a tree with Shunting yard algorithm. */
std::shared_ptr<AST_node> sort_shunting_yard_ASTnodes(std::shared_ptr<AST_node> root_node){
    stack<shared_ptr<AST_node>> operator_stack;
    stack<shared_ptr<AST_node>> operand_stack;
    auto root_tree = make_shared<AST_node>();
    auto marker = root_node;

/*
final Stack<Character> operatorStack = new Stack<>();
        final Stack<ASTNode> operandStack = new Stack<>();
        final char[] chars = input.toCharArray();
        main:*/
        while(marker){
            
            if(marker->token_type == Token_type::BI_OPERATOR){
                /* The present token is a bi operator */
                auto operator_top = operator_stack.top();

            }
        
        /*
        for(char c : chars) {
            char popped;
            switch(c) {
                case ' ':
                    break;
                case '(':
                    operatorStack.push('(');
                    break;
                case ')':
                    while(!operatorStack.isEmpty()) {
                        popped = operatorStack.pop();
                        if('(' == popped) {
                            continue main;
                        } else {
                            addNode(operandStack, popped);
                        }
                    }
                    throw new IllegalStateException("Unbalanced right " +
                            "parentheses");
                default:
                    if(operators.containsKey(c)) {
                        final Operator o1 = operators.get(c);
                        Operator o2;
                        while(!operatorStack.isEmpty() && null != (o2 =
                                operators.get(operatorStack.peek()))) {
                            if((!o1.isRightAssociative() &&
                                    0 == o1.comparePrecedence(o2)) ||
                                    o1.comparePrecedence(o2) < 0) {
                                operatorStack.pop();
                                addNode(operandStack, o2.getSymbol());
                            } else {
                                break;
                            }
                        }
                        operatorStack.push(c);
                    } else {
                        operandStack.push(new ASTNode(c, null, null));
                    }
                    break;
            }
        }*/
        }
        /*
        while(!operatorStack.isEmpty()) {
            addNode(operandStack, operatorStack.pop());
        }
        return operandStack.pop();
        */
}