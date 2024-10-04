#include "database.h"

Carrello Database::create_carrello(Carrello c){ 
    // da implementare update per la quantità
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100]; 
    
    if ( c.getQuantita() < 0){
        std::sprintf(errMsg, "Qualche parametro del carrello è errato");
        throw InvalidParameterException(errMsg);
        return c;
    }

    if (c.getUtente().getId() == 0 || c.getArticolo().getId() == 0){
        std::sprintf(errMsg, "l'articolo o l'utente del carrello è null");
        throw NullParameterException(errMsg);
        return c;
    }   

    //Controllo se esiste effettivamente l'utente
    sprintf(sqlcmd, "SELECT ID FROM utente WHERE id = %d ", c.getUtente().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'utente specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    //Controllo se esiste effettivamente l'utente
    sprintf(sqlcmd, "SELECT ID FROM articolo WHERE id = %d ", c.getArticolo().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'articolo specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, "SELECT ID FROM carrello WHERE articolo = %d AND utente = %d", c.getArticolo().getId(), c.getUtente().getId());
    res = conn->ExecSQLtuples(sqlcmd);

    
    if (PQntuples(res) > 0) {
        char message[100];
        std::sprintf(message, "Il carrello esiste già per l'articolo: %d e utente: %d", c.getArticolo().getId(), c.getUtente().getId());
        throw DuplicateKeyException(message);
    } else {
        char* timestamp = getCurrentTimestamp();

        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO Carrello VALUES (DEFAULT, %d, %d, '%s', %d) ON CONFLICT DO NOTHING RETURNING ID", c.getUtente().getId(), c.getArticolo().getId(), timestamp, c.getQuantita());
        res = conn->ExecSQLtuples(sqlcmd);
        int id = atoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        
        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        c.setId(id);
    }
    return c;
}