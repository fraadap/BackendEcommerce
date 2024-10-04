#include "userManager.h"
#include <iostream>
#include <ostream>

#define USERREAD "userRead"
#define USERWRITE "userWrite"

void UserManager::manage() {

    Con2DB con("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    Database db(&con);

    this->db = &db;

    redisReply *reply;
    // Connessione a Redis
    redisContext *redis = redisConnect("localhost", 6379);
    if (redis == NULL || redis->err) {
        if (redis) {
            db.create_log(Log(0, "", "Manager_error", "Errore nella connessione a Redis nel manager user"));
            std::cout << "Errore nella connessione a Redis: " << redis->errstr << std::endl;
            redisFree(redis);
            return;
        } else {
            db.create_log(Log(0, "", "Manager_error", "impossibile allocare la struttura redisContext nel manager user"));
            std::cout << "Errore: impossibile allocare la struttura redisContext" << std::endl;
        }
        return;
    }


    while (true) {
        std::string richiesta = "XREADGROUP GROUP diameter username BLOCK 1 COUNT 1 NOACK STREAMS " + std::string(USERREAD) + " >";
        reply = (redisReply*)redisCommand(redis, richiesta.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            assertReply(redis, reply);
            int clientSocket = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
            std::string operationId = reply->element[0]->element[1]->element[0]->element[1]->element[3]->str;
            
            if ( operationId == "doLoginUser"){
                std::string username = reply->element[0]->element[1]->element[0]->element[1]->element[5]->str;
                std::string password = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                freeReplyObject(reply); 

                loginUtente(redis, clientSocket, username.c_str(), password.c_str());

            }
            else if ( operationId == "doRegisterUser" ){
                std::string nazione = reply->element[0]->element[1]->element[0]->element[1]->element[5]->str;
                std::string citta = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                std::string cap = reply->element[0]->element[1]->element[0]->element[1]->element[9]->str;
                std::string via = reply->element[0]->element[1]->element[0]->element[1]->element[11]->str;
                std::string civico = reply->element[0]->element[1]->element[0]->element[1]->element[13]->str;
                int interno = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[15]->str);
                std::string username = reply->element[0]->element[1]->element[0]->element[1]->element[17]->str;
                std::string nome = reply->element[0]->element[1]->element[0]->element[1]->element[19]->str;
                std::string cognome = reply->element[0]->element[1]->element[0]->element[1]->element[21]->str;
                std::string mail = reply->element[0]->element[1]->element[0]->element[1]->element[23]->str;
                std::string password = reply->element[0]->element[1]->element[0]->element[1]->element[25]->str;
                freeReplyObject(reply); 

                createUtente(redis, clientSocket, nazione.c_str(), citta.c_str(), cap.c_str(), via.c_str(), civico.c_str(), interno, username.c_str(), nome.c_str(), cognome.c_str(), mail.c_str(), password.c_str());

            }
            else if ( operationId == "addPayment" ){
                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                std::string numero = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                std::string cvv = reply->element[0]->element[1]->element[0]->element[1]->element[9]->str;
                std::string titolare = reply->element[0]->element[1]->element[0]->element[1]->element[11]->str;
                std::string scadenza = reply->element[0]->element[1]->element[0]->element[1]->element[13]->str;

                addPayment(redis, clientSocket, idUtente, numero.c_str(), cvv.c_str(), titolare.c_str(), scadenza.c_str());

            }
            else if ( operationId == "reviewItem" ){
                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                int itemId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[7]->str);
                int valutazione = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[9]->str);
                std::string descrizione = reply->element[0]->element[1]->element[0]->element[1]->element[11]->str;
                freeReplyObject(reply); 
                addReview(redis, clientSocket, idUtente, itemId, valutazione, descrizione.c_str());
            }
            else if ( operationId == "unReviewItem" ){
                int reviewId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);

                deleteRecensione(redis, clientSocket, reviewId);
            }
            else if ( operationId == "getMyHomePage" ){

                getHomePage(redis, clientSocket);

            }
            else if ( operationId == "addMethodPaymentToUser" ){

                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                int paymentId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[7]->str);

                addMetodoToUtente(redis, clientSocket, idUtente, paymentId);

            }
            else if ( operationId == "removeMethodPaymentToUser" ){

                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                int paymentId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[7]->str);

                removeMetodoToUtente(redis, clientSocket, idUtente, paymentId);

            }
            else if ( operationId == "cartItem" ){

                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                int itemId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[7]->str);
                int quantita = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[9]->str);

                createCarrello(redis, clientSocket, idUtente, itemId, quantita);

            }
            else if ( operationId == "deleteCart" ){

                int cartId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);

                deleteCarrello(redis, clientSocket, cartId);

            }
            else if ( operationId == "favoriteItem" ){

                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                int itemId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[7]->str);

                addFavorite(redis, clientSocket, idUtente, itemId);

            }
            else if ( operationId == "unFavoriteItem" ){

                int favoriteId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);

                deleteFavorite(redis, clientSocket, favoriteId);

            }
            else if ( operationId == "getUserProfile" ){

                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);

                getProfile(redis, clientSocket, idUtente);

            }
            else if ( operationId == "getMyFavoritePage" ){

                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);

                getListaDesideri(redis, clientSocket, idUtente);

            }
            else if ( operationId == "newOrder" ){

                int idUtente = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                std::string stato = reply->element[0]->element[1]->element[0]->element[1]->element[7]->str;
                int idCarta = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[9]->str);

                createOrdine(redis, clientSocket, idUtente, idCarta, stato.c_str());

            }
            else if ( operationId == "addItemOrder"){

                int orderId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
                int itemId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[7]->str);
                int quantita = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[9]->str);

                addArticoloToOrdine(redis, clientSocket, orderId, itemId, quantita);
            }
            else if ( operationId == "getMyCartPage" ){

                int cartId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);

                getCarrelli(redis, clientSocket, cartId);

            }
            else if ( operationId == "deleteOrder" ){

                int orderId = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);

                deleteOrdine(redis, clientSocket, orderId);

            }
            else{
                freeReplyObject(reply); 
                reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", USERWRITE, "*", "idSocket", clientSocket, "operationId", operationId.c_str());
                if (reply == NULL) {
                    std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
                    redisFree(redis);
                }
                freeReplyObject(reply);
            }
            
        }
    }

    // Non dovrebbe mai arrivare a questo punto, ma chiudi comunque la connessione
    redisFree(redis);
}
