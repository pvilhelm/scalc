#include "statement.h"

using namespace std;

AST_statement_node::AST_statement_node()
{
    this->token_type = Token_type::STATEMENT;
}

Eval_value AST_statement_node::evaluate()
{
    if(child == nullptr)
        throw runtime_error("No child to evaluate");
    Eval_value v;
    auto marker = child;
    while(marker){
        if(marker->token_type == Token_type::BI_OPERATOR){
            auto child_biop = dynamic_pointer_cast<AST_bioperator_node>(child);
            v = child_biop->evaluate();
        } else
            throw runtime_error("Not implemented token type to evaluate");
        marker = marker->next;
    }

    return v;
}