/* Generated by re2c 1.0.1 on Sun Mar 24 11:04:34 2019 */
#line 1 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
#include <stdio.h>

#include "lexers/lexers.h"
using namespace std;

std::vector<Token> lex_tokens(const char *YYCURSOR)
{
	vector<Token> ans;
	const char *YYMARKER, *o1, *o2;
    const char *start = YYCURSOR;
	const char *yyt1;
	
loop:
    
#line 18 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
{
	char yych;
	unsigned int yyaccept = 0;
	yych = *YYCURSOR;
	switch (yych) {
	case 0x00:
	case '\n':	goto yy2;
	case '\t':
	case '\v':
	case ' ':	goto yy6;
	case '(':
		yyt1 = YYCURSOR;
		goto yy8;
	case '*':
	case '+':
	case '-':
	case '/':
	case '^':
		yyt1 = YYCURSOR;
		goto yy10;
	case '0':
		yyt1 = YYCURSOR;
		goto yy12;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		yyt1 = YYCURSOR;
		goto yy13;
	case '<':
	case '=':
	case '>':
		yyt1 = YYCURSOR;
		goto yy16;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
		yyt1 = YYCURSOR;
		goto yy17;
	case '\\':
		yyt1 = YYCURSOR;
		goto yy20;
	default:	goto yy4;
	}
yy2:
	++YYCURSOR;
#line 43 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
	{ return ans; }
#line 121 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
yy4:
	++YYCURSOR;
yy5:
#line 38 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
	{ 
            Token token(Token_type::INVALID, 0, 0);
            ans.push_back(token); 
            return ans; 
        }
#line 131 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
yy6:
	++YYCURSOR;
#line 67 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
	{goto loop;}
#line 136 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
yy8:
	yych = *++YYCURSOR;
	switch (yych) {
	case ')':	goto yy21;
	default:	goto yy8;
	}
yy10:
	++YYCURSOR;
yy11:
	o1 = yyt1;
	o2 = YYCURSOR;
#line 47 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
	{ 
            Token token(Token_type::BI_OPERATOR, (size_t)(o1 - start), (size_t)(o2 - start));
            ans.push_back(token); 
            goto loop; 
        }
#line 154 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
yy12:
	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case 'B':
	case 'b':	goto yy23;
	case 'X':
	case 'x':	goto yy25;
	default:	goto yy5;
	}
yy13:
	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '\'':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy13;
	case '.':	goto yy26;
	case 'E':
	case 'e':	goto yy28;
	case 'S':
	case 'U':
	case 's':
	case 'u':	goto yy29;
	default:	goto yy15;
	}
yy15:
	o1 = yyt1;
	o2 = YYCURSOR;
#line 52 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
	{ 
            Token token(Token_type::NUMBER_LITERAL, (size_t)(o1 - start), (size_t)(o2 - start));
            ans.push_back(token); 
            goto loop; 
        }
#line 198 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
yy16:
	yych = *++YYCURSOR;
	switch (yych) {
	case '=':	goto yy10;
	default:	goto yy11;
	}
yy17:
	yych = *++YYCURSOR;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy17;
	default:	goto yy19;
	}
yy19:
	o1 = yyt1;
	o2 = YYCURSOR;
#line 62 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
	{ 
            Token token(Token_type::SYMBOL, (size_t)(o1 - start), (size_t)(o2 - start));
            ans.push_back(token); 
            goto loop; 
        }
#line 282 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
yy20:
	yych = *++YYCURSOR;
	switch (yych) {
	case '\n':	goto yy30;
	default:	goto yy11;
	}
yy21:
	++YYCURSOR;
	o1 = yyt1;
	o2 = YYCURSOR;
#line 57 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
	{ 
            Token token(Token_type::LIST_LITERAL, (size_t)(o1 - start), (size_t)(o2 - start));
            ans.push_back(token); 
            goto loop; 
        }
#line 299 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
yy23:
	yych = *++YYCURSOR;
	switch (yych) {
	case '\'':
	case '0':
	case '1':	goto yy32;
	default:	goto yy24;
	}
yy24:
	YYCURSOR = YYMARKER;
	if (yyaccept == 0) {
		goto yy5;
	} else {
		goto yy15;
	}
yy25:
	yych = *++YYCURSOR;
	switch (yych) {
	case '\'':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy34;
	default:	goto yy24;
	}
yy26:
	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '\'':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy26;
	case 'E':
	case 'e':	goto yy28;
	default:	goto yy15;
	}
yy28:
	yych = *++YYCURSOR;
	switch (yych) {
	case '\'':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy36;
	case '+':
	case '-':	goto yy38;
	default:	goto yy24;
	}
yy29:
	++YYCURSOR;
	goto yy15;
yy30:
	++YYCURSOR;
#line 44 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"
	{
            goto loop;
        }
#line 389 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.cc"
yy32:
	yych = *++YYCURSOR;
	switch (yych) {
	case '\'':
	case '0':
	case '1':	goto yy32;
	case 'S':
	case 'U':
	case 's':
	case 'u':	goto yy29;
	default:	goto yy15;
	}
yy34:
	yych = *++YYCURSOR;
	switch (yych) {
	case '\'':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy34;
	case 'S':
	case 'U':
	case 's':
	case 'u':	goto yy29;
	default:	goto yy15;
	}
yy36:
	yych = *++YYCURSOR;
	switch (yych) {
	case '\'':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy36;
	default:	goto yy15;
	}
yy38:
	yych = *++YYCURSOR;
	switch (yych) {
	case '\'':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy36;
	default:	goto yy24;
	}
}
#line 68 "/mnt/c/repos/scalc/build/../lexers/lexer_tokens.re"

}