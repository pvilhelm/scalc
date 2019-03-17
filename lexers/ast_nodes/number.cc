#include "number.h"

AST_number_node::AST_number_node()
{
    this->token_type = Token_type::NUMBER_LITERAL;
}