#include "userManager.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include <ostream>
#include "../../utils.h"

#define USERWRITE "userWrite"

void UserManager::getProfile(redisContext *redis, int clientSocket, int idUtente){
    Utente ut(idUtente);
    Profilo p;
    try{
        p = this->db->get_profilo(ut);
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
    std::vector<Metodo_pagamento> metodi = p.getMetodi();
    std::string metodo = "Metodi di pagamento:\n";
    for ( int x = 0; x < metodi.size(); ++x ){
        metodo += "     ";
        metodo += "ID: ";
        metodo += intToString(metodi[x].getId());
        metodo += "\n";
        metodo += "     ";
        metodo += "Numero: ";
        metodo += metodi[x].getNumero();
        metodo += "\n";
        metodo += "     ";
        metodo += "Scadenza: ";
        metodo += metodi[x].getScadenza();
        metodo += "\n";
        metodo += "     ";
        metodo += "Titolare: ";
        metodo += metodi[x].getTitolare();
        metodo += "\n\n";
    }
    std::vector<Carrello> carrelli = p.getCarrelli();
    std::string carrello = "\nCarrelli:\n";
    for ( int x = 0; x < carrelli.size(); ++x ){
        carrello += "     ";
        carrello += "ID: ";
        carrello += intToString(carrelli[x].getId());
        carrello += "\n";
        carrello += "     ";
        carrello += "Articolo: ";
        carrello += "\n";
        carrello += "          ID: ";
        carrello += intToString(carrelli[x].getArticolo().getId());
        carrello += "\n";
        carrello += "          categoria: ";
        carrello += carrelli[x].getArticolo().getCategoria().getNome();
        carrello += "\n";
        carrello += "          Marca: ";
        carrello += carrelli[x].getArticolo().getMarca();
        carrello += "\n";
        carrello += "          Modello: ";
        carrello += carrelli[x].getArticolo().getModello();
        carrello += "\n";
        carrello += "          Disponibilità: ";
        carrello += intToString(carrelli[x].getArticolo().getDisponibilita());
        carrello += "\n";
        carrello += "          Descrizione: ";
        carrello += carrelli[x].getArticolo().getDescrizione();
        carrello += "\n";
        carrello += "     ";
        carrello += "Quantita: ";
        carrello += intToString(carrelli[x].getQuantita());
        carrello += "\n";
        carrello += "     ";
        carrello += "Aggiunta: ";
        carrello += carrelli[x].getAggiunta();
        carrello += "\n\n";
    }
    std::vector<Lista_desideri> liste = p.getListeDesideri();
    std::string lista = "\nLista dei preferiti:\n";
    for ( int x = 0; x < liste.size(); ++x){
        lista += "     ";
        lista += "ID: ";
        lista += intToString(liste[x].getId());
        lista += "\n";
        lista += "     ";
        lista += "Utente: ";
        lista += "\n";
        lista += "          ID: ";
        lista += intToString(liste[x].getUtente().getId());
        lista += "\n";
        lista += "     ";
        lista += "Articolo: ";
        lista += "\n";
        lista += "          ID: ";
        lista += intToString(liste[x].getArticolo().getId());
        lista += "\n";
        lista += "          categoria: ";
        lista += liste[x].getArticolo().getCategoria().getNome();
        lista += "\n";
        lista += "          Marca: ";
        lista += liste[x].getArticolo().getMarca();
        lista += "\n";
        lista += "          Modello: ";
        lista += liste[x].getArticolo().getModello();
        lista += "\n";
        lista += "          Disponibilità: ";
        lista += intToString(liste[x].getArticolo().getDisponibilita());
        lista += "\n";
        lista += "          Descrizione: ";
        lista += liste[x].getArticolo().getDescrizione();
        lista += "\n";
        lista += "     ";
        lista += "DateTime: ";
        lista += liste[x].getDatetime();
        lista += "\n\n";
    }
     // Crea una stringa di risposta che include l'ID e l'username
    std::string responseBody = metodo + carrello + lista;
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