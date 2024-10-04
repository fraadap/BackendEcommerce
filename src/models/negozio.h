#ifndef NEGOZIO_H
#define NEGOZIO_H

#include <cstring>
#include <string>
#include "indirizzo.h"
#include "metodo_pagamento.h"

class Negozio {
    public:
        // COSTRUTTORE
        Negozio(){}
        Negozio(const int id): id(id){}
        Negozio(const char* nome, const char* password):nome(strdup(nome)), password(strdup(password)){}
        Negozio(const int id, const char* nome, const char* telefono, const char* email, const Indirizzo& indirizzo, const Metodo_pagamento& metodo_pagamento, const char* password):
        id(id), nome(strdup(nome)), telefono(strdup(telefono)), email(strdup(email)), indirizzo(indirizzo), metodo_pagamento(metodo_pagamento), password(strdup(password)) {}

    int getId() const {return id;}
    void setId(int id){this->id=id;} 
    char* getNome() const {return nome;}
    char* getTelefono() const {return telefono;}
    char* getEmail() const {return email;}
    char* getPassword() const {return password;}
    Indirizzo getIndirizzo() const {return indirizzo;}
    Metodo_pagamento getMetodo_pagamento() const {return metodo_pagamento;}
    void setIndirizzo(Indirizzo in) {indirizzo = in;}
    void setMetodoPagamento(Metodo_pagamento mp) {metodo_pagamento = mp;}
    

    private:
        int id;
        char* nome;
        char* telefono;
        char* email; 
        Indirizzo indirizzo;
        Metodo_pagamento metodo_pagamento;
        char* password; 

};


#endif