#include "catch.hpp"

#include <string>
#include <vector>

#include "lexers/lexers.h"

using namespace std;

TEST_CASE("Test lexer expressions"){
    SECTION("Simple test"){
        string q = "1 + 1";
        
        auto ans = lex_tokens(q.c_str());

        REQUIRE(ans[0].token_type == Token_type::NUMBER_LITERAL);
        REQUIRE(ans[1].token_type == Token_type::BI_OPERATOR);
        REQUIRE(ans[2].token_type == Token_type::NUMBER_LITERAL);

        REQUIRE(ans[0].from_string(q)=="1");
        REQUIRE(ans[1].from_string(q)=="+");
        REQUIRE(ans[2].from_string(q)=="1");
    }
    SECTION("Simple test"){
        string q = "1 - 1";
        
        auto ans = lex_tokens(q.c_str());

        REQUIRE(ans[0].token_type == Token_type::NUMBER_LITERAL);
        REQUIRE(ans[1].token_type == Token_type::BI_OPERATOR);
        REQUIRE(ans[2].token_type == Token_type::NUMBER_LITERAL);

        REQUIRE(ans[0].from_string(q)=="1");
        REQUIRE(ans[1].from_string(q)=="-");
        REQUIRE(ans[2].from_string(q)=="1");
    }
    SECTION("Simple dual char operator"){
        string q = "1 == 2";
        
        auto ans = lex_tokens(q.c_str());

        REQUIRE(ans[0].token_type == Token_type::NUMBER_LITERAL);
        REQUIRE(ans[1].token_type == Token_type::BI_OPERATOR);
        REQUIRE(ans[2].token_type == Token_type::NUMBER_LITERAL);

        REQUIRE(ans[0].from_string(q)=="1");
        REQUIRE(ans[1].from_string(q)=="==");
        REQUIRE(ans[2].from_string(q)=="2");
    }
    SECTION("Simple list"){
        string q = "a = (1, 2, 3)";
        
        auto ans = lex_tokens(q.c_str());

        REQUIRE(ans[0].token_type == Token_type::SYMBOL);
        REQUIRE(ans[1].token_type == Token_type::BI_OPERATOR);
        REQUIRE(ans[2].token_type == Token_type::LIST_LITERAL);

        REQUIRE(ans[0].from_string(q)=="a");
        REQUIRE(ans[1].from_string(q)=="=");
        REQUIRE(ans[2].from_string(q)=="(1, 2, 3)");
    }
    SECTION("Simple assignement"){
        string q = "a = 2 - 3 + f";
        
        auto ans = lex_tokens(q.c_str());

        REQUIRE(ans[0].token_type == Token_type::SYMBOL);
        REQUIRE(ans[1].token_type == Token_type::BI_OPERATOR);
        REQUIRE(ans[2].token_type == Token_type::NUMBER_LITERAL);
        REQUIRE(ans[3].token_type == Token_type::BI_OPERATOR);
        REQUIRE(ans[4].token_type == Token_type::NUMBER_LITERAL);
        REQUIRE(ans[5].token_type == Token_type::BI_OPERATOR);
        REQUIRE(ans[6].token_type == Token_type::SYMBOL);

        REQUIRE(ans[0].from_string(q)=="a");
        REQUIRE(ans[1].from_string(q)=="=");
        REQUIRE(ans[2].from_string(q)=="2");
    }

}