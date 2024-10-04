#include <hiredis/hiredis.h>
#include "../../../db/database.h"
#include "../../../db/con2db/pgsql.h"
#include "../../con2redis/src/con2redis.h"
#include "../../../models/header.h"



#ifndef ADMIN_MANAGER
#define ADMIN_MANAGER


class AdminManager {

public:
    AdminManager() {}

    void manage();

    void loginAdmin(redisContext *redis, int clientSocket, const char *nome, const char *password);

private:

    Database *db;

};

#endif 
