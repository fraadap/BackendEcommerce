#include "lista_desideri.h"

// Costruttore
Lista_desideri::Lista_desideri(const int id, const Utente& utente, const Articolo& articolo, const std::string* datetime)
    : id(id), utente(utente), articolo(articolo), datetime(*datetime) {}
