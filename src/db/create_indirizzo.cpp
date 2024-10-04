#include "database.h"

Indirizzo Database::create_indirizzo(Indirizzo in){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(in.getVia()) == 0 || strlen(in.getCivico()) == 0 || strlen(in.getZona().getCodice()) == 0){
        std::sprintf(errMsg, "Uno dei parametri di indirizzo è null: %s %s %s", in.getVia(), in.getVia(), in.getZona().getCodice());
        throw NullParameterException(errMsg);
        return in;
    }    
    
    if (strlen(in.getVia())>30 || strlen(in.getCivico())>10 || strlen(in.getZona().getCodice())>5){
        std::sprintf(errMsg, "Uno dei parametri di indirizzo è troppo lungo: %s %s %s", in.getVia(), in.getCivico(), in.getZona().getCodice());
        throw InvalidParameterException(errMsg);
        return in;
    }

    // Check per la chiave
    sprintf(sqlcmd, "SELECT ID FROM indirizzo WHERE via = '%s' AND civico = '%s' AND interno = '%d' AND zona = '%s'", in.getVia(), in.getCivico(), in.getInterno(), in.getZona().getCodice());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        int id = atoi(PQgetvalue(res, 0, 0));
        in.setId(id);
    } else {
        try{
            in.setZona(this->create_cap(in.getZona()));
        }catch(const std::exception& e){
            throw;
            return in;
        }
        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO  indirizzo VALUES (DEFAULT, '%s', '%s', '%d', '%s') RETURNING ID", in.getVia(), in.getCivico(), in.getInterno(), in.getZona().getCodice());
        res = conn->ExecSQLtuples(sqlcmd);
        
        int id = atoi(PQgetvalue(res, 0, 0));
        in.setId(id);

        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
    }
    

    return in;
}