#include <vector>
#include <tuple>
#include <string>
#include <cinttypes>

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
    LIST_LITERAL
};

class Token {
public:
    Token_type token_type;
    size_t begin = 0;
    size_t end = 0;
    
    Token(){;};
    Token(Token_type tt, size_t begin, size_t end) : token_type(tt), begin(begin), end(end){;};

    std::string from_string(std::string &s){
        return s.substr(begin, end - begin);
    }

    size_t length(){
        return end - begin;
    }
};

std::vector<Token> lex_tokens(const char *YYCURSOR);
Number lex_number(const char *YYCURSOR);