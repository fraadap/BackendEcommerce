#include "shopManager.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include <ostream>
#include "../../utils.h"

#define SHOPWRITE "shopWrite"

void ShopManager::getordineNegozio(redisContext *redis, int clientSocket, int idNegozio){
    std::string ordine;
    try{
        Negozio ne(idNegozio);
        std::vector<Articolo_ordine> ordini = this->db->get_ordini_negozio(ne);
        ordine = "Ordini:\n";
        for (int x = 0; x < ordini.size(); ++x) {
            ordine += "Ordine: ";
            ordine += intToString(ordini[x].getOrdine().getId());
            ordine += "\n";
            ordine += "          ID: ";
            ordine += intToString(ordini[x].getArticolo().getId());
            ordine += "\n";
            ordine += "          categoria: ";
            ordine += ordini[x].getArticolo().getCategoria().getNome();
            ordine += "\n";
            ordine += "          Marca: ";
            ordine += ordini[x].getArticolo().getMarca();
            ordine += "\n";
            ordine += "          Modello: ";
            ordine += ordini[x].getArticolo().getModello();
            ordine += "\n";
            ordine += "          Disponibilità: ";
            ordine += intToString(ordini[x].getArticolo().getDisponibilita());
            ordine += "\n";
            ordine += "          Descrizione: ";
            ordine += ordini[x].getArticolo().getDescrizione();
        }
        
    } catch(const std::exception& e){  
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
    std::string responseBody = ordine;
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