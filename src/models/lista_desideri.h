#ifndef LISTA_DESIDERI_H
#define LISTA_DESIDERI_H

#include <string>
#include "utente.h"
#include "articolo.h"

class Lista_desideri {
public:
    // Costruttore
    Lista_desideri(){}
    Lista_desideri(int id): id(id){}
    Lista_desideri(const int id, const Utente& utente, const Articolo& articolo, const char* datetime)
        : id(id), utente(utente), articolo(articolo), datetime(strdup(datetime)) {}


    // Metodi per ottenere informazioni sugli attributi
    int getId() const {return id;}
    void setId(int id){this->id=id;} 
    Utente getUtente() const {return utente;}
    Articolo getArticolo() const {return articolo;}
    char* getDatetime() const {return datetime;}

private:
    int id;
    Utente utente;
    Articolo articolo;
    char* datetime;

};


#endif