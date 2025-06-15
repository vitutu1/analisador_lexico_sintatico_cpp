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
%locations

%parse-param { Lexer& lexer }


%token<std::string> ID "identifier"
%token<std::string> NUMBER "number"
%token END 0 "end of file"


%token T_PLUS "+"
%token T_MINUS "-"
%token T_MULTIPLY "*"
%token T_DIVIDE "/"
%token T_ASSIGN "="
%token T_LPAREN "("
%token T_RPAREN ")"


%type<ASTNodePtr> stmt expr program stmt_list


%right T_ASSIGN
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE

%%


program:
    stmt_list           { $$ = $1; lexer.root = $1; }
    ;


stmt_list:
    %empty              { $$ = std::make_shared<ProgramNode>(); }
    | stmt_list stmt    {
                            auto prog_node = std::dynamic_pointer_cast<ProgramNode>($1);
                            if (prog_node && $2) {
                                prog_node->statements.push_back($2);
                            }
                            $$ = prog_node;
                        }
    ;


stmt:
    expr '\n'           { $$ = $1; }
    | ID T_ASSIGN expr '\n' { $$ = std::make_shared<BinOpNode>("=", std::make_shared<VarNode>($1), $3); }
    | '\n'              { $$ = nullptr; }
    | error '\n'        { yyerrok; $$ = nullptr; }
    ;


expr:
    NUMBER              { $$ = std::make_shared<NumberNode>($1); }
    | ID                { $$ = std::make_shared<VarNode>($1); }
    | expr T_PLUS expr      { $$ = std::make_shared<BinOpNode>("+", $1, $3); }
    | expr T_MINUS expr     { $$ = std::make_shared<BinOpNode>("-", $1, $3); }
    | expr T_MULTIPLY expr  { $$ = std::make_shared<BinOpNode>("*", $1, $3); }
    | expr T_DIVIDE expr    { $$ = std::make_shared<BinOpNode>("/", $1, $3); }
    | T_LPAREN expr T_RPAREN { $$ = $2; }
    ;

%%

void parser::Parser::error(const location& l, const std::string& m) {
    std::cerr << "Erro na linha " << l.begin.line << ": " << m << std::endl;
}
