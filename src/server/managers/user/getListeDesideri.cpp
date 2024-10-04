#include "userManager.h"
#include <hiredis/hiredis.h>
#include "../../utils.h"

#define USERWRITE "userWrite"

void UserManager::getListaDesideri(redisContext *redis, int clientSocket, int idUtente){
    Utente ut(idUtente);
    std::vector<Lista_desideri> liste;
    try{
        
        liste = this->db->get_lista_desideri(ut);

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

    std::string lista = "\nLista dei preferiti:\n";
    for ( int x = 0; x < liste.size(); ++x){
        lista += "     ";
        lista += "ID: ";
        lista += intToString(liste[x].getId());
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
    std::string responseBody = lista;
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