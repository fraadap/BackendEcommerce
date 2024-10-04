
#ifndef DATABASE
#define DATABASE

#include "con2db/pgsql.h"
#include <iostream>
#include "../models/header.h"
#include <cstring>
#include <vector>

class Database {
private:
	Con2DB *conn;
    char* getCurrentTimestamp();
	char* computeHash(const char*);

public:
    Database(Con2DB *con);

	// tutte le funzioni del database
	void get_esempio();
	Utente create_utente(Utente);
	Negozio create_negozio(Negozio);
	Indirizzo create_indirizzo(Indirizzo);
	Cap create_cap(Cap);
	Citta create_citta(Citta);
	Nazione create_nazione(Nazione);
	Metodo_pagamento create_metodo_pagamento(Metodo_pagamento);
	Categoria create_categoria(Categoria);
	Articolo create_articolo(Articolo);
	SpesaSpedizione create_spesa_spedizione(SpesaSpedizione);
	Carrello create_carrello(Carrello);
	Lista_desideri create_lista_desideri(Lista_desideri);
	Recensione create_recensione(Recensione);
	Ordine create_ordine (Ordine);
	Corriere create_corriere (Corriere);
	Utente_metodo add_metodo_pagamento_to_utente(Utente_metodo);
	Ordine_corriere add_ordine_to_corriere(Ordine_corriere);
	Articolo_ordine add_articolo_to_ordine(Articolo_ordine ao);
	Utente login_utente (Utente);
	Negozio login_negozio(Negozio);
	Admin login_admin(Admin);
	void delete_lista_preferiti(Lista_desideri);
	void delete_carrello(Carrello);
	void delete_recensione(Recensione);
	void delete_ordine(Ordine);
	void remove_metodo_pagamento_from_utente(Utente_metodo);
	std::vector<Carrello> get_carrelli(Utente);
	std::vector<Lista_desideri> get_lista_desideri(Utente);
	std::vector<Articolo> get_home_page();
	Profilo get_profilo(Utente);
	std::vector<Metodo_pagamento> get_metodi_pagamento(Utente);
	Log create_log(Log);
	std::vector<Log> get_log(const char* inizioTime, const char * fineTime, const char * tag);
	std::vector<Articolo_ordine> get_ordini_negozio(Negozio);
	std::vector<Ordine_corriere> get_ordini_corriere(Corriere);
	RequestLog create_request_log(RequestLog);
	std::vector<RequestLog> get_request_log(const char* inizioTime, const char * fineTime);
	RequestLog update_request_log(RequestLog log);
};

// eccezione per quando si infrange un vincolo di chiave
class DuplicateKeyException : public std::runtime_error {
public:
    DuplicateKeyException(const std::string& message) : std::runtime_error(message) {}
}; 

class InvalidParameterException : public std::runtime_error {
public:
	InvalidParameterException(const std::string& message) : std::runtime_error(message) {}
};

class NullParameterException : public std::runtime_error {
public:
	NullParameterException(const std::string& message) : std::runtime_error(message) {}
};

class RecordNotFoundException : public std::runtime_error {
public:
	RecordNotFoundException(const std::string& message) : std::runtime_error(message) {}
};

#endif // DATABASE
