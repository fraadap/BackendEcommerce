#include "shopManager.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include <ostream>
#include "../../utils.h"

#define SHOPWRITE "shopWrite"

void ShopManager::createArticolo(redisContext *redis, int clientSocket, int idNegozio, const char *nome, const char *modello, const char *marca, const char *descrizione, int disponibilita, double prezzo, const char *pubblicazione, bool offerta, const char *inizio_offerta, double percentuale, int durata_ore, const char *categoria) { 
    Negozio n(idNegozio);
    Categoria c(categoria, "");
    Articolo a(0, nome, modello, marca, descrizione,
    disponibilita, prezzo, "", offerta,
    "", percentuale, durata_ore, c, n);
    try{
        a = this->db->create_articolo(a);
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

    int id = a.getId();

    // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "ID: " + intToString(id) + "\nNome: " + nome + "\nModello: " + modello + "\nMarca: " + marca + "\nDescrizione: " + descrizione + "\ndisponibilità: " + intToString(disponibilita) + "\nPrezzo: " + doubleToString(prezzo) + "\nPubblicazione: " + pubblicazione + "\nOfferta: " + boolToString(offerta) + "\ninizio_offerta: " + inizio_offerta + "\nPercentuale: " + doubleToString(percentuale) + "\ndurata_ore: " + intToString(durata_ore) + "\nCategoria: " + categoria;
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