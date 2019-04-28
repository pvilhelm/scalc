#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <cinttypes>
#include <memory>
#include <map>
#include <stdexcept>
#include <tuple>
#include <string>
#include <algorithm>

class Number{
public:
    enum class Number_type {
        INVALID,
        FLOAT64,
        FLOAT32,
        INT64,
        UINT64,
        INT32,
        UINT32,
        INT16,
        UINT16,
        INT8,
        UINT8
    };

    double f_val = 0;
    int64_t i_val = 0;
    uint64_t u_val = 0;

    Number_type number_type = Number_type::INVALID;

    Number(){};
    Number(Number_type number_type) : number_type(number_type){};

    void set_val_i64(int64_t val){
        i_val = val;
        number_type = Number_type::INT64;
    };
    void set_val_d64(double val){
        f_val = val;
        number_type = Number_type::FLOAT64;
    };
    void set_val_u64(uint64_t val){
        u_val = val;
        number_type = Number_type::UINT64;
    };
};

enum class Token_type {
    INVALID,
    UNI_OPERATOR,
    BI_OPERATOR,
    TRI_OPERATOR,
    NUMBER_LITERAL,
    SYMBOL,
    FNC,
    COMMENT,
    LIST_LITERAL,
    END,
    START,
    STATEMENT,
};

enum class Bi_operator_type {
    INVALID,
    PLUS,/* + */
    MINUS,/* - */
    POWER_OF,/* ^ */
    TIMES, /* * */
    RDIVIDE, /* / */
    LDIVIDE, /* \ */
    EQUALS, /* == */
    ASSIGN, /* = */
};

const std::map<Bi_operator_type, int> map_bioptype_to_priority = {
    /* Lower number is higher priority. */
    {Bi_operator_type::PLUS, 3},
    {Bi_operator_type::MINUS, 3},
    {Bi_operator_type::POWER_OF, 1},
    {Bi_operator_type::TIMES, 2},
    {Bi_operator_type::RDIVIDE, 2},
    {Bi_operator_type::LDIVIDE, 2},
    {Bi_operator_type::EQUALS, 4},
    {Bi_operator_type::ASSIGN, 4},
};

enum class Operator_associativity {
    RIGHT,
    LEFT
};

const std::map<Bi_operator_type, Operator_associativity> map_bioptype_to_associativity = {
    {Bi_operator_type::PLUS, Operator_associativity::RIGHT},
    {Bi_operator_type::MINUS, Operator_associativity::RIGHT},
    {Bi_operator_type::POWER_OF, Operator_associativity::LEFT},
    {Bi_operator_type::TIMES, Operator_associativity::RIGHT},
    {Bi_operator_type::RDIVIDE, Operator_associativity::RIGHT},
    {Bi_operator_type::LDIVIDE, Operator_associativity::RIGHT},
    {Bi_operator_type::EQUALS, Operator_associativity::RIGHT},
    {Bi_operator_type::ASSIGN, Operator_associativity::LEFT},
};

class Token {
public:
    Token_type token_type;
    size_t begin = 0;
    size_t end = 0;
    
    Token(){;};
    Token(Token_type tt, size_t begin, size_t end) : token_type(tt), begin(begin), end(end){;};

    std::string from_string(std::string s){
        return s.substr(begin, end - begin);
    }

    size_t length(){
        return end - begin;
    }
};

enum class Eval_value_type {
    NUMBER,
    SYMBOL,
    INVALID
};

class Eval_value {
public:
    Eval_value_type eval_value_type = Eval_value_type::INVALID;
    Number number;

    std::string to_string(){
        std::string s = "";
        if(eval_value_type == Eval_value_type::NUMBER){
            if(number.number_type == Number::Number_type::FLOAT64)
                s += std::to_string(number.f_val);
            else if(number.number_type == Number::Number_type::INT64)
                s += std::to_string(number.i_val);
            else if(number.number_type == Number::Number_type::UINT64)
                s += std::to_string(number.u_val);
        }

        return s;
    }
};

class Symbol {
public:
    std::string name;
    Eval_value value;
};

class Scope {
public:
    std::shared_ptr<Scope> parent_scope = nullptr;

    std::map<std::string, std::shared_ptr<Symbol>> map_symbolname_to_symbolptr{};

    std::string to_string(){
        std::shared_ptr<std::string> s_ptr(new std::string);
        std::shared_ptr<size_t> intend_ptr = std::make_shared<size_t>(0);
        to_string_helper(intend_ptr, s_ptr);
        return *s_ptr;
    }

    void to_string_helper(std::shared_ptr<size_t> intend_ptr, std::shared_ptr<std::string> s_ptr){
        if(parent_scope != nullptr){
            *s_ptr += std::string(*intend_ptr, ' ') + "{\n";
            parent_scope->to_string_helper(intend_ptr, s_ptr);
            *intend_ptr +=4;
        }
        std::vector<std::shared_ptr<Symbol>> v_str;
        for(auto p : map_symbolname_to_symbolptr){
            v_str.push_back(p.second);
        }
        /* Sort symbols by name */
        std::sort(v_str.begin(), v_str.end(),
                    [](std::shared_ptr<Symbol> a, std::shared_ptr<Symbol> b){
                        return a->name < b->name;
                    }
                 );
        *s_ptr += std::string(*intend_ptr, ' ') + "{\n";
        for(auto sym : v_str)
            *s_ptr += std::string(*intend_ptr + 4, ' ') + sym->name + ": " + sym->value.to_string() + "\n";
        *s_ptr += std::string(*intend_ptr, ' ') + "}\n";
        if(parent_scope != nullptr)
            *s_ptr += std::string(*intend_ptr - 4, ' ') + "}\n";
    }

    void add_symbol(std::shared_ptr<Symbol> ptr_symbol){
        auto const iter = map_symbolname_to_symbolptr.find(ptr_symbol->name);
        if(iter != map_symbolname_to_symbolptr.cend())
            throw std::runtime_error("Symbol allready existing!");
        map_symbolname_to_symbolptr[ptr_symbol->name] = ptr_symbol;
    }

    std::shared_ptr<Symbol> get_symbol(std::string name){
        auto iter = map_symbolname_to_symbolptr.find(name);
        if(iter == map_symbolname_to_symbolptr.end()){
            if(parent_scope == nullptr)
                return nullptr;
            else 
                return parent_scope->get_symbol(name);
        }
        return iter->second;
    }
};

class AST_node {
    public:
        std::shared_ptr<AST_node> child = nullptr;
        std::shared_ptr<AST_node> next = nullptr;
        Token_type token_type = Token_type::INVALID;
        std::shared_ptr<Scope> scope = nullptr;

        virtual ~AST_node(){};

        std::shared_ptr<std::string> ptr_s = nullptr;
        size_t offset = 0;
        size_t length = 0;

        std::string as_string(){
            std::string ans;
            if(token_type == Token_type::START)
                return "";
            if(token_type == Token_type::END)
                return "";
            
            if(!ptr_s)
                throw std::runtime_error("String ptr null");

            ans = ptr_s->substr(offset, length);
            return ans;
        }

        std::string as_string_tree(bool print_type = false){
            std::shared_ptr<std::string> ptr_ss = std::make_shared<std::string>();
            as_string_tree_helper(ptr_ss, print_type);
            return *ptr_ss;
        }

        virtual std::string type_as_string(){
            switch(token_type){
                case Token_type::UNI_OPERATOR   : 	return "UNI_OPERATOR";
                case Token_type::BI_OPERATOR    :	return "BI_OPERATOR";
                case Token_type::TRI_OPERATOR   :	return "TRI_OPERATOR";
                case Token_type::NUMBER_LITERAL :	return "NUMBER_LITERAL";
                case Token_type::SYMBOL         :	return "SYMBOL";
                case Token_type::FNC            :	return "FNC";
                case Token_type::COMMENT        :	return "COMMENT";
                case Token_type::LIST_LITERAL   :	return "LIST_LITERAL";
                case Token_type::END            :	return "END";
                case Token_type::START          :	return "START";
                default     					:   throw std::runtime_error("Invalid Token type");
            }
        }

    private:
        void as_string_tree_helper(std::shared_ptr<std::string> s, bool print_type = false, size_t intend = 0){
            if(this->token_type == Token_type::INVALID)
                throw std::runtime_error("Improper tree");

            if(this->token_type != Token_type::LIST_LITERAL){
                    *s += std::string(intend, ' ') + as_string() + (print_type ? " " + type_as_string() : "");
                    *s += "\n";
            }

            if(this->child != nullptr){
                *s += std::string(intend, ' ') + "{\n";
                this->child->as_string_tree_helper(s, print_type, intend + 4);
                *s += std::string(intend, ' ') + "}\n";
            }

            if(next == nullptr)
                return;
            else
                next->as_string_tree_helper(s, print_type, intend);
        }
};

std::shared_ptr<AST_node> string_to_ASTnodes
(
    std::shared_ptr<std::string> ptr_s,
    size_t offset, 
    size_t len
);

std::shared_ptr<AST_node> string_to_ASTnodes
(   
    std::shared_ptr<std::string> ptr_s, 
    size_t offset, 
    size_t len, 
    std::shared_ptr<Scope> parent_scope
);

std::shared_ptr<AST_node> string_to_ASTnodes
(   
    std::shared_ptr<std::string> ptr_s, 
    size_t offset, 
    size_t len, 
    std::shared_ptr<Scope> parent_scope,
    std::shared_ptr<Scope> current_scope
);

std::shared_ptr<AST_node> lines_to_ASTnodes
(
    std::shared_ptr<std::string> ptr_s, 
    size_t offset, 
    size_t len, 
    std::shared_ptr<Scope> parent_scope,
    std::shared_ptr<Scope> current_scope
);

std::vector<Token> lex_tokens(const char *YYCURSOR);
Number lex_number(const char *YYCURSOR);

std::shared_ptr<AST_node> lex_operator_token(const char *YYCURSOR);

std::shared_ptr<AST_node> sort_shunting_yard_ASTnodes(std::shared_ptr<AST_node> root_node);

#include "lexers/ast_nodes/bioperator.h"
#include "lexers/ast_nodes/number.h"
#include "lexers/ast_nodes/symbol.h"
#include "lexers/ast_nodes/statement.h"

#include "lexers/statement_machine.h"