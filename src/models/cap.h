#ifndef CAP_H
#define CAP_H

#include <string>
#include "citta.h"

class Cap {
    public:
        // COSTRUTTORE
        Cap(){}
        Cap(const char* codice, const Citta& citta) : codice(strdup(codice)),citta(citta) {}
    char* getCodice() const {return codice;}
    Citta getCitta() const {return citta;}
    void setCitta(Citta c) { citta = c;}
    
    private:
        char* codice;
        Citta citta;

};

#endif