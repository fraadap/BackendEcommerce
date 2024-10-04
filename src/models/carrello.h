#ifndef CARRELLO_H
#define CARRELLO_H

#include <cstring>
#include <string>
#include "utente.h"
#include "articolo.h"

class Carrello {
public:
    // Costruttore
    Carrello(){}
    Carrello(int id): id(id){}
    Carrello(int id, const Utente& utente, const Articolo& articolo, const char* aggiunta, int quantita):
        id(id), utente(utente), articolo(articolo), aggiunta(strdup(aggiunta)), quantita(quantita) {}

    // Metodi per ottenere informazioni sugli attributi
    int getId() const { return id; }
    void setId(int id){this->id=id;} 
    Utente getUtente() const { return utente; }
    Articolo getArticolo() const { return articolo; }
    char* getAggiunta() const { return aggiunta; }
    int getQuantita() const { return quantita; }

private:
    int id;
    Utente utente;
    Articolo articolo;
    char* aggiunta;
    int quantita;
};


#endif