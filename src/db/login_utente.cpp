#include "database.h"

Utente Database::login_utente(Utente ut){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(ut.getUsername()) == 0 ||  strlen(ut.getPassword()) == 0){
        std::sprintf(errMsg, "Uno dei parametri di utente è null");
        throw NullParameterException(errMsg);
        return ut;
    }

    char* passHashed = this->computeHash(ut.getPassword());


    sprintf(sqlcmd, "SELECT ID FROM utente WHERE username = '%s' AND pass = '%s'", ut.getUsername(), passHashed);
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) == 0) {
        std::sprintf(errMsg, "Login utente fallito");
        throw RecordNotFoundException(errMsg);
        return ut;
    } else{
        int id = atoi(PQgetvalue(res, 0, 0));
        ut.setId(id);
    }
    return ut;
}