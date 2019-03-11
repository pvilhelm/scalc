#include "catch.hpp"

#include <string>
#include <vector>
#include <iostream>

#include "lexers/lexers.h"

using namespace std;

TEST_CASE("Test ast"){
    SECTION("Simple test"){
        auto q = make_shared<string>("a = 2 - 3 + f");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto marker = root_node;
        REQUIRE(marker->token_type == Token_type::START);

        marker = marker->next;
        REQUIRE(marker->token_type == Token_type::SYMBOL);
        REQUIRE(marker->as_string() == "a");

        marker = marker->next;
        REQUIRE(marker->token_type == Token_type::BI_OPERATOR);
        REQUIRE(marker->as_string() == "=");

        marker = marker->next;
        REQUIRE(marker->token_type == Token_type::NUMBER_LITERAL);
        REQUIRE(marker->as_string() == "2");

        marker = marker->next;
        REQUIRE(marker->token_type == Token_type::BI_OPERATOR);
        REQUIRE(marker->as_string() == "-");

        marker = marker->next;
        REQUIRE(marker->token_type == Token_type::NUMBER_LITERAL);
        REQUIRE(marker->as_string() == "3");

        marker = marker->next;
        REQUIRE(marker->token_type == Token_type::BI_OPERATOR);
        REQUIRE(marker->as_string() == "+");

        marker = marker->next;
        REQUIRE(marker->token_type == Token_type::SYMBOL);
        REQUIRE(marker->as_string() == "f");

    }

    SECTION("Another simple test"){
        auto q = make_shared<string>("f = 1u + 1. / 4");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        string key =
R"rrr( START
f SYMBOL
= BI_OPERATOR::ASSIGN
1u NUMBER_LITERAL
+ BI_OPERATOR::PLUS
1. NUMBER_LITERAL
/ BI_OPERATOR::RDIVIDE
4 NUMBER_LITERAL
)rrr";
        REQUIRE(key == root_node->as_string_tree(true));
    }

    SECTION("Another simple test"){
        auto q = make_shared<string>("f = (1 + 1) * 2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        string key =
R"rrr( START
f SYMBOL
= BI_OPERATOR::ASSIGN
{
     START
    1 NUMBER_LITERAL
    + BI_OPERATOR::PLUS
    1 NUMBER_LITERAL
}
* BI_OPERATOR::TIMES
2 NUMBER_LITERAL
)rrr";
        REQUIRE(key == root_node->as_string_tree(true));
    }
    SECTION("Another simple test 2"){
        auto q = make_shared<string>("a = sin(2.3 + pi)");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        string key =
R"rrr( START
a SYMBOL
= BI_OPERATOR::ASSIGN
sin SYMBOL
{
     START
    2.3 NUMBER_LITERAL
    + BI_OPERATOR::PLUS
    pi SYMBOL
}
)rrr";
        REQUIRE(key == root_node->as_string_tree(true));
    }

    SECTION("Test AST tree sort with Shunting yard algo"){
        auto q = make_shared<string>("a = 2 + 2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        REQUIRE(ast_root->as_string() == "="); 
        REQUIRE(ast_root->as_string_tree(true) == 
R"rrr(= BI_OPERATOR::ASSIGN
{
    + BI_OPERATOR::PLUS
    {
        2 NUMBER_LITERAL
        2 NUMBER_LITERAL
    }
    a SYMBOL
}
)rrr"
        );
    }

    SECTION("Test AST tree sort with Shunting yard algo 2"){
        auto q = make_shared<string>("a = 1 / 2 + 2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        REQUIRE(ast_root->as_string() == "="); 
        REQUIRE(ast_root->as_string_tree(true) == 
R"rrr(= BI_OPERATOR::ASSIGN
{
    + BI_OPERATOR::PLUS
    {
        2 NUMBER_LITERAL
        / BI_OPERATOR::RDIVIDE
        {
            2 NUMBER_LITERAL
            1 NUMBER_LITERAL
        }
    }
    a SYMBOL
}
)rrr"
        );
    }

    SECTION("Test AST tree sort with Shunting yard algo 3"){
        auto q = make_shared<string>("a = (1 / 2 + 2) * 1");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        REQUIRE(ast_root->as_string() == "="); 
        REQUIRE(ast_root->as_string_tree(true) == 
R"rrr(= BI_OPERATOR::ASSIGN
{
    * BI_OPERATOR::TIMES
    {
        1 NUMBER_LITERAL
        + BI_OPERATOR::PLUS
        {
            2 NUMBER_LITERAL
            / BI_OPERATOR::RDIVIDE
            {
                2 NUMBER_LITERAL
                1 NUMBER_LITERAL
            }
        }
    }
    a SYMBOL
}
)rrr"
        );
    }

    SECTION("Test AST tree sort with Shunting yard algo 4"){
        auto q = make_shared<string>("a = 2^3+1");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        REQUIRE(ast_root->as_string() == "="); 
        REQUIRE(ast_root->as_string_tree(true) == 
R"rrr(= BI_OPERATOR::ASSIGN
{
    + BI_OPERATOR::PLUS
    {
        1 NUMBER_LITERAL
        ^ BI_OPERATOR::POWER_OF
        {
            3 NUMBER_LITERAL
            2 NUMBER_LITERAL
        }
    }
    a SYMBOL
}
)rrr"
        );
    }
    SECTION("Test AST tree sort with Shunting yard algo 5"){
        auto q = make_shared<string>("a = 2^4^3+1");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        REQUIRE(ast_root->as_string() == "="); 
        REQUIRE(ast_root->as_string_tree(true) == 
R"rrr(= BI_OPERATOR::ASSIGN
{
    + BI_OPERATOR::PLUS
    {
        1 NUMBER_LITERAL
        ^ BI_OPERATOR::POWER_OF
        {
            ^ BI_OPERATOR::POWER_OF
            {
                3 NUMBER_LITERAL
                4 NUMBER_LITERAL
            }
            2 NUMBER_LITERAL
        }
    }
    a SYMBOL
}
)rrr"
        );
    }
}
