%{
#include "lexer.hpp"
#include <string>

#undef YY_DECL
#define YY_DECL int Lexer::yylex(parser::Parser::semantic_type* yylval, parser::Parser::location_type* yylloc)
%}

%option c++ noyywrap nounput noinput
%option yyclass="Lexer"

%%

%{

    yylloc->begin.column = m_column;
    m_column += yyleng;
%}

[ \t]+              { /* Ignora espaços */ }

\n+                 {
                        yylineno += yyleng;
                        m_column = 1;
                        return '\n';
                    }

[0-9]+(\.[0-9]+)?   {
                        yylval->emplace<std::string>(yytext);
                        return parser::Parser::token::NUMBER;
                    }

[a-zA-Z_][a-zA-Z0-9_]* {
                        yylval->emplace<std::string>(yytext);
                        return parser::Parser::token::ID;
                    }

.                   { return yytext[0]; }

%%


Lexer::Lexer(std::istream& in) : yyFlexLexer(&in) {}
