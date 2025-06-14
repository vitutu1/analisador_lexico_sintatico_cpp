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
        std::cout << "Digite expressoes (Ctrl+D para finalizar):\n";
    }

    Lexer lexer(*input_stream);
    parser::Parser parser(lexer);

    int result = parser.parse();

    if (result == 0 && lexer.root) {
        std::cout << "\n--- Arvore Sintatica (AST) ---\n";
        lexer.root->print();
        std::cout << "-----------------------------\n";
        std::cout << "Analise bem-sucedida!" << std::endl;
    } else if (result != 0) {
        std::cout << "Analise finalizada com erros." << std::endl;
    }

    return result;
}
