#ifndef INDIRIZZO_H
#define INDIRIZZO_H

#include <cstring>
#include <string>
#include "cap.h"

class Indirizzo {
    public:
        // COSTRUTTORE
        Indirizzo(){}
        Indirizzo(int id): id(id){}
        Indirizzo(int id, const char* via, const char* civico, int interno, Cap& zona) : id(id), via(strdup(via)), civico(strdup(civico)), interno(interno), zona(zona) {}

        int getId() const {return id;}
        void setId(int id){this->id=id;} 
        char* getVia() const {return via;}
        char* getCivico() const {return civico;}
        int getInterno() const {return interno;}
        Cap getZona() const {return zona;}
        void setZona(Cap cap) {zona = cap;}

    private:
        int id;
        char* via;
        char* civico;
        int interno;
        Cap zona;

};

#endif