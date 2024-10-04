#include <hiredis/hiredis.h>
#include "../../../db/database.h"
#include "../../../db/con2db/pgsql.h"
#include "../../con2redis/src/con2redis.h"
#include "../../../models/header.h"

#ifndef SHOP_MANAGER
#define SHOP_MANAGER



class ShopManager {
public:
    ShopManager() {}

    void manage();

    void createNegozio(
        redisContext *redis,
        int clientSocket,
        const char *nazione, 
        const char *citta, 
        const char *capp, 
        const char *via, 
        const char *civico, 
        int interno, 
        const char *numero, 
        const char *titolare, 
        const char *scadenza, 
        const char *nome, 
        const char *telefono, 
        const char *mail, 
        const char *password);

    void loginNegozio(
        redisContext *redis, 
        int clientSocket, 
        const char *nome, 
        const char *password);

    void createArticolo(
        redisContext *redis, 
        int clientSocket, 
        int idNegozio, 
        const char* nome, 
        const char* modello, 
        const char* marca, 
        const char* descrizione, 
        int disponibilita, 
        double prezzo, 
        const char* pubblicazione, 
        bool offerta, 
        const char* inizio_offerta, 
        double percentuale, 
        int durata_ore, 
        const char* categoria); //da rifinire
        
    void createCategoria(
        redisContext *redis, 
        int clientSocket, 
        const char *nome, 
        const char *padre);

    void getOrdiniNegozio(
        redisContext *redis, 
        int clientSocket, 
        int idNegozio);

    void getordineNegozio(
        redisContext *redis, 
        int clientSocket, 
        int idNegozio);

private:
    Database *db;
};

#endif 
