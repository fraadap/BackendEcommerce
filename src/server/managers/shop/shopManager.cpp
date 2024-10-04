#include "shopManager.h"
#include "../../con2redis/src/con2redis.h"
#include <iostream>
#include <ostream>

#define SHOPREAD "shopRead"
#define SHOPWRITE "shopWrite"

void ShopManager::manage() {

    Con2DB con("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    Database db(&con);

    this->db = &db;

    redisReply *reply;
    // Connessione a Redis
    redisContext *redis = redisConnect("localhost", 6379);
    if (redis == NULL || redis->err) {
        if (redis) {
            db.create_log(Log(0, "", "Manager_error", "Errore nella connessione a Redis nel manager shop"));
            std::cout << "Errore nella connessione a Redis: " << redis->errstr << std::endl;
            redisFree(redis);
        } else {
            db.create_log(Log(0, "", "Manager_error", "impossibile allocare la struttura redisContext nel manager shop"));
            std::cout << "Errore: impossibile allocare la struttura redisContext" << std::endl;
        }
        return;
    }


    while (true) {
        std::string richiesta = "XREADGROUP GROUP diameter username BLOCK 1 COUNT 1 NOACK STREAMS " + std::string(SHOPREAD) + " >";
        reply = (redisReply*)redisCommand(redis, richiesta.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            assertReply(redis, reply);
            int clientSocket = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
            std::string operationId = reply->element[0]->element[1]->element[0]->element[1]->element[3]->str;
            if ( operationId == "doRegisterShop"){

                std::string nazione = reply->element[0]->element[1]->element[0]->element[1]->element[5]->str;
                std::string citta = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                std::string cap = reply->element[0]->element[1]->element[0]->element[1]->element[9]->str;
                std::string via = reply->element[0]->element[1]->element[0]->element[1]->element[11]->str;
                std::string civico = reply->element[0]->element[1]->element[0]->element[1]->element[13]->str;
                int interno = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[15]->str);
                std::string numero = reply->element[0]->element[1]->element[0]->element[1]->element[17]->str;
                std::string titolare = reply->element[0]->element[1]->element[0]->element[1]->element[19]->str;
                std::string scadenza = reply->element[0]->element[1]->element[0]->element[1]->element[21]->str;
                std::string nome = reply->element[0]->element[1]->element[0]->element[1]->element[23]->str;
                std::string telefono = reply->element[0]->element[1]->element[0]->element[1]->element[25]->str;
                std::string mail = reply->element[0]->element[1]->element[0]->element[1]->element[27]->str;
                std::string password = reply->element[0]->element[1]->element[0]->element[1]->element[29]->str;
                freeReplyObject(reply); 

                createNegozio(redis, clientSocket, nazione.c_str(), citta.c_str(), cap.c_str(), via.c_str(), civico.c_str(), interno, numero.c_str(), titolare.c_str(), scadenza.c_str(), nome.c_str(), telefono.c_str(), mail.c_str(), password.c_str());
           
            }
            else if ( operationId == "doLoginShop"){
                std::string nome = reply->element[0]->element[1]->element[0]->element[1]->element[5]->str;
                std::string password = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                freeReplyObject(reply);

                loginNegozio(redis, clientSocket, nome.c_str(), password.c_str());
            }
            else if ( operationId == "addItem" ){
                bool offerta;
                int idNegozio = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                std::string nome = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                std::string modello = reply->element[0]->element[1]->element[0]->element[1]->element[9]->str;
                std::string marca = reply->element[0]->element[1]->element[0]->element[1]->element[11]->str;
                std::string descrizione = reply->element[0]->element[1]->element[0]->element[1]->element[13]->str;
                int disponibilita = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[15]->str);
                double prezzo = std::stod(reply->element[0]->element[1]->element[0]->element[1]->element[17]->str);
                std::string pubblicazione = reply->element[0]->element[1]->element[0]->element[1]->element[19]->str;
                std::string appoggio = reply->element[0]->element[1]->element[0]->element[1]->element[21]->str;
                if ( appoggio == "true" ){
                    offerta = true;
                }
                else{
                    offerta = false;
                }
                std::string inizio_offerta = reply->element[0]->element[1]->element[0]->element[1]->element[23]->str;
                double percentuale = std::stod(reply->element[0]->element[1]->element[0]->element[1]->element[25]->str);
                int durata_ore = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[27]->str);
                std::string categoria = reply->element[0]->element[1]->element[0]->element[1]->element[29]->str;

                createArticolo(redis, clientSocket, idNegozio, nome.c_str(), modello.c_str(), marca.c_str(), descrizione.c_str(), disponibilita, prezzo, pubblicazione.c_str(), offerta, inizio_offerta.c_str(), percentuale, durata_ore, categoria.c_str());

            }
            else if ( operationId == "newCategory"){
                std::string nome = reply->element[0]->element[1]->element[0]->element[1]->element[5]->str;
                std::string padre = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                freeReplyObject(reply);

                createCategoria(redis, clientSocket, nome.c_str(), padre.c_str());
            }
            else if ( operationId == "getOrdineShop"){
                int idShop = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                freeReplyObject(reply);

                getordineNegozio(redis, clientSocket, idShop);

            }
            else{
                freeReplyObject(reply); 
                reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", SHOPWRITE, "*", "idSocket", clientSocket, "errore", "errore");
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