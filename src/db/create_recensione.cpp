#include "database.h"

Recensione Database::create_recensione(Recensione r){ 
    // da implementare update per la quantità
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100]; 
    
    if ( r.getValutazione() < 0 || r.getValutazione() > 5){
        std::sprintf(errMsg, "Qualche parametro della recensione è errato");
        throw InvalidParameterException(errMsg);
        return r;
    }

    if (r.getUtente().getId() == 0 || r.getArticolo().getId() == 0){
        std::sprintf(errMsg, "l'articolo o l'utente della recensione è null");
        throw NullParameterException(errMsg);
        return r;
    }   

    //Controllo se esiste effettivamente l'utente
    sprintf(sqlcmd, "SELECT ID FROM utente WHERE id = %d ", r.getUtente().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'utente specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }
        //Controllo se esiste effettivamente l'articolo
    sprintf(sqlcmd, "SELECT ID FROM articolo WHERE id = %d ", r.getArticolo().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'articolo specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, "SELECT ID FROM recensione WHERE articolo = %d AND utente = %d", r.getArticolo().getId(), r.getUtente().getId());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        char message[100];
        std::sprintf(message, "La recensione esiste già per l'articolo: %d e utente: %d", r.getArticolo().getId(), r.getUtente().getId());
        throw DuplicateKeyException(message);
    } else {
        char* timestamp = getCurrentTimestamp();

        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO Recensione VALUES (DEFAULT, %d, %d, '%s', %d, '%s') ON CONFLICT DO NOTHING RETURNING ID", r.getUtente().getId(), r.getArticolo().getId(), timestamp, r.getValutazione(), r.getDescrizione());
        res = conn->ExecSQLtuples(sqlcmd);
        int id = atoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        
        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        r.setId(id);
    }
    return r;
}