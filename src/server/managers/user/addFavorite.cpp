#include "userManager.h"
#include <hiredis/hiredis.h>
#include "../../utils.h"

#define USERWRITE "userWrite"

void UserManager::addFavorite(redisContext *redis, int clientSocket, int idUtente, int itemId){
    Articolo a(itemId);
    Utente u(idUtente);
    Lista_desideri ld(0, u, a, "");
    try {
        ld = this->db->create_lista_desideri(ld);
    } catch (const std::exception& e) {
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

    int id = ld.getId();

     // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "ID: " + intToString(id) + "\nL'articolo: " + intToString(itemId) + "\nè stato aggiunto tra i preferiti di : " + intToString(idUtente);
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