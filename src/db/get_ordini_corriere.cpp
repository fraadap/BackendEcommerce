#include "database.h"
std::vector<Ordine_corriere> Database::get_ordini_corriere(Corriere co){

 PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    std::vector<Ordine_corriere> ordini;
    if (co.getId() == 0 ) {
        std::sprintf(errMsg, "L'id del corriere è null");
        throw NullParameterException(errMsg);
    }    
    

    sprintf(sqlcmd, "SELECT ID FROM corriere WHERE id = %d", co.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        std::sprintf(errMsg, "Corriere specificato non trovato");
        throw RecordNotFoundException(errMsg);
    }

    sprintf(sqlcmd, 
    "SELECT ordine_corriere.id, ordine_corriere.inizio, ordine_corriere.fine, "
    "ordine.id, ordine.OrderStatus, ordine.istante, "
    "utente.id, utente.username, utente.nome, utente.cognome , indirizzo.id, indirizzo.via, indirizzo.civico, indirizzo.interno, cap.codice, citta.id, citta.nome, nazione.nome "
    " FROM Ordine JOIN Utente ON ordine.utente = utente.id JOIN Indirizzo ON utente.indirizzo = indirizzo.id "
    " JOIN CAP ON indirizzo.zona = CAP.codice JOIN Citta ON CAP.citta = citta.id JOIN Nazione ON citta.nazione = nazione.nome JOIN Ordine_corriere ON Ordine_corriere.ordine = ordine.id"
    " WHERE ordine_corriere.corriere = %d",
    co.getId());

    res = conn->ExecSQLtuples(sqlcmd);

    int numRows = PQntuples(res);
    for (int i = 0; i < numRows; ++i) {
        int ordine_corriere_id = atoi(PQgetvalue(res, i, 2));
        const char* ordine_corriere_inizio = PQgetvalue(res, i, 2);
        const char* ordine_corriere_fine = PQgetvalue(res, i, 2);
        int idOrdine = atoi(PQgetvalue(res, i, 1));
        const char* stato = PQgetvalue(res, i, 2);
        const char* istante = PQgetvalue(res, i, 3);
        int idUtente = atoi(PQgetvalue(res, i, 4));
        const char* username = PQgetvalue(res, i, 5);
        const char* nome = PQgetvalue(res, i, 5);
        const char* cognome = PQgetvalue(res, i, 5);
        int indirizzo_id = atoi(PQgetvalue(res, i, 1));
        const char* via = PQgetvalue(res, i, 5);
        const char* civico = PQgetvalue(res, i, 5);
        int interno = atoi(PQgetvalue(res, i, 5));
        const char* cap = PQgetvalue(res, i, 5);
        int citta_id = atoi(PQgetvalue(res, i, 5));
        const char* citta = PQgetvalue(res, i, 5);
        const char* nazione = PQgetvalue(res, i, 5);
        
        Cap zona(cap, Citta(citta_id, citta, Nazione(nazione)));
        Indirizzo in(indirizzo_id, via, civico, interno, zona);

        Utente ut(idUtente, username, nome, cognome, "", "", in, "");
        Ordine ord(idOrdine, istante, stato, ut, Metodo_pagamento());
        Ordine_corriere ord_cor(ordine_corriere_id, ord, co.getId(), ordine_corriere_inizio, ordine_corriere_fine);

        ordini.push_back(ord_cor);
    }
    PQclear(res);

    return ordini;
}
