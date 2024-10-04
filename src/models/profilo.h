#ifndef PROFILO_H
#define PROFILO_H

#include <string>
#include "lista_desideri.h"
#include "ordine.h"
#include "carrello.h"
#include "utente.h"
#include "metodo_pagamento.h"
#include <vector>

class Profilo {
public:
    // Costruttore
    Profilo(){}
    Profilo(Utente utente, std::vector<Metodo_pagamento> metodi, std::vector<Carrello> carrelli, 
    std::vector<Lista_desideri> liste) 
    : utente(utente), metodi(metodi), carrelli(carrelli), liste(liste) {}


    Utente getUtente() const { return utente; }
    std::vector<Metodo_pagamento> getMetodi() {return metodi;}
    std::vector<Carrello> getCarrelli(){return carrelli;}
    std::vector<Lista_desideri> getListeDesideri(){return liste;};


private:
    Utente utente;
    std::vector<Metodo_pagamento> metodi;
    std::vector<Carrello> carrelli;
    std::vector<Lista_desideri> liste;
};
#endif