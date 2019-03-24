#pragma once

#include "../lexers.h"

class AST_statement_node : public AST_node {
public:
    AST_statement_node();
    Eval_value evaluate();
};