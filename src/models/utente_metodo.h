#ifndef UTENTE_METODO
#define UTENTE_METODO

#include <string>
#include "metodo_pagamento.h"
#include "utente.h"

class Utente_metodo {
    public:
        // COSTRUTTORE
        Utente_metodo(){}
        Utente_metodo(int id): id(id){}
        Utente_metodo(const int id, const Utente utente, const Metodo_pagamento& metodo_pagamento)
        : id(id), utente(utente), metodo_pagamento(metodo_pagamento) {}
        Utente_metodo(const int id, const Utente utente, const Metodo_pagamento& metodo_pagamento, const char* aggiunta)
    : id(id), utente(utente), metodo_pagamento(metodo_pagamento), aggiunta(strdup(aggiunta)) {}


    int getId() const {return id;}
    void setId(int id){this->id=id;} 
    Utente getUtente() const {return utente;}
    Metodo_pagamento getMetodo_pagamento() const {return metodo_pagamento;}
    char* getAggiunta() const {return aggiunta;}

    private:
        int id;
        Utente utente;
        Metodo_pagamento metodo_pagamento;
        char* aggiunta;
};

#endif
