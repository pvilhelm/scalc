#include "catch.hpp"

#include <string>
#include <vector>
#include <iostream>

#include "lexers/lexers.h"

using namespace std;

TEST_CASE("Statement machine tests"){
    SECTION("Simple test"){
        string q = "2 + 2";
        Statement_machine sm = Statement_machine::string_to_machine(q);

        auto ans = sm.step_machine();
        REQUIRE(sm.is_done());
        REQUIRE(ans.number.f_val == 4);
    }
    SECTION("Simple test run_machine"){
        string q = "a = 2 + 2\nb = 2 * a";
        Statement_machine sm = Statement_machine::string_to_machine(q);

        auto ans = sm.run_machine();
        REQUIRE(sm.is_done());
        REQUIRE(ans.number.f_val == 8);
        REQUIRE(sm.scope->get_symbol("a")->value.number.f_val == 4);
        REQUIRE(sm.scope->get_symbol("b")->value.number.f_val == 8);
    }
    SECTION("Simple test run_machine"){
        string q = 
R"qweqwe(a = 1
b = 2
c = a + b*2
d = 2^2 + c
)qweqwe";
        Statement_machine sm = Statement_machine::string_to_machine(q);

        auto ans = sm.run_machine();
        REQUIRE(sm.is_done());
        REQUIRE(ans.number.f_val == 9);
        REQUIRE(sm.scope->to_string() == R"RRR({
    a: 1.000000
    b: 2.000000
    c: 5.000000
    d: 9.000000
}
)RRR");
    }
    SECTION("Simple test run_machine"){
        string q = 
R"qweqwe(a = 1+2*3
2+a
)qweqwe";
        Statement_machine sm = Statement_machine::string_to_machine(q);

        auto ans = sm.run_machine();
        REQUIRE(sm.is_done());
        REQUIRE(ans.number.f_val == 9);
    }
    SECTION("Simple test run_machine"){
        string q = "1     +      1\n";
        Statement_machine sm = Statement_machine::string_to_machine(q);
        q = "";
        auto ans = sm.run_machine();
        REQUIRE(sm.is_done());
        REQUIRE(ans.number.f_val == 2);
    }
}