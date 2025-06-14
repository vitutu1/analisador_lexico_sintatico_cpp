#ifndef LEXER_HPP
#define LEXER_HPP

#if ! defined(yyFlexLexer)
#include <FlexLexer.h>
#endif

#include "parser.tab.hpp" // Gerado pelo Bison

class Lexer : public yyFlexLexer {
public:
    Lexer(std::istream& in);
    virtual ~Lexer() = default;

    // A função que o parser chama.
    virtual int yylex(
        parser::Parser::semantic_type* yylval,
        parser::Parser::location_type* yylloc
    );

    // Armazena a raiz da AST.
    ASTNodePtr root = nullptr;

private:
    // Variável para rastrear manualmente a coluna.
    int m_column = 1;
};

#endif // LEXER_HPP
