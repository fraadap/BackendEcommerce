#include "userManager.h"
#include <hiredis/hiredis.h>
#include "../../utils.h"

#define USERWRITE "userWrite"

void UserManager::getCarrelli(redisContext *redis, int clientSocket, int idCarrello){
    Utente ut(idCarrello);
    std::string articoli = "\nArticoli nel carrello:\n";
    try{
        std::vector<Carrello> carrelli = this->db->get_carrelli(ut);

        for (int x = 0; x < carrelli.size(); ++x) {
            articoli += "\n";
            articoli += "          ID: ";
            articoli += intToString(carrelli[x].getArticolo().getId());
            articoli += "\n";
            articoli += "          categoria: ";
            articoli += carrelli[x].getArticolo().getCategoria().getNome();
            articoli += "\n";
            articoli += "          Marca: ";
            articoli += carrelli[x].getArticolo().getMarca();
            articoli += "\n";
            articoli += "          Modello: ";
            articoli += carrelli[x].getArticolo().getModello();
            articoli += "\n";
            articoli += "          Disponibilità: ";
            articoli += intToString(carrelli[x].getArticolo().getDisponibilita());
            articoli += "\n";
            articoli += "          Descrizione: ";
            articoli += carrelli[x].getArticolo().getDescrizione();
        }
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

    // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "ID carrello: " + intToString(idCarrello) + "\n" + articoli;
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