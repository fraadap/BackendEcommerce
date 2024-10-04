#ifndef ADMIN_H
#define ADMIN_H

#include <cstring>
#include <string>

class Admin {
    public:
        // COSTRUTTORE
        Admin(){}
        Admin(const int id): id(id){}
        Admin(const int id, const char* nome, const char* pass): id(id), nome(strdup(nome)), pass(strdup(pass)) {}

    void setId(int id){this->id=id;}
    int getId(){return this->id;} 
    char* getNome() const {return nome;}
    char* getPassword() const {return pass;}
    private:
        int id;
        char* nome;
        char* pass;

};


#endif