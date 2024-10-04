#ifndef CORRIERE_H
#define CORRIERE_H

#include <cstring>
#include <string>

class Corriere {
    public:
        // COSTRUTTORE
        Corriere(){}
        Corriere(const int id): id(id){}
        Corriere(const int id, const char* nome, const char* cognome): id(id), nome(strdup(nome)), cognome(strdup(cognome)) {}

    void setId(int id){this->id=id;}
    int getId(){return this->id;} 
    char* getNome() const {return nome;}
    char* getCognome() const {return cognome;}
    private:
        int id;
        char* nome;
        char* cognome;

};


#endif