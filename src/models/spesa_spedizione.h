#ifndef SPESA_SPEDIZIONE_H
#define SPESA_SPEDIZIONE_H
#include <string>
#include "articolo.h"

class SpesaSpedizione {
public:
    // Costruttore
    SpesaSpedizione(){}
    SpesaSpedizione(int id, const char* tipologia, double prezzo, const Articolo& articolo)
    : id(id), tipologia(strdup(tipologia)), prezzo(prezzo), articolo(articolo) {}


    // Metodi per ottenere informazioni sugli attributi
    int getId() const { return id; }
    void setId(int id){this->id=id;} 
    char* getTipologia() const { return tipologia; }
    double getPrezzo() const { return prezzo; }
    Articolo getArticolo() const { return articolo; }

private:
    int id;
    char* tipologia;
    double prezzo;
    Articolo articolo; // Utilizzo di un oggetto Articolo come attributo
};

#endif