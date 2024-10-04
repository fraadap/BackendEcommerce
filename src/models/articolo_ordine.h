#ifndef ARTICOLO_ORDINE_H
#define ARTICOLO_ORDINE_H


#include <string>
#include "utente.h"
#include "ordine.h"
#include "articolo.h"

class Articolo_ordine {
public:
    // Costruttore
    Articolo_ordine(int id, Ordine& ordine, Articolo& articolo, int quantita)
        : id(id), ordine(ordine), articolo(articolo), quantita(quantita) {}


    // Metodi per ottenere informazioni sugli attributi
    int getId() const { return id; }
    void setId(int id){this->id=id;} 
    Ordine getOrdine() const { return ordine; }
    Articolo getArticolo() const { return articolo; }
    int getQuantita() const{ return quantita; }

private:
    int id;
    Ordine ordine;
    Articolo articolo;
    int quantita;
    
};


#endif