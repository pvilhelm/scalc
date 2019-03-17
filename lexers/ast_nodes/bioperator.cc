using namespace std;

#include "bioperator.h"

AST_bioperator_node::AST_bioperator_node()
{
    this->token_type = Token_type::BI_OPERATOR;
}

std::string AST_bioperator_node::type_as_string()  
{
    switch(bi_operator_type){
        case Bi_operator_type::ASSIGN       : 	return "BI_OPERATOR::ASSIGN";
        case Bi_operator_type::EQUALS       :	return "BI_OPERATOR::EQUALS";
        case Bi_operator_type::INVALID      :	return "BI_OPERATOR::INVALID";
        case Bi_operator_type::LDIVIDE      :	return "BI_OPERATOR::LDIVIDE";
        case Bi_operator_type::MINUS        :	return "BI_OPERATOR::MINUS";
        case Bi_operator_type::PLUS         :	return "BI_OPERATOR::PLUS";
        case Bi_operator_type::POWER_OF     :	return "BI_OPERATOR::POWER_OF";
        case Bi_operator_type::RDIVIDE      :	return "BI_OPERATOR::RDIVIDE";
        case Bi_operator_type::TIMES        :	return "BI_OPERATOR::TIMES";
        default     					    :   throw std::runtime_error("Invalid Bi_operator_type");
    }
}

int AST_bioperator_node::get_priority() const 
{
    auto ans = map_bioptype_to_priority.find(this->bi_operator_type);
    if(ans != map_bioptype_to_priority.cend())
        return ans->second;
    else
        throw std::runtime_error("Bugg. Bioperator token doesn't have operator priority");
}

Operator_associativity AST_bioperator_node::get_associativity() const 
{
    auto ans = map_bioptype_to_associativity.find(this->bi_operator_type);
    if(ans != map_bioptype_to_associativity.cend())
        return ans->second;
    else
        throw std::runtime_error("Bugg. Bioperator token doesn't have operator associativity");
}

Eval_value AST_bioperator_node::evaluate()
{
    if(child == nullptr)
            throw std::runtime_error("Bioperatornode has no children");
    if(child->next == nullptr)
        throw std::runtime_error("Bioperatornode has only one child");
    if(child->next->next != nullptr)
        throw std::runtime_error("Bioperatornode has only more than two children");
    
    Eval_value ans;
    Eval_value l;
    Eval_value r;


    if(child->token_type == Token_type::NUMBER_LITERAL){
        auto node_r = dynamic_pointer_cast<AST_number_node>(child);
        r.number = node_r->number;
    } else if(child->token_type == Token_type::BI_OPERATOR){
        auto node_r = dynamic_pointer_cast<AST_bioperator_node>(child);
        r.number = node_r->evaluate().number;
    } else
        throw runtime_error("Not implemented yet");
    if(r.number.number_type == Number::Number_type::INVALID)
        throw runtime_error("Invalid number");

    if(child->next->token_type == Token_type::NUMBER_LITERAL){
        auto node_l = dynamic_pointer_cast<AST_number_node>(child);
        l.number = node_l->number;
    } else if(child->next->token_type == Token_type::BI_OPERATOR){
        auto node_l = dynamic_pointer_cast<AST_bioperator_node>(child);
        l.number = node_l->evaluate().number;
    } else
        throw runtime_error("Not implemented yet");
    if(l.number.number_type == Number::Number_type::INVALID)
        throw runtime_error("Invalid number");

    if(bi_operator_type == Bi_operator_type::PLUS){
        if(l.number.number_type == Number::Number_type::FLOAT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = r.number.f_val + l.number.f_val;
                ans.number.set_val_d64(sum);
            }
        }
    } else
        throw std::runtime_error("Not implemented operation");

    return ans};
}