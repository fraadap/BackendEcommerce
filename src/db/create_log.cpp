#include "database.h"

Log Database::create_log(Log log){ // si deve pensare alla password
    PGresult *res;
    char sqlcmd[1000];

        sprintf(sqlcmd, "INSERT INTO LogTable VALUES (DEFAULT, CURRENT_TIMESTAMP, '%s', '%s') RETURNING ID", log.getTag(), log.getInfo());
        res = conn->ExecSQLtuples(sqlcmd);
        
        int id = atoi(PQgetvalue(res, 0, 0));
        log.setId(id);

        PQclear(res);

    return log;
}