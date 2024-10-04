#include "database.h"

void Database::delete_lista_preferiti(Lista_desideri ld){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (ld.getId() == 0){
        std::sprintf(errMsg, "L'id della lista desideri non è settato");
        throw NullParameterException(errMsg);
    }

    sprintf(sqlcmd, "DELETE FROM lista_desideri WHERE id = %d returning id", ld.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        PQclear(res);
        std::sprintf(errMsg, "Lista dei desideri non trovata");
        throw RecordNotFoundException(errMsg);
    } 
}