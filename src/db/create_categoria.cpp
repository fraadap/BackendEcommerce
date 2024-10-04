#include "database.h"

Categoria Database::create_categoria(Categoria c){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(c.getNome()) == 0){
        std::sprintf(errMsg, "Il nome della categoria è null");
        throw NullParameterException(errMsg);
        return c;
    }    
    
    if (strlen(c.getNome())>255){
        std::sprintf(errMsg, "Il nome della categoria è troppo lungo: %s", c.getNome());
        throw InvalidParameterException(errMsg);
        return c;
    }

    //Controllo se già esiste la categoria
    sprintf(sqlcmd, "SELECT nome FROM categoria WHERE nome = '%s' ", c.getNome());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 1) {
        std::sprintf(errMsg, "la categoria %s già esiste", c.getNome());
        throw DuplicateKeyException(errMsg);
    }

    sprintf(sqlcmd, "BEGIN"); 
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);

    if (strlen(c.getPadre()) == 0){
        sprintf(sqlcmd, "INSERT INTO categoria VALUES ('%s', NULL) ON CONFLICT DO NOTHING", c.getNome());
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
    } else {
           sprintf(sqlcmd, "INSERT INTO categoria VALUES ('%s', '%s') ON CONFLICT DO NOTHING", c.getNome(), c.getPadre());
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
    } 
    
    sprintf(sqlcmd, "COMMIT"); 
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);

    return c;
}