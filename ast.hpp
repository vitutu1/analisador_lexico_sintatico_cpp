#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Estrutura base para todos os nós da arvore
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

// Ponteiro para gerenciar a memória dos nós
using ASTNodePtr = std::shared_ptr<ASTNode>;

// Nó para representar um número
struct NumberNode : ASTNode {
    std::string value;
    NumberNode(const std::string& val) : value(val) {}
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Number(" << value << ")\n";
    }
};

// Nó para representar uma variável
struct VarNode : ASTNode {
    std::string name;
    VarNode(const std::string& n) : name(n) {}
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Var(" << name << ")\n";
    }
};

// Nó para representar uma operação binária (ex: +, -, *, /)
struct BinOpNode : ASTNode {
    std::string op;
    ASTNodePtr left, right;
    BinOpNode(const std::string& o, ASTNodePtr l, ASTNodePtr r)
        : op(o), left(l), right(r) {}
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "BinOp(" << op << ")\n";
        if (left) {
            left->print(indent + 2);
        }
        if (right) {
            right->print(indent + 2);
        }
    }
};

#endif
