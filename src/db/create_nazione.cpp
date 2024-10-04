#include "database.h"

Nazione Database::create_nazione(Nazione n){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(n.getNome()) == 0){
        std::sprintf(errMsg, "Il nome della nazione è null");
        throw NullParameterException(errMsg);
        return n;
    }    
    
    if (strlen(n.getNome())>255){
        std::sprintf(errMsg, "Il nome della nazione è troppo lungo: %s", n.getNome());
        throw InvalidParameterException(errMsg);
        return n;
    }
    sprintf(sqlcmd, "BEGIN"); 
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);

    sprintf(sqlcmd, "INSERT INTO nazione VALUES (\'%s\') ON CONFLICT DO NOTHING", n.getNome());
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);
    
    sprintf(sqlcmd, "COMMIT"); 
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);

    return n;
}