#include "shopManager.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include <ostream>
#include "../../utils.h"

#define SHOPWRITE "shopWrite"

void ShopManager::createNegozio(redisContext *redis, int clientSocket, const char *nazione, const char *citta, const char *capp, const char *via, const char *civico, int interno, const char *numero, const char *titolare, const char *scadenza, const char *nome, const char *telefono, const char *mail, const char *password){ // testato
    Nazione n(nazione);
    Citta c(0, citta, n);
    Cap cap(capp, c);
    Indirizzo in(0,via,civico, interno, cap);
    Metodo_pagamento mp(0, "numero", titolare, scadenza);
    Negozio ne(0,nome,telefono,mail, in, mp, password);

    try{
        ne = this->db->create_negozio(ne);
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

    int id = ne.getId();

    // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "ID: " + intToString(id) + "\nNazione: " + nazione + "\nCitta: " + citta + "\nCAP: " + capp + "\nVia: " + via + "\nCivico: " + civico + "\nInterno: " + intToString(interno) + "\nNumero di telefono: " + numero + "\nTitolare: " + titolare + "\nScadenza: " + scadenza + "\nNome: " + nome + "\nTelefono: " + telefono + "\nMail: " + mail;
    std::string responseHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + intToString(responseBody.length()) + "\r\n\r\n";
    std::string response = responseHeader + responseBody;

    redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s", SHOPWRITE, "idSocket", clientSocket, "response", response.c_str());
    if (reply == NULL) {
        std::cout << "Errore durante la lettura dei messaggi dal feed di stream" << std::endl;
        redisFree(redis);
    }
    freeReplyObject(reply);

    return;
}