#include "database.h"

void Database::remove_metodo_pagamento_from_utente(Utente_metodo um){ // si deve pensare alla password
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (um.getId() == 0){

        if (um.getUtente().getId() == 0 || um.getMetodo_pagamento().getId() == 0) {
            std::sprintf(errMsg, "L'id di utente o del metodo di pagamento è null");
            throw NullParameterException(errMsg);
        }    
    

        sprintf(sqlcmd, "SELECT ID FROM utente WHERE id = %d", um.getUtente().getId());
        res = conn->ExecSQLtuples(sqlcmd);
        if (PQntuples(res) == 0) {
            std::sprintf(errMsg, "Utente specificato non trovato");
            throw RecordNotFoundException(errMsg);
        }

        sprintf(sqlcmd, "SELECT ID FROM metodo_pagamento WHERE id = %d", um.getMetodo_pagamento().getId());
        res = conn->ExecSQLtuples(sqlcmd);
        if (PQntuples(res) == 0) {
            std::sprintf(errMsg, "metodo pagamento specificato non trovato");
            throw RecordNotFoundException(errMsg);
        }


        sprintf(sqlcmd, "DELETE FROM utente_metodo WHERE utente = %d AND metodo_pagamento = %d returning id", um.getUtente().getId(), um.getMetodo_pagamento().getId());
        res = conn->ExecSQLtuples(sqlcmd);
                
        if (PQntuples(res) == 0) {
            std::sprintf(errMsg, "Il metodo di pagamento specificato non appartiene a quell'utente");
            throw RecordNotFoundException(errMsg);
        }
    } else{
        sprintf(sqlcmd, "DELETE FROM utente_metodo WHERE id = %d returning id", um.getId());
        res = conn->ExecSQLtuples(sqlcmd);
                
        if (PQntuples(res) == 0) {
            std::sprintf(errMsg, "Il metodo di pagamento specificato non appartiene a quell'utente");
            throw RecordNotFoundException(errMsg);
        }
    }

}