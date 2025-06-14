#include <iostream>
#include <fstream>
#include "ast.hpp"
#include "lexer.hpp"
#include "parser.tab.hpp"

int main(int argc, char **argv) {
    std::istream* input_stream = &std::cin;
    std::ifstream file;

    if (argc > 1) {
        file.open(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Nao foi possivel abrir o arquivo: " << argv[1] << std::endl;
            return 1;
        }
        input_stream = &file;
    } else {
        std::cout << "Digite seu programa (Ctrl+D para finalizar):\n";
    }

    Lexer lexer(*input_stream);
    parser::Parser parser(lexer);

    int result = parser.parse();

    
    if (result == 0 && lexer.root) {

        std::cout << "\n--- Arvore Sintatica Completa (AST) ---\n";
        lexer.root->print();
        std::cout << "--------------------------------------\n";

        std::cout << "\nAvaliando o programa...\n";
        SymbolTable symbols;
        double final_result = lexer.root->eval(symbols);

        std::cout << "\n>>> Resultado da ultima expressao: " << final_result << std::endl;
    }

    std::cout << "\nAnalise finalizada." << std::endl;

    return result;
}
