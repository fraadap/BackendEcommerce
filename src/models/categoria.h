#ifndef CATEGORIA_H
#define CATEGORIA_H

#include <cstring>
#include <string>

class Categoria {
public:
    // Costruttore
    Categoria(){}
    Categoria(const char* nome): nome(strdup(nome)) {}
    Categoria(const char* nome, const char* padre): nome(strdup(nome)), categoria_padre(strdup(padre)) {}

    // Metodi per ottenere informazioni sugli attributi
    char* getPadre() const { return categoria_padre; }
    char* getNome() const { return nome; }

private:
    char* nome;
    char* categoria_padre;

};


#endif