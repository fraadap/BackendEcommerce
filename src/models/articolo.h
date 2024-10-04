#ifndef ARTICOLO_H
#define ARTICOLO_H

#include <string>
#include <chrono>
#include "categoria.h"
#include "negozio.h"

class Articolo {
    public:
        // COSTRUTTORE
Articolo(){}
Articolo(int id): id(id){}
Articolo(int id, const char* nome, const char* modello, const char* marca,
             const char* descrizione, int disponibilita, double prezzo,
             const char* pubblicazione, bool offerta, const char* inizio_offerta,
             double percentuale, int durata_ore, const Categoria& categoria, const Negozio& negozio)
    : id(id), nome(strdup(nome)), modello(strdup(modello)), marca(strdup(marca)),
      descrizione(strdup(descrizione)), disponibilita(disponibilita), prezzo(prezzo),
      pubblicazione(strdup(pubblicazione)), offerta(offerta), inizio_offerta(strdup(inizio_offerta)),
      percentuale(percentuale), durata_ore(durata_ore), categoria(categoria), negozio(negozio) {}

    int getId() const { return id; }
    void setId(int id){this->id=id;} 
    char* getNome() const { return nome; }
    char* getModello() const { return modello; }
    char* getMarca() const { return marca; }
    char* getDescrizione() const { return descrizione; }
    int getDisponibilita() const { return disponibilita; }
    double getPrezzo() const { return prezzo; }
    char* getPubblicazione() const { return pubblicazione; }
    bool isOfferta() const { return offerta; }
    char* getInizioOfferta() const { return inizio_offerta; }
    double getPercentuale() const { return percentuale; }
    int getDurataOre() const { return durata_ore; }
    Categoria getCategoria() const { return categoria; }
    Negozio getNegozio() const { return negozio; }

    private:
        int id;
        char* nome;
        char* modello;
        char* marca;
        char* descrizione;
        int disponibilita;
        double prezzo;
        char* pubblicazione;
        bool offerta;
        char* inizio_offerta;
        double percentuale;
        int durata_ore;
        Categoria categoria;
        Negozio negozio;


};

#endif