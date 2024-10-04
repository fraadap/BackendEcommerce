#include "database.h"

RequestLog Database::create_request_log(RequestLog log){ // si deve pensare alla password
    PGresult *res;
    char sqlcmd[1000];
    sprintf(sqlcmd, "INSERT INTO RequestLogTable VALUES (DEFAULT, %d, CURRENT_TIMESTAMP, NULL) ON CONFLICT DO NOTHING RETURNING ID", log.getFd());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) > 0){
        int id = atoi(PQgetvalue(res, 0, 0));
        log.setId(id);
    }
    PQclear(res);

    return log;
}