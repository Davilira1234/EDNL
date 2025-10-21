#include "dicionario.hpp"
#include <fstream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

// ========================
// Palavra
// ========================
Palavra::Palavra(string i, string p, string c) {
    ingles = toLower(i);
    portugues = p;
    classificacao = c;
}

string Palavra::toLower(string s) {
    for (char &c : s) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return s;
}

// ========================
// NoBST
// ========================
NoBST::NoBST(const Palavra &p) : palavra(p), esq(nullptr), dir(nullptr) {}

// ========================
// BST
// ========================
BST::BST() : raiz(nullptr) {}

NoBST* BST::inserir(NoBST* r, const Palavra &p) {
    if (r == nullptr) return new NoBST(p);
    if (p.ingles < r->palavra.ingles) r->esq = inserir(r->esq, p);
    else if (p.ingles > r->palavra.ingles) r->dir = inserir(r->dir, p);
    return r;
}

void BST::inserir(const Palavra &p) {
    raiz = inserir(raiz, p);
}

NoBST* BST::buscar(NoBST* r, const string &ingles) {
    if (r == nullptr || r->palavra.ingles == ingles) return r;
    if (ingles < r->palavra.ingles) return buscar(r->esq, ingles);
    return buscar(r->dir, ingles);
}

Palavra* BST::buscar(const string &ingles) {
    NoBST* res = buscar(raiz, Palavra::toLower(ingles));
    return (res ? &res->palavra : nullptr);
}

NoBST* BST::minimo(NoBST* no) {
    while (no && no->esq) no = no->esq;
    return no;
}

NoBST* BST::remover(NoBST* r, const string &ingles) {
    if (r == nullptr) return r;
    if (ingles < r->palavra.ingles) r->esq = remover(r->esq, ingles);
    else if (ingles > r->palavra.ingles) r->dir = remover(r->dir, ingles);
    else {
        if (r->esq == nullptr) {
            NoBST* temp = r->dir;
            delete r;
            return temp;
        } else if (r->dir == nullptr) {
            NoBST* temp = r->esq;
            delete r;
            return temp;
        }
        NoBST* temp = minimo(r->dir);
        r->palavra = temp->palavra;
        r->dir = remover(r->dir, temp->palavra.ingles);
    }
    return r;
}

void BST::remover(const string &ingles) {
    raiz = remover(raiz, Palavra::toLower(ingles));
}

void BST::emOrdem(NoBST* r) {
    if (r != nullptr) {
        emOrdem(r->esq);
        cout << r->palavra.ingles << endl;
        emOrdem(r->dir);
    }
}

void BST::listarEmOrdem() {
    emOrdem(raiz);
}

void BST::listarEmLargura() {
    if (raiz == nullptr) return;
    queue<NoBST*> fila;
    fila.push(raiz);
    while (!fila.empty()) {
        NoBST* atual = fila.front();
        fila.pop();
        cout << atual->palavra.ingles << endl;
        if (atual->esq) fila.push(atual->esq);
        if (atual->dir) fila.push(atual->dir);
    }
}

// ========================
// NoLista
// ========================
NoLista::NoLista(Palavra p) : dado(p), prox(nullptr), ant(nullptr) {}

// ========================
// ListaDupla
// ========================
ListaDupla::ListaDupla() : cabeca(nullptr), cauda(nullptr) {}

void ListaDupla::adicionar(Palavra p) {
    NoLista* novo = new NoLista(p);
    if (cauda == nullptr) cabeca = cauda = novo;
    else {
        cauda->prox = novo;
        novo->ant = cauda;
        cauda = novo;
    }
}

bool ListaDupla::vazia() const {
    return cabeca == nullptr;
}

int ListaDupla::tamanho() const {
    int cont = 0;
    NoLista* atual = cabeca;
    while (atual) { cont++; atual = atual->prox; }
    return cont;
}

Palavra ListaDupla::removerPorIndice(int i) {
    int tam = tamanho();
    if (i < 0 || i >= tam) return Palavra();

    NoLista* no;
    if (i < tam / 2) {
        no = cabeca;
        for (int j = 0; j < i; j++) no = no->prox;
    } else {
        no = cauda;
        for (int j = tam - 1; j > i; j--) no = no->ant;
    }

    if (no->ant) no->ant->prox = no->prox;
    else cabeca = no->prox;
    if (no->prox) no->prox->ant = no->ant;
    else cauda = no->ant;

    Palavra dado = no->dado;
    delete no;
    return dado;
}

Palavra ListaDupla::removerAleatorio() {
    int tam = tamanho();
    if (tam == 0) return Palavra();
    int i = rand() % tam;
    return removerPorIndice(i);
}

void ListaDupla::carregarDoArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro: arquivo '" << nomeArquivo << "' nao encontrado.\n";
        return;
    }

    string linha;
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        size_t p1 = linha.find(';');
        size_t p2 = linha.find(';', p1 + 1);
        if (p1 == string::npos || p2 == string::npos) continue;

        string ing = linha.substr(0, p1);
        string pt = linha.substr(p1 + 1, p2 - p1 - 1);
        string cl = linha.substr(p2 + 1);

        auto trim = [](string s) {
            while (!s.empty() && isspace(static_cast<unsigned char>(s[0]))) s.erase(0, 1);
            while (!s.empty() && isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
            return s;
        };

        ing = trim(ing);
        pt = trim(pt);
        cl = trim(cl);

        if (!ing.empty() && !pt.empty() && !cl.empty())
            adicionar(Palavra(ing, pt, cl));
    }
    arquivo.close();
}

// ========================
// Dicionario
// ========================
void Dicionario::inicializar(const string& arquivo) {
    srand(time(0));
    listaTemp.carregarDoArquivo(arquivo);
    int total = listaTemp.tamanho();
    cout << "Carregadas " << total << " palavras. Inserindo na BST de forma aleatoria...\n";

    while (!listaTemp.vazia()) {
        arvore.inserir(listaTemp.removerAleatorio());
    }
    cout << "Dicionario inicializado!\n\n";
}

void Dicionario::menu() {
    int opcao;
    do {
        cout << "=== DICIONARIO DE INGLES ===\n";
        cout << "1. Buscar palavra\n";
        cout << "2. Remover palavra\n";
        cout << "3. Adicionar nova palavra\n";
        cout << "4. Listar em ordem (In-Order)\n";
        cout << "5. Listar em largura (Level-Order)\n";
        cout << "0. Sair\n";
        cout << "Opcao: ";
        cin >> opcao;
        cin.ignore();

        string ing, pt, cl;
        Palavra* res;

        switch (opcao) {
            case 1:
                cout << "Palavra em ingles: ";
                getline(cin, ing);
                res = arvore.buscar(ing);
                if (res) cout << "-> " << res->portugues << " (" << res->classificacao << ")\n";
                else cout << "Palavra nao encontrada.\n";
                break;

            case 2:
                cout << "Palavra a remover: ";
                getline(cin, ing);
                arvore.remover(ing);
                cout << "Operacao concluida.\n";
                break;

            case 3:
                cout << "Ingles: "; getline(cin, ing);
                cout << "Portugues: "; getline(cin, pt);
                cout << "Classificacao: "; getline(cin, cl);
                arvore.inserir(Palavra(ing, pt, cl));
                cout << "Palavra adicionada.\n";
                break;

            case 4:
                cout << "Palavras em ordem:\n";
                arvore.listarEmOrdem();
                break;

            case 5:
                cout << "Palavras em largura:\n";
                arvore.listarEmLargura();
                break;

            case 0:
                cout << "Encerrando...\n";
                break;

            default:
                cout << "Opcao invalida.\n";
        }
        cout << endl;
    } while (opcao != 0);
}
