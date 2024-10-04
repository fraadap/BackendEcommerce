#include "userManager.h"
#include <hiredis/hiredis.h>
#include "../../utils.h"

#define USERWRITE "userWrite"

void UserManager::createCarrello(redisContext *redis, int clientSocket, int idUtente, int itemId, int quantita) {
    Articolo a(itemId);
    Utente ut(idUtente);
    Carrello c(0, ut, a, "", quantita);
    try{
        c = this->db->create_carrello(c);
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

    int id = c.getId();

    // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "Articolo: " + intToString(itemId) + "\naggiunto al carrello: " + intToString(id) + "\ndell'Utente: " + intToString(idUtente) + "\nNumero pezzi: " + intToString(quantita);
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