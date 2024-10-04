#include "database.h"
#include <cstring>

Citta Database::create_citta(Citta c){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(c.getNome()) == 0 || strlen(c.getNazione().getNome()) == 0 ){
        std::sprintf(errMsg, "Uno dei parametri di città è null: %s %s", c.getNome(), c.getNazione().getNome());
        throw NullParameterException(errMsg);
        return c;
    }    
    
    if (strlen(c.getNome())>255){
        std::sprintf(errMsg, "Il nome della città è troppo lungo: %s", c.getNome());
        throw InvalidParameterException(errMsg);
        return c;
    }

    // Check per la chiave
    sprintf(sqlcmd, "SELECT ID FROM citta WHERE nome = '%s' AND nazione = '%s'", c.getNome(), c.getNazione().getNome());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        int id = atoi(PQgetvalue(res, 0, 0));
        c.setId(id);
    } else {
        
        try{
            this->create_nazione(c.getNazione());
        }catch(const std::exception& e){
            throw;
            return c;
        }

        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO citta VALUES (DEFAULT, '%s', '%s') RETURNING ID", c.getNome(), c.getNazione().getNome());
        res = conn->ExecSQLtuples(sqlcmd);
        
        int id = atoi(PQgetvalue(res, 0, 0));
        c.setId(id);

        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
    }
    

    return c;
}