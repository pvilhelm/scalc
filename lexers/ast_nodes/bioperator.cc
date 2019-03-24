using namespace std;

#include <cmath>

#include "bioperator.h"

int64_t pow_i64(int64_t l, int64_t r)
{
    if(r == 0) return 0;
    int64_t ans = l;
    for(int i = 1; i < r; i++)
        ans *= l;
    return ans;
}

uint64_t pow_u64(uint64_t l, uint64_t r)
{
    if(r == 0) return 0;
    uint64_t ans = l;
    for(int i = 1; i < r; i++)
        ans *= l;
    return ans;
}

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
        r.eval_value_type = Eval_value_type::NUMBER;
    } else if(child->token_type == Token_type::BI_OPERATOR){
        auto node_r = dynamic_pointer_cast<AST_bioperator_node>(child);
        r = node_r->evaluate();
    } else if(child->token_type == Token_type::SYMBOL){
        auto node_r = dynamic_pointer_cast<AST_symbol_node>(child);
        r = node_r->evaluate();
    } else
        throw runtime_error("Not implemented yet");
    if(r.number.number_type == Number::Number_type::INVALID)
        throw runtime_error("Invalid number");
    if(r.eval_value_type == Eval_value_type::INVALID)
        throw runtime_error("Invalid value type");

    if(child->next->token_type == Token_type::NUMBER_LITERAL){
        auto node_l = dynamic_pointer_cast<AST_number_node>(child->next);
        l.number = node_l->number;
        l.eval_value_type = Eval_value_type::NUMBER;
    } else if(child->next->token_type == Token_type::BI_OPERATOR){
        auto node_l = dynamic_pointer_cast<AST_bioperator_node>(child->next);
        l = node_l->evaluate();
    } else if(child->next->token_type == Token_type::SYMBOL){
        /* Todo: refactr so that assign l values are not handled like this ... */
        if(bi_operator_type == Bi_operator_type::ASSIGN){
            l.eval_value_type = Eval_value_type::SYMBOL;
        } else {
            auto node_l = dynamic_pointer_cast<AST_symbol_node>(child->next);
            l = node_l->evaluate();
        }
    }else
        throw runtime_error("Not implemented yet");
    if(l.eval_value_type == Eval_value_type::INVALID)
        throw runtime_error("Invalid value type");
    if(l.eval_value_type == Eval_value_type::NUMBER && l.number.number_type == Number::Number_type::INVALID)
        throw runtime_error("Invalid number");

    if(bi_operator_type == Bi_operator_type::PLUS){
        if(l.number.number_type == Number::Number_type::FLOAT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = r.number.f_val + l.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                double sum = r.number.i_val + l.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                double sum = r.number.u_val + l.number.f_val;
                ans.number.set_val_d64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::INT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = r.number.f_val + l.number.i_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                int64_t sum = r.number.i_val + l.number.i_val;
                ans.number.set_val_i64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = r.number.u_val + l.number.i_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::UINT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = r.number.f_val + l.number.u_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                uint64_t sum = r.number.i_val + l.number.u_val;
                ans.number.set_val_u64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = r.number.u_val + l.number.u_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        }     
        else
            throw std::runtime_error("Not implemented operation");
    } else if(bi_operator_type == Bi_operator_type::MINUS){
        if(l.number.number_type == Number::Number_type::FLOAT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.f_val - r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                double sum = l.number.f_val - r.number.i_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                double sum = l.number.f_val - r.number.u_val;
                ans.number.set_val_d64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::INT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.i_val - r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                int64_t sum = l.number.i_val - r.number.i_val;
                ans.number.set_val_i64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = l.number.i_val - r.number.u_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::UINT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.u_val - r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                uint64_t sum = l.number.u_val - r.number.i_val;
                ans.number.set_val_u64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = l.number.u_val - r.number.u_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        }     
        else
            throw std::runtime_error("Not implemented operation");
    } else if(bi_operator_type == Bi_operator_type::TIMES){
        if(l.number.number_type == Number::Number_type::FLOAT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.f_val * r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                double sum = l.number.f_val * r.number.i_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                double sum = l.number.f_val * r.number.u_val;
                ans.number.set_val_d64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::INT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.i_val * r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                int64_t sum = l.number.i_val * r.number.i_val;
                ans.number.set_val_i64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = l.number.i_val * r.number.u_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::UINT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.u_val * r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                uint64_t sum = l.number.u_val * r.number.i_val;
                ans.number.set_val_u64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = l.number.u_val * r.number.u_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        }     
        else
            throw std::runtime_error("Not implemented operation");
    } else if(bi_operator_type == Bi_operator_type::RDIVIDE){
        if(l.number.number_type == Number::Number_type::FLOAT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.f_val / r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                double sum = l.number.f_val / r.number.i_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                double sum = l.number.f_val / r.number.u_val;
                ans.number.set_val_d64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::INT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.i_val / r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                int64_t sum = l.number.i_val / r.number.i_val;
                ans.number.set_val_i64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = l.number.i_val / r.number.u_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::UINT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = l.number.u_val / r.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                uint64_t sum = l.number.u_val / r.number.i_val;
                ans.number.set_val_u64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = l.number.u_val / r.number.u_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        }     
        else
            throw std::runtime_error("Not implemented operation");
    } else if(bi_operator_type == Bi_operator_type::LDIVIDE){
        if(l.number.number_type == Number::Number_type::FLOAT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = r.number.f_val / l.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                double sum = r.number.i_val / l.number.f_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                double sum = r.number.u_val / l.number.f_val;
                ans.number.set_val_d64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::INT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = r.number.f_val / l.number.i_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                int64_t sum = r.number.i_val / l.number.i_val;
                ans.number.set_val_i64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = r.number.u_val / l.number.i_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::UINT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = r.number.f_val / l.number.u_val;
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                uint64_t sum = r.number.i_val / l.number.u_val;
                ans.number.set_val_u64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = r.number.u_val / l.number.u_val;
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        }     
        else
            throw std::runtime_error("Not implemented operation");
    } else if(bi_operator_type == Bi_operator_type::POWER_OF){
        if(l.number.number_type == Number::Number_type::FLOAT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = pow(l.number.f_val, r.number.f_val);
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                double sum = pow(l.number.f_val, (double)r.number.i_val);
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                double sum = pow(l.number.f_val, (double)r.number.u_val);
                ans.number.set_val_d64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::INT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = pow((double)l.number.i_val, r.number.f_val);
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                int64_t sum = pow_i64(l.number.i_val, r.number.i_val);
                ans.number.set_val_i64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = pow_u64((uint64_t)l.number.i_val, r.number.u_val);
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        } else if (l.number.number_type == Number::Number_type::UINT64){
            if(r.number.number_type == Number::Number_type::FLOAT64){
                double sum = pow((double)l.number.u_val, r.number.f_val);
                ans.number.set_val_d64(sum);
            } else if(r.number.number_type == Number::Number_type::INT64) {
                uint64_t sum = pow_u64(l.number.u_val, (uint64_t)r.number.i_val);
                ans.number.set_val_u64(sum);
            } else if(r.number.number_type == Number::Number_type::UINT64) {
                uint64_t sum = pow_u64(l.number.u_val, r.number.u_val);
                ans.number.set_val_u64(sum);
            } else
                throw std::runtime_error("Not implemented operation");
        }     
        else
            throw std::runtime_error("Not implemented operation");
    } else if(bi_operator_type == Bi_operator_type::ASSIGN){
        /* The left node is the node to assign to. This should really be handled differently
         * so we don't take the number of the left node in vain but w/e. */
        if(child->next->token_type != Token_type::SYMBOL)
            throw runtime_error("Trying to assign to an non-symbol");

        auto node_l = dynamic_pointer_cast<AST_symbol_node>(child->next);
        auto scope = node_l->scope;
        auto symbol = node_l->symbol_ptr;
        symbol->value = r;
        ans = r;
    } else
        throw std::runtime_error("Not implemented operation");

    ans.eval_value_type = Eval_value_type::NUMBER;
    return ans;
}