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

## Fechar e sair com ctrl D ou func + D