#include "deliverManager.h"
#include "../../con2redis/src/con2redis.h"
#include <iostream>

#define DELIVERYREAD "deliveryRead"
#define DELIVERYWRITE "deliveryWrite"

void DeliverManager::manage() {
    Con2DB con("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    Database db(&con);

    this->db = &db;
    redisReply *reply;
    // Connessione a Redis
    redisContext *redis = redisConnect("localhost", 6379);
    if (redis == NULL || redis->err) {
        if (redis) {
            db.create_log(Log(0, "", "Manager_error", "Errore nella connessione a Redis nel manager deliver"));
            std::cout << "Errore nella connessione a Redis: " << redis->errstr << std::endl;
            redisFree(redis);
        } else {
            db.create_log(Log(0, "", "Manager_error", "impossibile allocare la struttura redisContext nel manager deliver"));
            std::cout << "Errore: impossibile allocare la struttura redisContext" << std::endl;
        }
        return;
    }


    while (true) {
        std::string richiesta = "XREADGROUP GROUP diameter username BLOCK 10000000 COUNT 1 NOACK STREAMS " + std::string(DELIVERYREAD) + " >";
        reply = (redisReply*)redisCommand(redis, richiesta.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            assertReply(redis, reply);
            int clientSocket = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
            std::string operationId = reply->element[0]->element[1]->element[0]->element[1]->element[3]->str;
            if ( operationId == "doRegisterCourier"){
                std::string nome = reply->element[0]->element[1]->element[0]->element[1]->element[5]->str;
                std::string cognome = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                freeReplyObject(reply);

                createCorriere(redis, clientSocket, nome.c_str(), cognome.c_str());
           
            }
            else if ( operationId == "addItemOrderCourier" ){
                int idCourier = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                int idOrder = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[7]->str);
                freeReplyObject(reply);

                addOrdineToCorriere(redis, clientSocket, idCourier, idOrder);

            }
            else if ( operationId == "getOrdineCourier"){
                int idCourier = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                freeReplyObject(reply);

                getordineCourier(redis, clientSocket, idCourier);
            }
            else{
                freeReplyObject(reply); 
                reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", DELIVERYWRITE, "*", "idSocket", clientSocket, "errore", "errore");
                if (reply == NULL) {
                    std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
                    redisFree(redis);
                }
                freeReplyObject(reply);
            }
        }
    }

    redisFree(redis);
}