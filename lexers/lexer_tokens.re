#include <stdio.h>

#include "lexers/lexers.h"
using namespace std;

std::vector<Token> lex_tokens(const char *YYCURSOR)
{
	vector<Token> ans;
	const char *YYMARKER, *o1, *o2;
    const char *start = YYCURSOR;
	/*!stags:re2c format = 'const char *@@;'; */
	
loop:
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;

        line_continue = "\\\n" | "\\\r\n"; //At a "line continue" the expression continue on the next line. 
        end = "\x00" | "\n" | "\r\n";
		operator = [+\-*\\/=<>\^] | "<=" | ">=" | "==";

        float = [0-9']*("."[0-9']*);
        float_EE = [0-9']*("."?[0-9']*)[Ee][-+]?[0-9']+;
        int = [0-9']*[Ss]?;
        uint = [0-9']*[uU];
        uint_hex = '0x'[a-fA-F0-9']+[uU]?;
        int_hex = '0x'[a-fA-F0-9']+[sS];
        b_int = '0b'[10']+[sS];
        b_uint = '0b'[10']+[uU]?;

		number = float | float_EE | int | uint | uint_hex | int_hex | b_int | b_uint;
        
		ws = [ \n\t\v];
        // List literal (a, b, c) etc, are also used for function calls
        list_literal = "("[^)]*")"; 
        symbol = [a-zA-Z]+[a-zA-Z0-9_]*;

        * { 
            Token token(Token_type::INVALID, 0, 0);
            ans.push_back(token); 
            return ans; 
        }
        end { return ans; }
        line_continue {
            goto loop;
        }
        @o1 operator @o2 { 
            Token token(Token_type::BI_OPERATOR, (size_t)(o1 - start), (size_t)(o2 - start));
            ans.push_back(token); 
            goto loop; 
        }
        @o1 number @o2 { 
            Token token(Token_type::NUMBER_LITERAL, (size_t)(o1 - start), (size_t)(o2 - start));
            ans.push_back(token); 
            goto loop; 
        }
        @o1 list_literal @o2 { 
            Token token(Token_type::LIST_LITERAL, (size_t)(o1 - start), (size_t)(o2 - start));
            ans.push_back(token); 
            goto loop; 
        }
        @o1 symbol @o2 { 
            Token token(Token_type::SYMBOL, (size_t)(o1 - start), (size_t)(o2 - start));
            ans.push_back(token); 
            goto loop; 
        }
        ws {goto loop;}
    */
}