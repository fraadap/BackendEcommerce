#ifndef METODO_PAGAMENTO_H
#define METODO_PAGAMENTO_H

#include <cstring>
#include <string>
#include <chrono>


class Metodo_pagamento {
    public:
        // COSTRUTTORE
        Metodo_pagamento(){}
        Metodo_pagamento(const int id): id(id){}
        Metodo_pagamento(const int id, const char* numero,  const char* titolare, const char* scadenza):
            id(id), numero(strdup(numero)), titolare(strdup(titolare)), scadenza(strdup(scadenza)) {}

        int getId() const {return id;}
        void setId(int id){this->id=id;} 
        char* getNumero() const {return numero;}
        char* getTitolare() const {return titolare;}
        char* getScadenza() const {return scadenza;}

    private:
        int id;
        char* numero;
        char* cvv; 
        char* titolare; 
        char* scadenza;


};

#endif