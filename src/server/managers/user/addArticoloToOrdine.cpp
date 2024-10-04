#include "userManager.h"
#include <hiredis/hiredis.h>
#include "../../utils.h"

#define USERWRITE "userWrite"

void UserManager::addArticoloToOrdine(redisContext *redis, int clientSocket, int orderId, int itemId, int quantita){
    Ordine o(orderId);
    Articolo a(itemId);
    Articolo_ordine ao(0, o, a, quantita);
    try{
        ao = this->db->add_articolo_to_ordine(ao);
    }catch(const std::exception& e){
         // Crea una stringa di risposta che include l'ID e l'username
        std::string responseBody = e.what();
        std::string responseHeader = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: " + intToString(responseBody.length()) + "\r\n\r\n";
        std::string response = responseHeader + responseBody;

        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", USERWRITE, "*", "idSocket", clientSocket, "response", response.c_str());
        if (reply == NULL) {
            std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
            redisFree(redis);
        }
        freeReplyObject(reply);
        return;
    }

    int id = ao.getId();

     // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "ID: " + intToString(id) + "\nArticolo n. " + intToString(itemId) + "\nAggiunto all'ordine n. " + intToString(orderId);
    std::string responseHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + intToString(responseBody.length()) + "\r\n\r\n";
    std::string response = responseHeader + responseBody;

    redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", USERWRITE, "*", "idSocket", clientSocket, "response", response.c_str());
    if (reply == NULL) {
        std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
        redisFree(redis);
    }
    freeReplyObject(reply);

    return;

}