#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.tab.hpp"


std::string get_base_name(const std::string& path) {
    size_t last_slash = path.find_last_of("/\\");
    std::string filename = (last_slash == std::string::npos) ? path : path.substr(last_slash + 1);
    size_t last_dot = filename.find_last_of(".");
    return (last_dot == std::string::npos) ? filename : filename.substr(0, last_dot);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_codigo_fonte> [arquivo_de_saida]" << std::endl;
        return 1;
    }

    std::string source_file_path = argv[1];
    std::ifstream file(source_file_path);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de codigo fonte: " << source_file_path << std::endl;
        return 1;
    }

    std::string output_filename;
    if (argc > 2) {
        output_filename = argv[2];
    } else {
        output_filename = get_base_name(source_file_path);
    }
    std::string temp_cpp_file = output_filename + ".cpp";


    Lexer lexer(file);
    parser::Parser parser(lexer);
    int parse_result = parser.parse();

    if (parse_result != 0 || !lexer.root) {
        std::cerr << "Erro de compilacao durante a analise." << std::endl;
        return 1;
    }
    std::cout << "Analise sintatica concluida com sucesso." << std::endl;


    std::cout << "Gerando codigo intermediario C++..." << std::endl;
    std::string cpp_code = lexer.root->codegen();

    std::ofstream temp_out(temp_cpp_file);
    if (!temp_out) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo C++ temporario." << std::endl;
        return 1;
    }
    temp_out << cpp_code;
    temp_out.close();    std::cout << "Codigo C++ gerado em '" << temp_cpp_file << "'." << std::endl;


    std::cout << "Compilando o codigo C++ para gerar o executavel..." << std::endl;
    std::string command = "g++ -std=c++17 -Wall " + temp_cpp_file + " -o " + output_filename;

    int compile_result = system(command.c_str());

    if (compile_result != 0) {
        std::cerr << "Erro durante a compilacao do codigo C++. O arquivo temporario '" << temp_cpp_file << "' foi mantido para depuracao." << std::endl;
        return 1;
    }

    std::cout << "\nCompilacao bem-sucedida!" << std::endl;
    std::cout << "Executavel gerado: '" << output_filename << "'" << std::endl;

    remove(temp_cpp_file.c_str());

    return 0;
}