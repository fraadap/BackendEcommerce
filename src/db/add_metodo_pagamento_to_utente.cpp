#include "database.h"


Utente_metodo Database::add_metodo_pagamento_to_utente(Utente_metodo um){ // si deve pensare alla password
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (um.getUtente().getId() == 0) {
        std::sprintf(errMsg, "L'id di utente è null");
        throw NullParameterException(errMsg);
        return um;
    }    
 

    sprintf(sqlcmd, "SELECT ID FROM utente WHERE id = %d", um.getUtente().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) > 0) {
        try{
            int id = atoi(PQgetvalue(res, 0, 0));
            um.getUtente().setId(id);

            if (um.getMetodo_pagamento().getId() == 0){
                um.getMetodo_pagamento().setId(this->create_metodo_pagamento(um.getMetodo_pagamento()).getId());
            }

            char* timestamp = getCurrentTimestamp();

            sprintf(sqlcmd, "BEGIN"); 
            res = conn->ExecSQLcmd(sqlcmd);
            PQclear(res);

            sprintf(sqlcmd, "INSERT INTO Utente_metodo VALUES (DEFAULT, '%d', '%d', '%s' ) RETURNING ID", um.getUtente().getId(), um.getMetodo_pagamento().getId(), timestamp);
            res = conn->ExecSQLtuples(sqlcmd);
            
            id = atoi(PQgetvalue(res, 0, 0));
            um.setId(id);

            sprintf(sqlcmd, "COMMIT"); 
            res = conn->ExecSQLcmd(sqlcmd);
            PQclear(res);

        }catch(const std::exception& e){
            throw;
            return um;
        }
    } else {
        std::sprintf(errMsg, "Utente specificato non trovato");
        throw RecordNotFoundException(errMsg);
        return um; 

    }
    return um;
}