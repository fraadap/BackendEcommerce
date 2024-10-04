#include "carrello.h"

// Costruttore
Carrello::Carrello(int id, const Utente& utente, const Articolo& articolo,
                   const std::string& aggiunta, int quantita)
    : id(id), utente(utente), articolo(articolo), aggiunta(aggiunta), quantita(quantita) {}
