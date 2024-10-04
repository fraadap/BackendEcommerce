#include "database.h"

SpesaSpedizione Database::create_spesa_spedizione(SpesaSpedizione ss){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100]; 
    
    if (strlen(ss.getTipologia())>255 ||  ss.getPrezzo() < 0 || ss.getArticolo().getId() == 0){
        std::sprintf(errMsg, "Qualche parametro della spesa spedizione sono errati");
        throw InvalidParameterException(errMsg);
        return ss;
    }

    //Controllo se esiste effettivamente l'articolo
    sprintf(sqlcmd, "SELECT ID FROM articolo WHERE id = %d ", ss.getArticolo().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'articolo specificato non esiste");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, "SELECT ID FROM spesa_spedizione WHERE articolo = %d AND tipologia = '%s'", ss.getArticolo().getId(), ss.getTipologia());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        char message[100];
        std::sprintf(message, "La spesa spedizione '%s' esiste già per l'articolo: %d", ss.getTipologia(), ss.getArticolo().getId());
        throw DuplicateKeyException(message);
    } else {
        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO Spesa_spedizione VALUES (DEFAULT, '%s', %.2f, %d) ON CONFLICT DO NOTHING RETURNING ID", ss.getTipologia(), ss.getPrezzo(), ss.getArticolo().getId());
        res = conn->ExecSQLtuples(sqlcmd);
        int id = atoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        
        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        ss.setId(id);
    }
    return ss;
}