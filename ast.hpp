#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

using SymbolTable = std::map<std::string, double>;

struct ASTNode {
  virtual ~ASTNode() = default;
  virtual void print(int indent = 0) const = 0;
  virtual double eval(SymbolTable& symbols) const = 0;
};

using ASTNodePtr = std::shared_ptr<ASTNode>;

struct ProgramNode : ASTNode {
  std::vector<ASTNodePtr> statements;
  void print(int indent = 0) const override {
    std::cout << std::string(indent, ' ') << "Program\n";
    for (const auto& stmt : statements) {
      if (stmt) stmt->print(indent + 2);
    }
  }
  double eval(SymbolTable& symbols) const override {
    double last_val = 0;
    for (const auto& stmt : statements) {
      if (stmt) last_val = stmt->eval(symbols);
    }
    return last_val; 
  }
};

struct NumberNode : ASTNode {
  std::string value;
  NumberNode(const std::string& val) : value(val) {}
  void print(int indent = 0) const override {
    std::cout << std::string(indent, ' ') << "Number(" << value << ")\n";
  }
  double eval(SymbolTable& symbols) const override {
    return std::stod(value);
  }
};

struct VarNode : ASTNode {
  std::string name;
  VarNode(const std::string& n) : name(n) {}
  void print(int indent = 0) const override {
    std::cout << std::string(indent, ' ') << "Var(" << name << ")\n";
  }
  double eval(SymbolTable& symbols) const override {
    if (symbols.find(name) == symbols.end()) {
        std::cerr << "Erro de Avaliacao: Variavel indefinida '" << name << "'\n";
        return 0;
    }
    return symbols[name];
  }
};

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
  double eval(SymbolTable& symbols) const override {
    if (op == "=") {
        VarNode* var = dynamic_cast<VarNode*>(left.get());
        if (!var) {
            std::cerr << "Erro de Avaliacao: O lado esquerdo da atribuicao deve ser uma variavel.\n";
            return 0;
        }
        double val = right->eval(symbols);
        symbols[var->name] = val;
        return val;
    }

    double leftVal = left->eval(symbols);
    double rightVal = right->eval(symbols);

    if (op == "+") return leftVal + rightVal;
    if (op == "-") return leftVal - rightVal;
    if (op == "*") return leftVal * rightVal;
    if (op == "/") {
        if (rightVal == 0) {
            std::cerr << "Erro de Avaliacao: Divisao por zero.\n";
            return 0;
        }
        return leftVal / rightVal;
    }

    std::cerr << "Erro de Avaliacao: Operador desconhecido '" << op << "'\n";
    return 0;
  }
};

#endif 
