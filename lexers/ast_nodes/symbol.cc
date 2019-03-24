#include "symbol.h"

using namespace std;

AST_symbol_node::AST_symbol_node()
{
    this->token_type = Token_type::SYMBOL;
}

Eval_value AST_symbol_node::evaluate()
{
    if(symbol_ptr == nullptr)
        throw runtime_error("Bugg");
    if(symbol_ptr->value.eval_value_type == Eval_value_type::INVALID)
        throw runtime_error("Evaluating symbol with invalid value");
    return symbol_ptr->value;
}