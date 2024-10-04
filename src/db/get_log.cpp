#include "database.h"
#include <cstring>

std::vector<Log> Database::get_log(const char* inizioTime, const char * fineTime, const char * tag){ // si deve pensare alla password
    PGresult *res;
    char sqlcmd[1000];
    std::vector<Log> logs;
    if(strlen(inizioTime)!=0 && strlen(fineTime)!=0){
        sprintf(sqlcmd, "SELECT * FROM LogTable WHERE timestamp < '%s' AND timestamp > '%s' AND tag LIKE '%s' ", fineTime, inizioTime, tag);
        res = conn->ExecSQLtuples(sqlcmd);
    }
    else {
        sprintf(sqlcmd, "SELECT * FROM LogTable WHERE tag LIKE '%s'", tag);
        res = conn->ExecSQLtuples(sqlcmd);
    }
    int numRows = PQntuples(res);
    for (int i = 0; i < numRows; ++i) {
        int idLog = atoi(PQgetvalue(res, i, 0));
        const char* timestamp = PQgetvalue(res, i, 1);
        const char* tag = PQgetvalue(res, i, 2);
        const char* info = PQgetvalue(res, i, 3);

        Log log(idLog, timestamp, tag, info);
        logs.push_back(log);
        
    }
    PQclear(res);
    return logs;
}