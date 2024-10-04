#include "indirizzo.h"

// Costruttore
Indirizzo::Indirizzo(int id, std::string& via, std::string& civico, int interno, Cap& zona)
    : id(id), via(via), civico(civico), interno(interno), zona(zona) {}
