#include "database.h"

Admin Database::login_admin(Admin ad){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(ad.getNome()) == 0 ||  strlen(ad.getPassword()) == 0){
        std::sprintf(errMsg, "Uno dei parametri di admin è null");
        throw NullParameterException(errMsg);
        return ad;
    }

    sprintf(sqlcmd, "SELECT ID FROM admin WHERE nome = '%s' AND pass = '%s'", ad.getNome(), ad.getPassword());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) == 0) {
        std::sprintf(errMsg, "Login admin fallito");
        throw RecordNotFoundException(errMsg);
        return ad;
    } else{
        int id = atoi(PQgetvalue(res, 0, 0));
        ad.setId(id);
    }
    return ad;
}