#include "articolo.h"

// Costruttore
Articolo::Articolo(int id, const std::string& nome, const std::string& modello, const std::string& marca,
                   const std::string& descrizione, int disponibilita, double prezzo,
                   const std::string& pubblicazione, bool offerta, const std::string& inizio_offerta,
                   double percentuale, int durata_ore, const Categoria& categoria, const Negozio& negozio)
    : id(id), nome(nome), modello(modello), marca(marca), descrizione(descrizione),
      disponibilita(disponibilita), prezzo(prezzo), pubblicazione(pubblicazione),
      offerta(offerta), inizio_offerta(inizio_offerta), percentuale(percentuale),
      durata_ore(durata_ore), categoria(categoria), negozio(negozio) {}

// Se ci sono altri metodi da implementare, aggiungili qui.

