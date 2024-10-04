#include <hiredis/hiredis.h>
#include "../../../db/database.h"
#include "../../../db/con2db/pgsql.h"
#include "../../con2redis/src/con2redis.h"
#include "../../../models/header.h"

#ifndef DELIVER_MANAGER
#define DELIVER_MANAGER


class DeliverManager {
public:
    DeliverManager() {}

    void manage();

    void createCorriere(
        redisContext *redis, 
        int clientSocket, 
        const char *nome, 
        const char *cognome);

    void addOrdineToCorriere(
        redisContext *redis, 
        int clientSocket, 
        int idCorriere, 
        int idOrdine);

    void getordineCourier(
        redisContext *redis, 
        int clientSocket, 
        int idCourier);


private:

    Database *db;

};

#endif 
