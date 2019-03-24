#include "catch.hpp"

#include <string>
#include <vector>

#include "lexers/lexers.h"

using namespace std;

TEST_CASE("Test number parsing"){
    SECTION("Simple parses"){
        string q = "1234.1234";
        auto a = lex_number(q.c_str());
        REQUIRE(a.f_val == 1234.1234);
    }
    SECTION("Simple parses with '"){
        string q = "12'34.12'34";
        auto a = lex_number(q.c_str());
        REQUIRE(a.f_val == 1234.1234);
    }
    SECTION("Simple EE with '"){
        string q = "1'0E2";
        auto a = lex_number(q.c_str());
        REQUIRE(a.f_val == 10e2);
    }
    SECTION("Simple EE with neg exponent"){
        string q = "10.2E-2";
        auto a = lex_number(q.c_str());
        REQUIRE(a.f_val == 10.2e-2);
    }
    SECTION("Simple int test"){
        string q = "1001s";
        auto a = lex_number(q.c_str());
        REQUIRE(a.i_val == 1001);
        REQUIRE(a.number_type == Number::Number_type::INT64);
    }
    SECTION("Simple int test with s"){
        string q = "1001s";
        auto a = lex_number(q.c_str());
        REQUIRE(a.i_val == 1001);
        REQUIRE(a.number_type == Number::Number_type::INT64);
    }
    SECTION("Simple int test"){
        string q = "1001u";
        auto a = lex_number(q.c_str());
        REQUIRE(a.u_val == 1001);
        REQUIRE(a.number_type == Number::Number_type::UINT64);
    }
    SECTION("Simple hex test"){
        string q = "0x1234";
        auto a = lex_number(q.c_str());
        REQUIRE(a.u_val == 0x1234);
        REQUIRE(a.number_type == Number::Number_type::UINT64);
    }
    SECTION("Simple hex test"){
        string q = "0x1234s";
        auto a = lex_number(q.c_str());
        REQUIRE(a.i_val == 0x1234);
        REQUIRE(a.number_type == Number::Number_type::INT64);
    }
    SECTION("Simple invalid hex test"){
        string q = "0x123Q4s";
        auto a = lex_number(q.c_str());
        REQUIRE(a.number_type == Number::Number_type::INVALID);
    }
    SECTION("Simple hex test with '"){
        string q = "0x1234'5678";
        auto a = lex_number(q.c_str());
        REQUIRE(a.u_val == 0x12345678);
        REQUIRE(a.number_type == Number::Number_type::UINT64);
    }

    SECTION("Simple signed bin test with '"){
        string q = "0b1001'1001s";
        auto a = lex_number(q.c_str());
        REQUIRE(a.i_val == 0b10011001);
        REQUIRE(a.number_type == Number::Number_type::INT64);
    }

    SECTION("Simple unsigned bin test with '"){
        string q = "0b1001'1001";
        auto a = lex_number(q.c_str());
        REQUIRE(a.u_val == 0b10011001);
        REQUIRE(a.number_type == Number::Number_type::UINT64);
    }

    SECTION("Simple unsigned bin test with ' and u"){
        string q = "0b1001'1001u";
        auto a = lex_number(q.c_str());
        REQUIRE(a.u_val == 0b10011001);
        REQUIRE(a.number_type == Number::Number_type::UINT64);
    }

    SECTION("Simple unsigned bin test with invalid numbers"){
        string q = "0b1002'1001u";
        auto a = lex_number(q.c_str());
        REQUIRE(a.number_type == Number::Number_type::INVALID);
    }
}