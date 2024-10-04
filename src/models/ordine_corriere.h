#ifndef ORDINE_CORRIERE_H
#define ORDINE_CORRIERE_H

#include <string>
#include "utente.h"
#include "ordine.h"
#include "corriere.h"

class Ordine_corriere {
public:
    // Costruttore
    Ordine_corriere(){}
    Ordine_corriere(int id, const Ordine& ordine, const Corriere& corriere, const char* inizio, const char* fine)
        : id(id), ordine(ordine), corriere(corriere), inizio(strdup(inizio)), fine(strdup(fine)) {}


    // Metodi per ottenere informazioni sugli attributi
    int getId() const { return id; }
    void setId(int id){this->id=id;} 
    Ordine getOrdine() const { return ordine; }
    Corriere getCorriere() const { return corriere; }
    char* getInizio() const { return inizio; }
    char* getFine() const { return fine; }
    void setInizio(char* in) {this->inizio =in;}


private:
    int id;
    Ordine ordine;
    Corriere corriere;
    char* inizio;
    char* fine;
    
};


#endif