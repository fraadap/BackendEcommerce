#include "Utente.h"

Utente::Utente(const int id, const std::string& username, const std::string& nome, const std::string& cognome, const std::string& registrazione, const std::string& email, const Indirizzo& indirizzo)
    : id(id), username(username), nome(nome), cognome(cognome), registrazione(registrazione), email(email), indirizzo(indirizzo) {}
