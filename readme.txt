# Instalação do Flex, Bison e Make

Este projeto utiliza Flex para análise léxica, Bison para análise sintática e Make para automação da compilação.
---
## Instalação no Linux (Ubuntu / WSL)

```bash
sudo apt update
sudo apt install flex bison make build-essential

## MacOS
```homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install flex bison make
export PATH="/opt/homebrew/opt/flex/bin:/opt/homebrew/opt/bison/bin:$PATH"
source ~/.zshrc  # ou ~/.bash_profile

## Verificando install
flex --version
bison --version
make --version

## Compilação 

Make

```limpeza 
make clean


## Rodar arquivo compilado

./analisador

--------------------------------EXEMPLO DE USO--------------------------------
vit@vit:~$ cd /mnt/c/myS/analisador_lexico_sintatico_cpp
vit@vit:/mnt/c/myS/analisador_lexico_sintatico_cpp$ ./analisador
Digite seu programa (Ctrl+D para finalizar):
x = 8 <<ENTER A CADA ITERAÇÃO OK
y = 9
z = 10
fig = x*y*z
fig+0

--- Arvore Sintatica Completa (AST) ---
Program
  BinOp(=)
    Var(x)
    Number(8)
  BinOp(=)
    Var(y)
    Number(9)
  BinOp(=)
    Var(z)
    Number(10)
  BinOp(=)
    Var(fig)
    BinOp(*)
      BinOp(*)
        Var(x)
        Var(y)
      Var(z)
  BinOp(+)
    Var(fig)
    Number(0)
--------------------------------------

--- Codigo Gerado a partir da AST ---
x = 8
y = 9
z = 10
fig = ((x * y) * z)
(fig + 0)
-------------------------------------

Avaliando o programa...

>>> Resultado da ultima expressao: 720

Analise finalizada.



## Fechar e sair com ctrl D ou func + D
