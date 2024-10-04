#include "database.h"


Articolo_ordine Database::add_articolo_to_ordine(Articolo_ordine ao){ // si deve pensare alla password

    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (ao.getArticolo().getId() == 0 || ao.getOrdine().getId() == 0 || ao.getQuantita() < 1) {
        std::sprintf(errMsg, "Uno dei parametri di Articolo o Ordine è null, oppure la quantità è minore di 1");
        throw NullParameterException(errMsg);
        return ao;
    }    

    //Controllo se esiste effettivamente l'articolo e l'ordine
    sprintf(sqlcmd, "SELECT ID FROM articolo WHERE id = %d ", ao.getArticolo().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'articolo non esiste");
        throw RecordNotFoundException(errMsg);
    }

    //Controllo se esiste effettivamente l'articolo e l'ordine
    sprintf(sqlcmd, "SELECT ID FROM ordine WHERE id = %d ", ao.getOrdine().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) == 0) {
        std::sprintf(errMsg, "L'ordine non esiste");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, "SELECT ID FROM articolo_ordine WHERE articolo = %d AND ordine = %d", ao.getArticolo().getId(), ao.getOrdine().getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if(PQntuples(res) > 0) {
        std::sprintf(errMsg, "L'articolo è già inserito nell'ordine");
        throw DuplicateKeyException(errMsg);
    }

    sprintf(sqlcmd, "BEGIN"); 
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);

    sprintf(sqlcmd, "INSERT INTO articolo_ordine VALUES (DEFAULT, '%d', '%d', '%d') RETURNING ID", ao.getOrdine().getId(), ao.getArticolo().getId(), ao.getQuantita());
    res = conn->ExecSQLtuples(sqlcmd);

    int id = atoi(PQgetvalue(res, 0, 0));
    ao.setId(id); 

    sprintf(sqlcmd, "COMMIT"); 
    res = conn->ExecSQLcmd(sqlcmd);
    PQclear(res);

    return ao;
}