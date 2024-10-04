#include "database.h"

RequestLog Database::update_request_log(RequestLog log){
    PGresult *res;
    char sqlcmd[1000];

        sprintf(sqlcmd, "UPDATE RequestLogTable SET fine = CURRENT_TIMESTAMP WHERE id = (SELECT id FROM RequestLogTable WHERE fine IS NULL  AND fd = %d ORDER BY inizio ASC LIMIT 1) RETURNING ID"
        , log.getFd());
        res = conn->ExecSQLtuples(sqlcmd);
        if (PQntuples(res) > 0){
            int id = atoi(PQgetvalue(res, 0, 0));
            log.setId(id);
        } 


        PQclear(res);

    return log;
}