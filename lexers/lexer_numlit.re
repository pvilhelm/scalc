#include <stdio.h>
#include <algorithm>

#include "lexers/lexers.h"
using namespace std;

Number lex_number(const char *YYCURSOR)
{
	vector<Token> ans;
	const char *YYMARKER, *o1, *o2;
    const char *start = YYCURSOR;

	/*!stags:re2c format = 'const char *@@;'; */
	
loop:
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;

        end = "\x00";
		float = [1-9][0-9']*"."[0-9']*;
        float_EE = [1-9][0-9']*"."?[0-9']*[Ee][-+]?[0-9']+;
        int = [1-9][0-9']*[Ss]?;
        uint = [1-9][0-9']*[uU];
        uint_hex = '0x'[a-fA-F0-9']+[uU]?;
        int_hex = '0x'[a-fA-F0-9']+[sS];
        b_int = '0b'[10']+[sS];
        b_uint = '0b'[10']+[uU];

        * { 
            Number n{};
            return n; 
        }
        end { return Number{}; }
        @o1 float @o2 { 
            string s(o1, o2);
            s.erase(std::remove(s.begin(), s.end(), '\''), s.end());

            double ans = stod(s);
            Number n;
            n.set_val_d64(ans);
            return n; 
        }

        @o1 float_EE @o2 { 
            string s(o1, o2);
            s.erase(std::remove(s.begin(), s.end(), '\''), s.end());

            double ans = stod(s);
            Number n;
            n.set_val_d64(ans);
            return n; 
        }
        
        @o1 int @o2 { 
            string s(o1, o2);
            s.erase(std::remove(s.begin(), s.end(), '\''), s.end());

            auto ans = stoll(s);
            Number n;
            n.set_val_i64(ans);
            return n; 
        }

        @o1 uint @o2 { 
            string s(o1, o2);
            s.erase(std::remove(s.begin(), s.end(), '\''), s.end());

            auto ans = stoull(s);
            Number n;
            n.set_val_u64(ans);
            return n; 
        }
    */
}