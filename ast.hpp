#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

using SymbolTable = std::map<std::string, double>;

// classe base
struct ASTNode {
  virtual ~ASTNode() = default;
  virtual void print(int indent = 0) const = 0;
  virtual double eval(SymbolTable& symbols) const = 0;
  virtual std::string codegen() const = 0;
};

// ponteiro para nós da AST.
using ASTNodePtr = std::shared_ptr<ASTNode>;

struct NumberNode : ASTNode {
  std::string value;
  NumberNode(const std::string& val) : value(val) {}
  void print(int indent = 0) const override { std::cout << std::string(indent, ' ') << "Number(" << value << ")\n"; }
  double eval(SymbolTable& symbols) const override { return std::stod(value); }
  std::string codegen() const override { return value; }
};

struct VarNode : ASTNode {
  std::string name;
  VarNode(const std::string& n) : name(n) {}
  void print(int indent = 0) const override { std::cout << std::string(indent, ' ') << "Var(" << name << ")\n"; }
  double eval(SymbolTable& symbols) const override {
    if (symbols.find(name) == symbols.end()) {
        std::cerr << "Erro de Avaliacao: Variavel indefinida '" << name << "'\n";
        return 0;
    }
    return symbols[name];
  }
  std::string codegen() const override { return name; }
};

struct BinOpNode : ASTNode {
  std::string op;
  ASTNodePtr left, right;
  BinOpNode(const std::string& o, ASTNodePtr l, ASTNodePtr r) : op(o), left(l), right(r) {}
  void print(int indent = 0) const override {
    std::cout << std::string(indent, ' ') << "BinOp(" << op << ")\n";
    if (left) left->print(indent + 2);
    if (right) right->print(indent + 2);
  }
  double eval(SymbolTable& symbols) const override {
    if (op == "=") {
        VarNode* var = dynamic_cast<VarNode*>(left.get());
        if (!var) { std::cerr << "Erro: Lado esquerdo da atribuicao invalido.\n"; return 0; }
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
        if (rightVal == 0) { std::cerr << "Erro: Divisao por zero.\n"; return 0; }
        return leftVal / rightVal;
    }
    return 0;
  }
  std::string codegen() const override {
    return "(" + left->codegen() + " " + op + " " + right->codegen() + ")";
  }
};

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

  std::string codegen() const override {
    std::string code = "#include <iostream>\n\n";
    code += "int main() {\n";

    std::map<std::string, bool> vars;
    for (const auto& stmt : statements) {
        if (auto binOpNode = std::dynamic_pointer_cast<BinOpNode>(stmt)) {
            if (binOpNode->op == "=") {
                if (auto varNode = std::dynamic_pointer_cast<VarNode>(binOpNode->left)) {
                    vars[varNode->name] = true;
                }
            }
        }
    }

    for (auto const& [name, _] : vars) {
        code += "    double " + name + ";\n";
    }

    code += "    double __last_val = 0.0;\n\n";

    for (const auto& stmt : statements) {
        if (stmt) {
            code += "    __last_val = " + stmt->codegen() + ";\n";
        }
    }

    code += "\n    std::cout << __last_val << std::endl;\n";
    code += "    return 0;\n}\n";
    return code;
  }
};

#endif