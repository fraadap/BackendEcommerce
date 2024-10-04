#include "deliverManager.h"
#include <hiredis/hiredis.h>
#include "../../utils.h"

#define DELIVERYWRITE "deliveryWrite"

void DeliverManager::addOrdineToCorriere(redisContext *redis, int clientSocket, int idCorriere, int idOrdine){
    Corriere c(idCorriere);
    Ordine ord(idOrdine);
    Ordine_corriere oc(0,ord, c, "", "");
    try{
        oc = this->db->add_ordine_to_corriere(oc);
    } catch(const std::exception& e){  
         // Crea una stringa di risposta che include l'ID e l'username
        std::string responseBody = e.what();
        std::string responseHeader = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: " + intToString(responseBody.length()) + "\r\n\r\n";
        std::string response = responseHeader + responseBody;

        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", DELIVERYWRITE, "*", "idSocket", clientSocket, "response", response.c_str());
        if (reply == NULL) {
            std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
            redisFree(redis);
        }
        freeReplyObject(reply);
        return;
    }
    int id = oc.getId();

    // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "ID: " + intToString(id) + "\nOrdine numero: " + intToString(idOrdine) + " aggiunto al corriere numero: " + intToString(idCorriere);
    std::string responseHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + intToString(responseBody.length()) + "\r\n\r\n";
    std::string response = responseHeader + responseBody;

    redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s %s %s %d %s %s", DELIVERYWRITE, "*", "idSocket", clientSocket, "response", response.c_str());
    if (reply == NULL) {
        std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
        redisFree(redis);
    }
    freeReplyObject(reply);

    return;

}