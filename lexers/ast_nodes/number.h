#pragma once

#include "../lexers.h"

class AST_number_node : public AST_node {
public:
    AST_number_node();

    Number number;
};