#pragma once

#include "lexers.h"

class Statement_machine{
public:
    std::shared_ptr<AST_node> root_statement;
    std::shared_ptr<AST_node> marker;
    std::shared_ptr<Scope> scope;

    Statement_machine(std::shared_ptr<AST_node> root_statement);

    Eval_value step_machine();

    Eval_value run_machine();

    bool is_done();

    static Statement_machine string_to_machine(std::shared_ptr<std::string> s);

    static Statement_machine string_to_machine(std::string ss);
};