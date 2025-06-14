%code requires {

#include <string>

#include <memory>

#include "ast.hpp"

class Lexer;

}



%code {

#include "lexer.hpp"

#undef yylex

#define yylex lexer.yylex

}



%define api.parser.class {Parser}

%define api.namespace {parser}

%define api.value.type variant

%define parse.assert

%locations



%parse-param { Lexer& lexer }



%token<std::string> ID "identifier"

%token<std::string> NUMBER "number"

%token END 0 "end of file"



%type<ASTNodePtr> stmt expr program



%right '='

%left '+' '-'

%left '*' '/'

%precedence '(' ')'



%%



program:

// Um programa começa com um nó de programa vazio.

%empty { $$ = std::make_shared<ProgramNode>(); }

// A cada novo statement, adicionamos ele à lista do nó do programa.

| program stmt {

auto prog_node = std::dynamic_pointer_cast<ProgramNode>($1);

if (prog_node && $2) {

prog_node->statements.push_back($2);

}

$$ = prog_node;

// A raiz do lexer sempre aponta para o nó do programa.

lexer.root = $$;

}

;



stmt:

expr '\n' { $$ = $1; }

| ID '=' expr '\n' { $$ = std::make_shared<BinOpNode>("=", std::make_shared<VarNode>($1), $3); }

| '\n' { $$ = nullptr; }

| error '\n' { yyerrok; $$ = nullptr; }

;



expr:

NUMBER { $$ = std::make_shared<NumberNode>($1); }

| ID { $$ = std::make_shared<VarNode>($1); }

| expr '+' expr { $$ = std::make_shared<BinOpNode>("+", $1, $3); }

| expr '-' expr { $$ = std::make_shared<BinOpNode>("-", $1, $3); }

| expr '*' expr { $$ = std::make_shared<BinOpNode>("*", $1, $3); }

| expr '/' expr { $$ = std::make_shared<BinOpNode>("/", $1, $3); }

| '(' expr ')' { $$ = $2; }

;



%%



void parser::Parser::error(const location& l, const std::string& m) {

std::cerr << "Erro na linha " << l.begin.line << ": " << m << std::endl;

}