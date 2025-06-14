#ifndef LEXER_HPP
#define LEXER_HPP

#if ! defined(yyFlexLexer)
#include <FlexLexer.h>
#endif

#include "parser.tab.hpp"
#include "ast.hpp"

class Lexer : public yyFlexLexer {
public:
    Lexer(std::istream& in);
    virtual ~Lexer() = default;

    virtual int yylex(
        parser::Parser::semantic_type* yylval, 
        parser::Parser::location_type* yylloc
    );

    SymbolTable symbols;
    ASTNodePtr root = nullptr;

private:
    int m_column = 1;
};

#endif 