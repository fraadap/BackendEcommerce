#include "database.h"


Ordine_corriere Database::add_ordine_to_corriere(Ordine_corriere oc){ // si deve pensare alla password

    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (oc.getCorriere().getId() == 0 || oc.getOrdine().getId() == 0 ) {
        std::sprintf(errMsg, "Uno dei parametri di ordine o corriere è null");
        throw NullParameterException(errMsg);
        return oc;
    }    

    sprintf(sqlcmd, "SELECT ID FROM ordine_corriere WHERE corriere = %d AND ordine = %d AND (fine IS NULL OR fine < CURRENT_TIMESTAMP);", oc.getCorriere().getId(), oc.getOrdine().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) > 0) {
        std::sprintf(errMsg, "Il corriere è già assegnato a questo ordine");
        throw DuplicateKeyException(errMsg);
    }


    //Controllo se esiste effettivamente il corriere
    sprintf(sqlcmd, "SELECT ID FROM corriere WHERE id = %d ", oc.getCorriere().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "Il corriere specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    //Controllo se esiste effettivamente il corriere
    sprintf(sqlcmd, "SELECT ID FROM ordine WHERE id = %d ", oc.getOrdine().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'ordine specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    if (strlen(oc.getInizio()) == 0){
        char* timestamp = getCurrentTimestamp();
        oc.setInizio(timestamp);
    }


    sprintf(sqlcmd, "BEGIN"); 
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);

    if (strlen(oc.getFine()) == 0){
            sprintf(sqlcmd, "INSERT INTO ordine_corriere VALUES (DEFAULT, '%d', '%d', '%s', NULL ) RETURNING ID", oc.getOrdine().getId(), oc.getCorriere().getId(), oc.getInizio());
        res = conn->ExecSQLtuples(sqlcmd);
    }
    else{
        sprintf(sqlcmd, "INSERT INTO ordine_corriere VALUES (DEFAULT, '%d', '%d', '%s', '%s' ) RETURNING ID", oc.getOrdine().getId(), oc.getCorriere().getId(), oc.getInizio(), oc.getFine());
        res = conn->ExecSQLtuples(sqlcmd);
    }

    int id = atoi(PQgetvalue(res, 0, 0));
    oc.setId(id); 

    sprintf(sqlcmd, "COMMIT"); 
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);

    return oc;
}