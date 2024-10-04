#include "database.h"

void Database::delete_recensione(Recensione r){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (r.getId() == 0){
        std::sprintf(errMsg, "L'id della recensione non è settato");
        throw NullParameterException(errMsg);
    }

    sprintf(sqlcmd, "DELETE FROM recensione WHERE id = %d returning id", r.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        PQclear(res);
        std::sprintf(errMsg, "Recensione non trovata");
        throw RecordNotFoundException(errMsg);
    } 
}