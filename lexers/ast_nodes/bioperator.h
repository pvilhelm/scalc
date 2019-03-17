#pragma once

#include "../lexers.h"

class AST_bioperator_node : public AST_node {
public:
    Bi_operator_type bi_operator_type = Bi_operator_type::INVALID;
    AST_bioperator_node();

    virtual std::string type_as_string() override;

    int get_priority() const;

    Operator_associativity get_associativity() const;

    Eval_value evaluate();
};