#include "userManager.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include <ostream>
#include "../../utils.h"

#define USERWRITE "userWrite"

void UserManager::createUtente(redisContext *redis, int clientSocket, const char *nazione, const char *citta, const char *capp, const char *via, const char *civico, int interno, const char *username, const char *nome, const char *cognome, const char *mail, const char *password){ // testato
    Nazione n(nazione);
    Citta c(0, citta, n);
    Cap cap(capp, c);
    Indirizzo in(0,via,civico, interno, cap);
    Utente ut(0,username, nome, cognome, "", mail, in, password);
    try{
        ut = this->db->create_utente(ut);
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

    int id = ut.getId();

    // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = "ID: " + intToString(id) + "\nNazione: " + nazione + "\nCitta: " + citta + "\nCAP: " + capp + "\nVia: " + via + "\nCivico: " + civico + "\nInterno: " + intToString(interno) + "\nUsername: " + username + "\nNome: " + nome + "\nCognome: " + cognome + "\nMail: " + mail;
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
