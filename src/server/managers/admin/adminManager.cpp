#include "adminManager.h"
#include "../../con2redis/src/con2redis.h"
#include <iostream>
#include <ostream>

#define ADMINREAD "adminRead"
#define ADMINWRITE "adminWrite"


void AdminManager::manage() {
    Con2DB con("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    Database db(&con);


    this->db = &db;
    
    redisReply *reply;
    // Connessione a Redis
    redisContext *redis = redisConnect("localhost", 6379);
    if (redis == NULL || redis->err) {
        if (redis) {
            db.create_log(Log(0, "", "Manager_error", "Errore nella connessione a Redis nel manager admin"));
            std::cout << "Errore nella connessione a Redis: " << redis->errstr << std::endl;
            redisFree(redis);
        } else {
            db.create_log(Log(0, "", "Manager_error", "impossibile allocare la struttura admin nel manager user"));
            std::cout << "Errore: impossibile allocare la struttura redisContext" << std::endl;
        }
        return;
    }


    while (true) {
        std::string richiesta = "XREADGROUP GROUP diameter username BLOCK 10000000 COUNT 1 NOACK STREAMS " + std::string(ADMINREAD) + " >";
        reply = (redisReply*)redisCommand(redis, richiesta.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            assertReply(redis, reply);
            int clientSocket = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
            std::string operationId = reply->element[0]->element[1]->element[0]->element[1]->element[3]->str;
            if ( operationId == "doLoginAdmin"){
                std::string nome = reply->element[0]->element[1]->element[0]->element[1]->element[5]->str;
                std::string password = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                freeReplyObject(reply); 

                loginAdmin(redis, clientSocket, nome.c_str(), password.c_str());
           
            }
            else{
                freeReplyObject(reply); 
                reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", ADMINWRITE, "*", "idSocket", clientSocket, "errore", "errore");
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