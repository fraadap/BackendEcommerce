#ifndef RECENSIONE_H
#define RECENSIONE_H

#include <cstring>
#include <string>
#include "utente.h"
#include "articolo.h"

class Recensione {
public:
    // Costruttore
    Recensione(){}
    Recensione(int id): id(id){}
    Recensione(int id, const Utente& utente, const Articolo& articolo, const char* aggiunta, int valutazione, const char* descrizione):
        id(id), utente(utente), articolo(articolo), aggiunta(strdup(aggiunta)), valutazione(valutazione), descrizione(strdup(descrizione)) {}

    // Metodi per ottenere informazioni sugli attributi
    int getId() const { return id; }
    void setId(int id){this->id=id;} 
    Utente getUtente() const { return utente; }
    Articolo getArticolo() const { return articolo; }
    char* getAggiunta() const { return aggiunta; }
    int getValutazione() const { return valutazione; }
    char* getDescrizione() const { return descrizione; }

private:
    int id;
    Utente utente;
    Articolo articolo;
    char* aggiunta;
    int valutazione;
    char* descrizione;
};


#endif