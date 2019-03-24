#pragma once

#include "../lexers.h"

class AST_symbol_node : public AST_node {
public:
    AST_symbol_node();

    std::shared_ptr<Symbol> symbol_ptr = nullptr;
    Eval_value evaluate();
};