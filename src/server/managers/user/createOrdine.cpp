#include "userManager.h"
#include <hiredis/hiredis.h>
#include "../../utils.h"

#define USERWRITE "userWrite"

void UserManager::createOrdine(redisContext *redis, int clientSocket, int idUtente, int paymentId, const char *stato){
    Utente ut(idUtente);
    Metodo_pagamento mp(paymentId);
    Ordine ord(0, "", stato, ut, mp);
    try{
        ord = this->db->create_ordine(ord);
    } catch(const std::exception& e){  
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

    int id = ord.getId();

    // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "ID Ordine: " + intToString(id) + "\nUtente: " + intToString(idUtente) + "\ncon il metodo di pagamento numero: " + intToString(paymentId);
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