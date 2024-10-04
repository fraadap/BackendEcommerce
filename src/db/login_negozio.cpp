#include "database.h"

Negozio Database::login_negozio(Negozio ne){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(ne.getNome()) == 0 ||  strlen(ne.getPassword()) == 0){
        std::sprintf(errMsg, "Uno dei parametri del negozio è null");
        throw NullParameterException(errMsg);
        return ne;
    }

    char* passHashed = this->computeHash(ne.getPassword());


    sprintf(sqlcmd, "SELECT ID FROM negozio WHERE nome = '%s' AND pass = '%s'", ne.getNome(), passHashed);
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) == 0) {
        std::sprintf(errMsg, "Login negozio fallito");
        throw RecordNotFoundException(errMsg);
        return ne;
    } else{
        int id = atoi(PQgetvalue(res, 0, 0));
        ne.setId(id);
    }
    return ne;
}