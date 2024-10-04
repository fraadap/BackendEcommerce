#ifndef UTENTE_H
#define UTENTE_H

#include <string>
#include "indirizzo.h"

class Utente {
    public:
        // COSTRUTTORE
        Utente(){}
        Utente(const int id): id(id){}
        Utente(const int id, const char* username): id(id), username(strdup(username)){}
        Utente(const char* username, const char* password): username(strdup(username)), password(strdup(password)){}
        Utente(const int id, const char* username, const char* nome, const char* cognome, const char* registrazione, const char* email, Indirizzo indirizzo, const char* password)
            : id(id), username(strdup(username)), nome(strdup(nome)), cognome(strdup(cognome)), registrazione(strdup(registrazione)), email(strdup(email)), indirizzo(indirizzo), password(strdup(password)) {}

void setId(int id){this->id=id;} 
int getId() const {return id;}
char* getUsername() const {return username;}
char* getNome() const {return nome;}
char* getCognome() const {return cognome;}
char* getRegistrazione() const {return registrazione;}
char* setRegistrazione(char* r) {return registrazione = r;}
char* getEmail() const {return email;}
char* getPassword() const {return password;}
Indirizzo getIndirizzo() const {return indirizzo;}
void setIndirizzo(Indirizzo in) {indirizzo = in;}
void setUsername(char* us){username = us;}

    private:
        int id;
        char* username;
        char* nome;
        char* cognome;
        char* registrazione;
        char* email; 
        Indirizzo indirizzo;
        char* password;

};

#endif