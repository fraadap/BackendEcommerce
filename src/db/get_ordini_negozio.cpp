#include "database.h"
std::vector<Articolo_ordine> Database::get_ordini_negozio(Negozio ne){

 PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    std::vector<Articolo_ordine> ordini;
    if (ne.getId() == 0 ) {
        std::sprintf(errMsg, "L'id del negozio è null");
        throw NullParameterException(errMsg);
    }    
    

    sprintf(sqlcmd, "SELECT ID FROM negozio WHERE id = %d", ne.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        std::sprintf(errMsg, "Negozio specificato non trovato");
        throw RecordNotFoundException(errMsg);
    }
    
    sprintf(sqlcmd, 
    "SELECT articolo_ordine.quantita, "
    "ordine.id, ordine.OrderStatus, ordine.istante, "
    "utente.id, utente.username, "
    "articolo.id, articolo.nome, articolo.modello, articolo.marca, articolo.disponibilita, articolo.prezzo, articolo.pubblicazione,"
    " articolo.offerta, articolo.inizio_offerta, articolo.percentuale_sconto, articolo.durata_ore, articolo.categoria, articolo.negozio, articolo.descrizione "
    " FROM articolo_ordine JOIN articolo ON articolo_ordine.articolo = articolo.id JOIN  ordine ON articolo_ordine.ordine = ordine.id"
    " JOIN utente ON ordine.utente = utente.id WHERE articolo.negozio = %d",
    ne.getId());

    res = conn->ExecSQLtuples(sqlcmd);

    int numRows = PQntuples(res);
    for (int i = 0; i < numRows; ++i) {
        int quantita = atoi(PQgetvalue(res, i, 0));
        int idOrdine = atoi(PQgetvalue(res, i, 1));
        const char* stato = PQgetvalue(res, i, 2);
        const char* istante = PQgetvalue(res, i, 3);
        int idUtente = atoi(PQgetvalue(res, i, 4));
        const char* username = PQgetvalue(res, i, 5);
        int idArticolo = atoi(PQgetvalue(res, i, 6));
        const char* nomeArticolo = PQgetvalue(res, i, 7);
        const char* modelloArticolo = PQgetvalue(res, i, 8);
        const char* marcaArticolo = PQgetvalue(res, i, 9);
        int disponibilitaArticolo = atoi(PQgetvalue(res, i,10));
        double prezzoArticolo = atof(PQgetvalue(res, i, 11));
        const char* pubblicazioneArticolo = PQgetvalue(res, i, 12);
        bool offertaArticolo = (strcmp(PQgetvalue(res, i, 13), "t") == 0);
        const char* inizioOffertaArticolo = PQgetvalue(res, i, 14);
        double percentualeSconto = atof(PQgetvalue(res, i, 15));
        int durataOfferta = atoi(PQgetvalue(res, i, 16));
        const char* categoriaArticolo = PQgetvalue(res, i, 17);
        int idNegozio = atoi(PQgetvalue(res, i, 18));
        const char* descrizioneArticolo = PQgetvalue(res, i, 19);
        
        Utente ut(idUtente, username);
        Negozio ne(idNegozio);
        Categoria cat(categoriaArticolo);
        Articolo ar(idArticolo, nomeArticolo, modelloArticolo, marcaArticolo, descrizioneArticolo, disponibilitaArticolo, prezzoArticolo, 
        pubblicazioneArticolo, offertaArticolo, inizioOffertaArticolo, percentualeSconto, durataOfferta, cat, ne);
        ;
        Ordine ord(idOrdine, istante, stato, ut, Metodo_pagamento());
        Articolo_ordine ar_or(0, ord, ar, quantita);
        ordini.push_back(ar_or);
    }
    PQclear(res);

    return ordini;
}
