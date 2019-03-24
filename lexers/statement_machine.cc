#include "statement_machine.h"

using namespace std;

Statement_machine::Statement_machine(std::shared_ptr<AST_node> root_statement){
    this->root_statement = dynamic_pointer_cast<AST_statement_node>(root_statement);
    if(this->root_statement == nullptr)
        throw runtime_error("Provided pointer not a statement node");
    marker = root_statement;
    scope = root_statement->scope;
}

Eval_value Statement_machine::run_machine(){
    Eval_value v{};
    while(!is_done()){
        v = step_machine();
    }

    return v;
}

Eval_value Statement_machine::step_machine(){
    if(marker == nullptr)
        throw runtime_error("Trying to step finnished machine");
    auto marker_stmnt = dynamic_pointer_cast<AST_statement_node>(marker);
    if(marker_stmnt == nullptr)
        throw runtime_error("Marker not a statement node");

    Eval_value v = marker_stmnt->evaluate();
    marker = dynamic_pointer_cast<AST_statement_node>(marker_stmnt->next);

    return v;
}

bool Statement_machine::is_done(){
    return (marker == nullptr);
}

Statement_machine Statement_machine::string_to_machine(std::shared_ptr<std::string> s){
    auto scope = make_shared<Scope>();
    auto root_node = lines_to_ASTnodes(s, 0, s->size(), nullptr, scope);
    auto root_statement = dynamic_pointer_cast<AST_statement_node>(root_node);

    return Statement_machine(root_statement);
}

Statement_machine Statement_machine::string_to_machine(std::string ss){
    auto s_ptr = make_shared<string>(ss);
    return string_to_machine(s_ptr);
}
