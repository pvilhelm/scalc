#include "catch.hpp"

#include <string>
#include <vector>

#include "lexers/lexers.h"

using namespace std;

TEST_CASE("Test operator parsing"){
    SECTION("Simple parses"){
        string q = "+";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::PLUS);
    }
    SECTION("Parse minus"){
        string q = "-";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::MINUS);
    }
    SECTION("Parse times"){
        string q = "*";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::TIMES);
    }
    SECTION("Parse plus"){
        string q = "+";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::PLUS);
    }
    SECTION("Parse ldivide"){
        string q = "\\";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::LDIVIDE);
    }
    SECTION("Parse rdivide"){
        string q = "/";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::RDIVIDE);
    }
    SECTION("Parse power of"){
        string q = "^";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::POWER_OF);
    }
    SECTION("Parse equals"){
        string q = "==";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::EQUALS);
    }
    SECTION("Parse assign"){
        string q = "=";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::ASSIGN);
    }
    SECTION("Parse assign"){
        string q = "a";
        auto a = dynamic_pointer_cast<AST_bioperator_node>(lex_operator_token(q.c_str()));
        REQUIRE(a->token_type == Token_type::BI_OPERATOR);
        REQUIRE(a->bi_operator_type == Bi_operator_type::INVALID);
    }
}