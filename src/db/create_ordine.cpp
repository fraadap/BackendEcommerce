#include "database.h"

Ordine Database::create_ordine(Ordine o) {
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(o.getStato()) == 0 || o.getUtente().getId() == 0 || o.getMetodo_pagamento().getId() == 0){
        std::sprintf(errMsg, "Uno dei parametri dell'ordine è null");
        throw NullParameterException(errMsg);
        return o;
    }

    //Controllo se esiste effettivamente l'utente
    sprintf(sqlcmd, "SELECT ID FROM utente WHERE id = %d ", o.getUtente().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'utente specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    //Controllo se esiste effettivamente il metodo di pagamento
    sprintf(sqlcmd, "SELECT ID FROM metodo_pagamento WHERE id = %d ", o.getMetodo_pagamento().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "Il metodo pagamento specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, "SELECT ID FROM utente_metodo WHERE utente = %d AND metodo_pagamento = %d", o.getUtente().getId(), o.getMetodo_pagamento().getId());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) == 0) {
        char message[100];
        std::sprintf(message, "Il metodo di pagamento specificato non appartiene a quell'utente");
        throw RecordNotFoundException(message);
    } else {
        char* timestamp = getCurrentTimestamp();

        sprintf(sqlcmd, "INSERT INTO Ordine (Istante, OrderStatus, Utente, Metodo_pagamento) VALUES ('%s', '%s', %d, %d) RETURNING ID",
                timestamp, o.getStato(), o.getUtente().getId(), o.getMetodo_pagamento().getId());

        res = conn->ExecSQLtuples(sqlcmd);

        int id = atoi(PQgetvalue(res, 0, 0));
        o.setId(id);

        PQclear(res);
    }

    return o;
}
