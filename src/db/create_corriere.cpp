#include "database.h"

Corriere Database::create_corriere(Corriere c) {
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];


    if (strlen(c.getNome()) == 0 || strlen(c.getCognome()) == 0) {
        std::sprintf(errMsg, "Nome e cognome del corriere devono essere non vuoti");
        throw NullParameterException(errMsg);
    }


    if (strlen(c.getNome()) > 30 || strlen(c.getCognome()) > 30) {
        std::sprintf(errMsg, "Nome e cognome del corriere devono avere lunghezza massima di 30 caratteri");
        throw InvalidParameterException(errMsg);
    }

    sprintf(sqlcmd, "INSERT INTO Corriere (Nome, Cognome) VALUES ('%s', '%s') RETURNING ID",
            c.getNome(), c.getCognome());

    res = conn->ExecSQLtuples(sqlcmd);

    int id = atoi(PQgetvalue(res, 0, 0));
    c.setId(id);

    PQclear(res);

    return c;
}
