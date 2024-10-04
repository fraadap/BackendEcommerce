#include <hiredis/hiredis.h>
#include "../../con2redis/src/con2redis.h"
#include "../../../db/con2db/pgsql.h"
#include "../../../db/database.h"
#include "../../../models/header.h"

#ifndef USER_MANAGER
#define USER_MANAGER


class UserManager {



public:
  UserManager(){}

  void manage();

  void loginUtente(
    redisContext *redis, 
    int clientSocket, 
    const char *username, 
    const char *password);

  void createUtente(
    redisContext *redis, 
    int clientSocket, 
    const char *nazione, 
    const char *citta, 
    const char *cap,
    const char *via, 
    const char *civico, 
    int interno,
    const char *username, 
    const char *nome,
    const char *cognome, 
    const char *mail,
    const char *password);

    void addPayment(
      redisContext *redis, 
      int clientSocket, 
      int idUtente, 
      const char *numero, 
      const char *cvv, 
      const char *titolare, 
      const char *scadenza);

    void addReview(
      redisContext *redis, 
      int clientSocket, 
      int idUtente, 
      int itemId, 
      int valutazione, 
      const char *descrizione);

    void deleteRecensione(
      redisContext *redis, 
      int clientSocket, 
      int reviewId);

    void getHomePage(
      redisContext *redis, 
      int clientSocket);

    void addMetodoToUtente(
      redisContext *redis, 
      int clientSocket, 
      int idUtente, 
      int paymentid);

    void removeMetodoToUtente(
      redisContext *redis, 
      int clientSocket, 
      int idUtente, 
      int paymentid);

    void createCarrello(
      redisContext *redis, 
      int clientSocket, 
      int idUtente, 
      int itemId, 
      int quantita);

    void deleteCarrello(
      redisContext *redis, 
      int clientSocket, 
      int cartId);

    void addFavorite(
      redisContext *redis, 
      int clientSocket, 
      int idUtente, 
      int itemId);

    void deleteFavorite(
      redisContext *redis, 
      int clientSocket, 
      int favoriteId);

    void getProfile(
      redisContext *redis, 
      int clientSocket, 
      int idUtente);

    void getListaDesideri(
      redisContext *redis, 
      int clientSocket, 
      int idUtente);

    void createOrdine(
      redisContext *redis, 
      int clientSocket, 
      int idUtente, 
      int paymentId, 
      const char *stato);

    void addArticoloToOrdine(
      redisContext *redis, 
      int clientSocket, 
      int orderId, 
      int itemId, 
      int quantita);

    void getCarrelli(
      redisContext *redis, 
      int clientSocket, 
      int idCarrello);

    void getOrdine(
      redisContext *redis, 
      int clientSocket, 
      int idCarrello);

    void deleteOrdine(
      redisContext *redis, 
      int clientSocket, 
      int orderId);

private:
    Database *db;
};




#endif
