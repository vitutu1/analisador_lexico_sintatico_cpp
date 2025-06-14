%code requires {
    #include <string>
    #include <memory>
    #include "ast.hpp"
    class Lexer; // Declaração "forward" da classe Lexer.
}

%code {
    #include "lexer.hpp"
    // Força o parser a chamar o método yylex do objeto lexer.
    #undef yylex
    #define yylex lexer.yylex
}

/* Configurações da API C++ moderna do Bison */
%define api.parser.class {Parser}
%define api.namespace {parser}
%define api.value.type variant
%define parse.assert
%locations

%parse-param { Lexer& lexer }

%token<std::string> ID "identifier"
%token<std::string> NUMBER "number"
%token END 0 "end of file"


%type<ASTNodePtr> stmt expr


%right '='
%left '+' '-'
%left '*' '/'
%precedence '(' ')'

%%

program:
    %empty
    | program stmt
    ;

stmt:
    expr '\n'           { if ($1) lexer.root = $1; }
    | ID '=' expr '\n'  { lexer.root = std::make_shared<BinOpNode>("=", std::make_shared<VarNode>($1), $3); }
    | '\n'              { /* Linhas em branco são ignoradas */ }
    | error '\n'        { yyerrok; /* Recuperação de erro simples */ }
    ;

expr:
    NUMBER              { $$ = std::make_shared<NumberNode>($1); }
    | ID                { $$ = std::make_shared<VarNode>($1); }
    | expr '+' expr     { $$ = std::make_shared<BinOpNode>("+", $1, $3); }
    | expr '-' expr     { $$ = std::make_shared<BinOpNode>("-", $1, $3); }
    | expr '*' expr     { $$ = std::make_shared<BinOpNode>("*", $1, $3); }
    | expr '/' expr     { $$ = std::make_shared<BinOpNode>("/", $1, $3); }
    | '(' expr ')'      { $$ = $2; }
    ;

%%

void parser::Parser::error(const location& l, const std::string& m) {
    std::cerr << "Erro na linha " << l.begin.line << ": " << m << std::endl;
}
