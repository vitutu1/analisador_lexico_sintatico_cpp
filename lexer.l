%{
#include "lexer.hpp"
#include "ast.hpp"
#include "parser.tab.hpp"
#include <string>
#include <iostream>
#include <cstdio> // Adicionado para printf


void debug_token(const char* token, const char* text) {
    printf("[LEXER] -> Token: %-10s | Text: \"%s\"\n", token, text);
}

Lexer::Lexer(std::istream& in) : yyFlexLexer(&in) {}

#undef YY_DECL
#define YY_DECL int Lexer::yylex(parser::Parser::semantic_type* yylval, parser::Parser::location_type* yylloc)

int yyFlexLexer::yylex() {
    std::cerr << "ERRO FATAL: yyFlexLexer::yylex() foi chamado! Abortando." << std::endl;
    exit(1);
}

%}

%option c++
%option noyywrap
%option yylineno

%%

[ \t]+          ;

\r\n|\n|\r      { debug_token("NEWLINE", "\\n"); return '\n'; }

[0-9]+(\.[0-9]+)? {
    yylval->build<std::string>(yytext);
    debug_token("NUMBER", yytext);
    return parser::Parser::token::NUMBER;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval->build<std::string>(yytext);
    debug_token("ID", yytext);
    return parser::Parser::token::ID;
}


"="             { debug_token("T_ASSIGN", yytext); return parser::Parser::token::T_ASSIGN; }
"+"             { debug_token("T_PLUS", yytext); return parser::Parser::token::T_PLUS; }
"-"             { debug_token("T_MINUS", yytext); return parser::Parser::token::T_MINUS; }
"*"             { debug_token("T_MULTIPLY", yytext); return parser::Parser::token::T_MULTIPLY; }
"/"             { debug_token("T_DIVIDE", yytext); return parser::Parser::token::T_DIVIDE; }
"("             { debug_token("T_LPAREN", yytext); return parser::Parser::token::T_LPAREN; }
")"             { debug_token("T_RPAREN", yytext); return parser::Parser::token::T_RPAREN; }

.               { printf("[LEXER] -> Caractere desconhecido: '%c' (ASCII: %d)\n", *yytext, *yytext); }

%%

int Lexer::yywrap() {
    return 1;
}
