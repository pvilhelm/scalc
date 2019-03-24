#include "catch.hpp"

#include <string>
#include <vector>
#include <iostream>

#include "lexers/lexers.h"

using namespace std;

TEST_CASE("Test eval"){
    SECTION("Bioperator eval 1"){
        auto q = make_shared<string>("2. + 2.");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 4.);
    }
    SECTION("Bioperator eval 2"){
        auto q = make_shared<string>("1. + 2. + 3.");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 6.);
    }
    SECTION("Bioperator eval 3"){
        auto q = make_shared<string>("1. + 2. + 3.");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 6.);
    }
    SECTION("Bioperator eval 4"){
        auto q = make_shared<string>("1 + (2. + 3.)");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 6.);
    }
    SECTION("Bioperator eval 5"){
        auto q = make_shared<string>("1s + 2s");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.i_val == 3.);
        REQUIRE(ans.number.number_type == Number::Number_type::INT64);
    }
    SECTION("Bioperator eval 6"){
        auto q = make_shared<string>("1u + 2u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 3.);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 7"){
        auto q = make_shared<string>("1 - 2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == -1.);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 8"){
        auto q = make_shared<string>("1s - 2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == -1.);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 9"){
        auto q = make_shared<string>("1s - 2u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == -1ll);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 9"){
        auto q = make_shared<string>("1+2+3+4+5+6+7+8+9-1-2+1");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 43.);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 10"){
        auto q = make_shared<string>("0b0001 + 0b0010");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 3);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 11"){
        auto q = make_shared<string>("0b0001s - 0b0010s");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.i_val == -1);
        REQUIRE(ans.number.number_type == Number::Number_type::INT64);
    }
    SECTION("Bioperator eval 12"){
        auto q = make_shared<string>("(1 + 0b0010s) - 0xFF");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == -252);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 12"){
        auto q = make_shared<string>("1 + 0b0010s + 0xFF");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 258);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 13"){
        auto q = make_shared<string>("2*3");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 6);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 14"){
        auto q = make_shared<string>("2s*3 + 1");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 7);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 15"){
        auto q = make_shared<string>("2 * 3s + 1u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 7);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 16"){
        auto q = make_shared<string>("7u*2s");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 14);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 17"){
        auto q = make_shared<string>("3s*4u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 12);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 18"){
        auto q = make_shared<string>("3u*4u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 12);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 19"){
        auto q = make_shared<string>("3s*4s");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.i_val == 12);
        REQUIRE(ans.number.number_type == Number::Number_type::INT64);
    }
    SECTION("Bioperator eval 20"){
        auto q = make_shared<string>("3s*4s+1u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 13);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 21"){
        auto q = make_shared<string>("4/2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 2);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 22"){
        auto q = make_shared<string>("4s/2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 2);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 23"){
        auto q = make_shared<string>("4s/2u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 2);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 24"){
        auto q = make_shared<string>("4s/3");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);

        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);

        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == Approx(4./3.));
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 25"){
        auto q = make_shared<string>("4s/3*2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == Approx(4./3.*2.));
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 26"){
        auto q = make_shared<string>("1/2*3/4+5");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == Approx(1./2.*3./4.+5.));
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 27"){
        auto q = make_shared<string>("2^3");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == Approx(8));
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 28"){
        auto q = make_shared<string>("2u ^ 3s");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 8);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 29"){
        auto q = make_shared<string>("2s^ 3s");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.i_val == 8);
        REQUIRE(ans.number.number_type == Number::Number_type::INT64);
    }
    SECTION("Bioperator eval 30"){
        auto q = make_shared<string>("4^3^2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == Approx(262144));
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 31"){
        auto q = make_shared<string>("2*4^3^2+1");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == Approx(262144.*2.+1));
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 32"){
        auto q = make_shared<string>("2\\4");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == Approx(2));
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 33"){
        auto q = make_shared<string>("2s\\4u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == Approx(2));
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 34"){
        auto q = make_shared<string>("2*2s\\4u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == Approx(1));
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 34"){
        auto q = make_shared<string>("2u\\4u");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.u_val == 2);
        REQUIRE(ans.number.number_type == Number::Number_type::UINT64);
    }
    SECTION("Bioperator eval 35"){
        auto q = make_shared<string>("a = 2");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 2);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
        REQUIRE(ast_root_bio->scope->get_symbol("a")->name == "a");
        REQUIRE(ast_root_bio->scope->get_symbol("a")->value.number.f_val == 2);
        REQUIRE(ast_root_bio->scope->get_symbol("a")->value.number.number_type == Number::Number_type::FLOAT64);
    }
    SECTION("Bioperator eval 36"){
        auto q = make_shared<string>("a_number = 2 + 3");

        auto root_node = string_to_ASTnodes(q, 0, q->length());

        auto  ast_root = sort_shunting_yard_ASTnodes(root_node);
        
        auto ast_root_bio = dynamic_pointer_cast<AST_bioperator_node>(ast_root);
        auto ans = ast_root_bio->evaluate();

        REQUIRE(ans.number.f_val == 5);
        REQUIRE(ans.number.number_type == Number::Number_type::FLOAT64);
        REQUIRE(ast_root_bio->scope->get_symbol("a_number")->name == "a_number");
        REQUIRE(ast_root_bio->scope->get_symbol("a_number")->value.number.f_val == 5);
        REQUIRE(ast_root_bio->scope->get_symbol("a_number")->value.number.number_type == Number::Number_type::FLOAT64);
    }

    SECTION("Bioperator eval 36"){
        auto q = make_shared<string>("a = 2 + 3\nb = 1 + a");
        auto scope = make_shared<Scope>();
        auto root_node = lines_to_ASTnodes(q, 0, q->size(), nullptr, scope);
        auto root_statement = dynamic_pointer_cast<AST_statement_node>(root_node);

        Eval_value value0 = root_statement->evaluate();
        Eval_value value1 = dynamic_pointer_cast<AST_statement_node>(root_statement->next)->evaluate();

        REQUIRE(value1.number.f_val == 6);
        REQUIRE(value0.number.f_val == 5);
    }
    SECTION("Bioperator eval 36"){
        auto q = make_shared<string>("a = 2 * 3\nb = 2 * a\nc = b * a");
        auto scope = make_shared<Scope>();
        auto root_node = lines_to_ASTnodes(q, 0, q->size(), nullptr, scope);
        auto root_statement = dynamic_pointer_cast<AST_statement_node>(root_node);

        Eval_value value0 = root_statement->evaluate();
        Eval_value value1 = dynamic_pointer_cast<AST_statement_node>(root_statement->next)->evaluate();
        Eval_value value2 = dynamic_pointer_cast<AST_statement_node>(root_statement->next->next)->evaluate();

        REQUIRE(value0.number.f_val == 6);
        REQUIRE(value1.number.f_val == 12);
        REQUIRE(value2.number.f_val == 72);
    }
}