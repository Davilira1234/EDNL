#ifndef DICIONARIO_HPP
#define DICIONARIO_HPP

#include <iostream>
#include <string>

// ========================
// Classe Palavra
// ========================
class Palavra {
public:
    std::string ingles;
    std::string portugues;
    std::string classificacao;

    Palavra(std::string i = "", std::string p = "", std::string c = "");
    static std::string toLower(std::string s);
};

// ========================
// Nó da BST
// ========================
class NoBST {
public:
    Palavra palavra;
    NoBST* esq;
    NoBST* dir;

    NoBST(const Palavra &p);
};

// ========================
// Classe BST
// ========================
class BST {
private:
    NoBST* raiz;

    NoBST* inserir(NoBST* r, const Palavra &p);
    NoBST* buscar(NoBST* r, const std::string &ingles);
    NoBST* remover(NoBST* r, const std::string &ingles);
    NoBST* minimo(NoBST* no);
    void emOrdem(NoBST* r);

public:
    BST();
    void inserir(const Palavra &p);
    void remover(const std::string &ingles);
    Palavra* buscar(const std::string &ingles);
    void listarEmOrdem();
    void listarEmLargura();
};

// ========================
// Nó da Lista Duplamente Encadeada
// ========================
class NoLista {
public:
    Palavra dado;
    NoLista* prox;
    NoLista* ant;

    NoLista(Palavra p);
};

// ========================
// Classe ListaDupla
// ========================
class ListaDupla {
private:
    NoLista* cabeca;
    NoLista* cauda;

public:
    ListaDupla();
    void adicionar(Palavra p);
    bool vazia() const;
    int tamanho() const;
    Palavra removerPorIndice(int i);
    Palavra removerAleatorio();
    void carregarDoArquivo(const std::string& nomeArquivo);
};

// ========================
// Classe Dicionario
// ========================
class Dicionario {
private:
    BST arvore;
    ListaDupla listaTemp;

public:
    void inicializar(const std::string& arquivo);
    void menu();
};

#endif
