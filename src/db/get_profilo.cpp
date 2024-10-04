#include "database.h"

Profilo Database::get_profilo(Utente ut){

    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (ut.getId() == 0 ) {
        std::sprintf(errMsg, "L'id di utente è null");
        throw NullParameterException(errMsg);
    }    
    

    sprintf(sqlcmd, "SELECT ID FROM utente WHERE id = %d", ut.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        std::sprintf(errMsg, "Utente specificato non trovato");
        throw RecordNotFoundException(errMsg);
    }
    
        sprintf(sqlcmd, 
    "SELECT username from utente where id = %d",
    ut.getId());

    res = conn->ExecSQLtuples(sqlcmd);
    char* username = PQgetvalue(res, 0, 0);
    ut.setUsername(username);
    PQclear(res);



    std::vector<Metodo_pagamento> metodi = this->get_metodi_pagamento(ut);

    std::vector<Carrello> carrelli = this->get_carrelli(ut);

    std::vector<Lista_desideri> liste = this->get_lista_desideri(ut);


    Profilo profilo(ut, metodi, carrelli, liste);


    return profilo;
}