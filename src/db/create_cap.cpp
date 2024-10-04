#include "database.h"

Cap Database::create_cap(Cap c){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(c.getCodice()) == 0 || strlen(c.getCitta().getNome()) == 0){
        std::sprintf(errMsg, "Uno dei parametri del cap è null: %s %s", c.getCodice(), c.getCitta().getNome());
        throw NullParameterException(errMsg);
        return c;
    }    
    
    if (strlen(c.getCodice())>5){
        std::sprintf(errMsg, "Il codice del cap è troppo lungo: %s", c.getCodice());
        throw InvalidParameterException(errMsg);
        return c;
    }
    // Check per la chiave
    sprintf(sqlcmd, "SELECT codice FROM cap WHERE codice = '%s'", c.getCodice());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        return c;
    } else {
        try{
            c.setCitta(this->create_citta(c.getCitta())); // se la città non esiste viene creata, altrimenti solo settata
        }catch(const std::exception& e){
            throw;
            return c;
        }
        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO cap VALUES ('%s', '%d')", c.getCodice(), c.getCitta().getId());
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
    }
    

    return c;
}