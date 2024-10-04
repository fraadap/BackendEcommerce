#ifndef ORDINE_H
#define ORDINE_H

#include <string>
#include "utente.h"
#include "metodo_pagamento.h"

class Ordine {
public:
    // Costruttore
    Ordine(){}
    Ordine(int id):id(id){}
    Ordine(int id, const char* istante, const char* stato, const Utente& utente, const Metodo_pagamento& metodo_pagamento)
    : id(id), istante(strdup(istante)), stato(strdup(stato)), utente(utente), metodo_pagamento(metodo_pagamento) {}


    // Metodi per ottenere informazioni sugli attributi
    int getId() const { return id; }
    void setId(int id){this->id=id;} 
    Utente getUtente() const { return utente; }
    Metodo_pagamento getMetodo_pagamento() const { return metodo_pagamento; }
    char* getIstante() const { return istante; }
    char* getStato() const { return stato; }

private:
    int id;
    char* istante;
    char* stato;
    Utente utente;
    Metodo_pagamento metodo_pagamento;
};
#endif