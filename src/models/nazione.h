#ifndef NAZIONE_H
#define NAZIONE_H
#include <cstring>

class Nazione {
    public:
        // COSTRUTTORE
        Nazione(){}
        Nazione( const char* nome):nome(strdup(nome)){}

    char* getNome() const {return nome;}

    private:
        char* nome;

};

#endif