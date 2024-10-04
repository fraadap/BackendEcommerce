#ifndef CITTA_H
#define CITTA_H

#include <string>
#include "nazione.h"

class Citta {
    public:
        // COSTRUTTORE
        Citta(){}
        Citta(const int id, const char* nome, const Nazione& nazione) : id(id), nome(strdup(nome)), nazione(nazione) {}

    int getId() const {return id;}
    void setId(int id){this->id=id;} 
    char* getNome() const {return nome;}
    Nazione getNazione() const {return nazione;}

    private:
        int id;
        char* nome;
        Nazione nazione;

};

#endif