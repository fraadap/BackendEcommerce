#include "database.h"

Lista_desideri Database::create_lista_desideri(Lista_desideri ld){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100]; 
    
    if (ld.getUtente().getId() == 0 || ld.getArticolo().getId() == 0){
        std::sprintf(errMsg, "l'articolo o l'utente della lista desideri è null");
        throw NullParameterException(errMsg);
        return ld;
    }   


    //Controllo se esiste effettivamente l'utente
    sprintf(sqlcmd, "SELECT ID FROM utente WHERE id = %d ", ld.getUtente().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'utente specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    //Controllo se esiste effettivamente l'articolo
    sprintf(sqlcmd, "SELECT ID FROM articolo WHERE id = %d ", ld.getArticolo().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'articolo specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, "SELECT ID FROM lista_desideri WHERE articolo = %d AND utente = %d", ld.getArticolo().getId(), ld.getUtente().getId());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        char message[100];
        std::sprintf(message, "La lista desideri esiste già per l'articolo: %d e utente: %d", ld.getArticolo().getId(), ld.getUtente().getId());
        throw DuplicateKeyException(message);
    } else {
        char* timestamp = getCurrentTimestamp();

        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO Lista_desideri VALUES (DEFAULT, %d, %d, '%s') ON CONFLICT DO NOTHING RETURNING ID", ld.getUtente().getId(), ld.getArticolo().getId(), timestamp);
        res = conn->ExecSQLtuples(sqlcmd);
        int id = atoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        
        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        ld.setId(id);
    }
    return ld;
}