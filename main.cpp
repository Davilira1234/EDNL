#include "dicionario.hpp"

int main() {
    Dicionario dic;
    dic.inicializar("palavras.txt");
    dic.menu();
    return 0;
}
