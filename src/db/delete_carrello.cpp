#include "database.h"

void Database::delete_carrello(Carrello c){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (c.getId() == 0){
        std::sprintf(errMsg, "L'id del carrello non è settato");
        throw NullParameterException(errMsg);
    }

    sprintf(sqlcmd, "DELETE FROM carrello WHERE id = %d returning id", c.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        PQclear(res);
        std::sprintf(errMsg, "Carrello non trovato");
        throw RecordNotFoundException(errMsg);
    } 
}