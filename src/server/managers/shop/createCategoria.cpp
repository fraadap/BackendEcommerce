#include "shopManager.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include <ostream>
#include "../../utils.h"

#define SHOPWRITE "shopWrite"

void ShopManager::createCategoria(redisContext *redis, int clientSocket, const char *nome, const char *padre){
    Categoria c(nome, "");
    Categoria c1(padre, padre);
    try{
        this->db->create_categoria(c);
        this->db->create_categoria(c1);

    }catch(const std::exception& e){  
         // Crea una stringa di risposta che include l'ID e l'username
        std::string responseBody = e.what();
        std::string responseHeader = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: " + intToString(responseBody.length()) + "\r\n\r\n";
        std::string response = responseHeader + responseBody;

        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", SHOPWRITE, "*", "idSocket", clientSocket, "response", response.c_str());
        if (reply == NULL) {
            std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
            redisFree(redis);
        }
        freeReplyObject(reply);
        return;
    }

    // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "Nome: " + std::string(nome) + "\nNome padre: " + padre;
    std::string responseHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + intToString(responseBody.length()) + "\r\n\r\n";
    std::string response = responseHeader + responseBody;

    redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", SHOPWRITE, "*", "idSocket", clientSocket, "response", response.c_str());
    if (reply == NULL) {
        std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
        redisFree(redis);
    }
    freeReplyObject(reply);

    return;

}