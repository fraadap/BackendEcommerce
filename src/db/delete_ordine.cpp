#include "database.h"

void Database::delete_ordine(Ordine ord){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (ord.getId() == 0){
        std::sprintf(errMsg, "L'id dell'ordine non è settato");
        throw NullParameterException(errMsg);
    }

    sprintf(sqlcmd, "SELECT id FROM ordine WHERE id = %d", ord.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        PQclear(res);
        std::sprintf(errMsg, "Ordine non trovato");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, "SELECT id FROM ordine_corriere WHERE ordine = %d", ord.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) > 0) {
        PQclear(res);
        std::sprintf(errMsg, "L'ordine non può essere cancellato perché è stato già essegnato ad un corriere");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, "DELETE FROM ordine WHERE id = %d returning id", ord.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        PQclear(res);
        std::sprintf(errMsg, "Ordine non trovato");
        throw RecordNotFoundException(errMsg);
    } 
}