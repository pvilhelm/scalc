#include <stdio.h>

#include "lexers/lexers.h"
using namespace std;

std::shared_ptr<AST_node> lex_operator_token(const char *YYCURSOR)
{
    const char *start = YYCURSOR;
    const char *YYMARKER;
	/*!stags:re2c format = 'const char *@@;'; */
	
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;

        end = "\x00";
        operator_plus = "+";
        operator_minus = "-";
        operator_times = "*";
        operator_powerof = "^";
        operator_ldivide = "\\";
        operator_rdivide = "/";
        operator_equals = "==";
        operator_assign = "=";

        * { 
            auto AST_node = make_shared<AST_bioperator_node>();
            return AST_node; 
        }

        operator_plus end{ 
            auto AST_node = make_shared<AST_bioperator_node>();
            AST_node->bi_operator_type = Bi_operator_type::PLUS;
            return AST_node; 
        }
        operator_minus end{ 
            auto AST_node = make_shared<AST_bioperator_node>();
            AST_node->bi_operator_type = Bi_operator_type::MINUS;
            return AST_node; 
        }
        operator_times end{ 
            auto AST_node = make_shared<AST_bioperator_node>();
            AST_node->bi_operator_type = Bi_operator_type::TIMES;
            return AST_node; 
        }
        operator_powerof end{ 
            auto AST_node = make_shared<AST_bioperator_node>();
            AST_node->token_type = Token_type::BI_OPERATOR;
            AST_node->bi_operator_type = Bi_operator_type::POWER_OF;
            return AST_node; 
        }
        operator_ldivide end{ 
            auto AST_node = make_shared<AST_bioperator_node>();
            AST_node->bi_operator_type = Bi_operator_type::LDIVIDE;
            return AST_node; 
        }
        operator_rdivide end{ 
            auto AST_node = make_shared<AST_bioperator_node>();
            AST_node->bi_operator_type = Bi_operator_type::RDIVIDE;
            return AST_node; 
        }
        operator_equals end{ 
            auto AST_node = make_shared<AST_bioperator_node>();
            AST_node->bi_operator_type = Bi_operator_type::EQUALS;
            return AST_node; 
        }
        operator_assign end{ 
            auto AST_node = make_shared<AST_bioperator_node>();
            AST_node->bi_operator_type = Bi_operator_type::ASSIGN;
            return AST_node; 
        }
    */
}