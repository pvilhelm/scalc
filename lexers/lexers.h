#include <vector>
#include <tuple>
#include <string>
#include <cinttypes>
#include <memory>
#include <map>

class Number{
public:
    enum class Number_type {
        INVALID,
        FLOAT,
        INT64,
        UINT64,
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
        number_type = Number_type::FLOAT;
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
    {Bi_operator_type::POWER_OF, Operator_associativity::RIGHT},
    {Bi_operator_type::TIMES, Operator_associativity::LEFT},
    {Bi_operator_type::RDIVIDE, Operator_associativity::RIGHT},
    {Bi_operator_type::LDIVIDE, Operator_associativity::RIGHT},
    {Bi_operator_type::EQUALS, Operator_associativity::RIGHT},
    {Bi_operator_type::ASSIGN, Operator_associativity::RIGHT},
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

class AST_node {
    public:
        std::shared_ptr<AST_node> child;
        std::shared_ptr<AST_node> next;
        Token_type token_type = Token_type::INVALID;

        virtual ~AST_node(){};

        std::shared_ptr<std::string> ptr_s;
        size_t offset;
        size_t length;

        std::string as_string(){
            std::string ans;
            if(token_type == Token_type::START)
                return "";
            if(token_type == Token_type::END)
                return "";
            
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
        void as_string_tree_helper(std::shared_ptr<std::string> s, bool print_type = false){  
            if(this->child != nullptr){
                *s += "{\n";
                this->child->as_string_tree_helper(s, print_type);
                *s += "}\n";
            } else {
                *s += as_string() + (print_type ? " " + type_as_string() : "");
                *s += "\n";
            }
            if(next == nullptr)
                return;
            else
                next->as_string_tree_helper(s, print_type);
        }
};

class AST_bioperator_node : public AST_node {
public:
    Bi_operator_type bi_operator_type = Bi_operator_type::INVALID;
    AST_bioperator_node(){
        this->token_type = Token_type::BI_OPERATOR;
    }

    virtual std::string type_as_string() override {
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
};

std::shared_ptr<AST_node> string_to_ASTnodes(std::shared_ptr<std::string> ptr_s, size_t offset, size_t len);

std::vector<Token> lex_tokens(const char *YYCURSOR);
Number lex_number(const char *YYCURSOR);

std::shared_ptr<AST_node> lex_operator_token(const char *YYCURSOR);