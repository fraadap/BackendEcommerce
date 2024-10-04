#include "database.h"
#include <exception>


Metodo_pagamento Database::create_metodo_pagamento(Metodo_pagamento mp){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if ( strlen(mp.getNumero()) == 0 || strlen(mp.getTitolare()) == 0 || strlen(mp.getScadenza()) == 0){
        std::sprintf(errMsg, "Uno dei parametri del metodo di pagamento è null");
        throw NullParameterException(errMsg);
        return mp;
    }    

    if (strlen(mp.getNumero()) > 40 || strlen(mp.getTitolare()) > 100){
        std::sprintf(errMsg, "Uno dei parametri del metodo di pagamento non rispetta la lunghezza desiderata: %s %s", mp.getNumero(), mp.getTitolare());
        throw InvalidParameterException(errMsg);
        return mp;
    }    
        sprintf(sqlcmd, "SELECT ID FROM metodo_pagamento WHERE numero = '%s'",mp.getNumero());
        res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        int id = atoi(PQgetvalue(res, 0, 0));
        mp.setId(id);
        return mp;
    } else {


        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
        try{
            sprintf(sqlcmd, "INSERT INTO metodo_pagamento VALUES (DEFAULT, '%s', '%s', '%s') RETURNING ID", mp.getNumero(), mp.getTitolare(), mp.getScadenza());
            res = conn->ExecSQLtuples(sqlcmd);
        }catch(const std::exception& e){
            throw;
        }
        int id = atoi(PQgetvalue(res, 0, 0));
        mp.setId(id);

        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
    }
    return mp;
}