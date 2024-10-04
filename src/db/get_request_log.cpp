#include "database.h"
#include <cstring>

std::vector<RequestLog> Database::get_request_log(const char* inizioTime, const char * fineTime){ // si deve pensare alla password
    PGresult *res;
    char sqlcmd[1000];
    std::vector<RequestLog> logs;
    if(strlen(inizioTime)!=0 && strlen(fineTime)!=0){
        sprintf(sqlcmd, "SELECT id, fd, inizio, fine FROM RequestLogTable WHERE inizio < '%s' AND inizio > '%s' AND fine IS NOT NULL", fineTime, inizioTime);
        res = conn->ExecSQLtuples(sqlcmd);
    }
    else {
        sprintf(sqlcmd, "SELECT * FROM RequestLogTable WHERE fine IS NOT NULL");
        res = conn->ExecSQLtuples(sqlcmd);
    }
    int numRows = PQntuples(res);
    for (int i = 0; i < numRows; ++i) {
        int idLog = atoi(PQgetvalue(res, i, 0));
        int fd = atoi(PQgetvalue(res, i, 1));
        const char* inizio = PQgetvalue(res, i, 2);
        const char* fine = PQgetvalue(res, i, 3);
        printf("%s\n", inizio);
        RequestLog log(idLog, fd, inizio, fine);
        logs.push_back(log);
        
    }
    PQclear(res);
    return logs;
}