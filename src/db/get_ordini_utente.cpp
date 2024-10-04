
/*
#include "database.h"

std::vector<Articolo_ordine> Database::get_ordini_utente(Utente ut) {
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];
    std::vector<Articolo_ordine> ordini;

    if (ut.getId() == 0) {
        std::sprintf(errMsg, "L'id dell'utente è null");
        throw NullParameterException(errMsg);
    }

    sprintf(sqlcmd, 
        "SELECT ordine.id, ordine.quantita, ordine.istante, ordine.orderStatus, ordine.metodo_pagamento, "
        "articolo.id, articolo.nome, articolo.modello, articolo.marca, articolo.descrizione, articolo.disponibilita, "
        "articolo.prezzo, articolo.pubblicazione, articolo.offerta, articolo.inizio_offerta, articolo.percentuale_sconto, "
        "articolo.durata_ore, articolo.categoria, articolo.negozio "
        "FROM ordine "
        "JOIN articolo ON ordine.articolo = articolo.id "
        "WHERE ordine.utente = %d", ut.getId());

    res = conn->ExecSQLtuples(sqlcmd);
    int numRows = PQntuples(res);

    for (int i = 0; i < numRows; ++i) {
        int idOrdine = atoi(PQgetvalue(res, i, 0));
        int quantita = atoi(PQgetvalue(res, i, 1));
        const char* dataOrdine = PQgetvalue(res, i, 2);
        const char* statoOrdine = PQgetvalue(res, i, 3);
        const char* metodoPagamento = PQgetvalue(res, i, 4);
        int idArticolo = atoi(PQgetvalue(res, i, 5));
        const char* nomeArticolo = PQgetvalue(res, i, 6);
        const char* modelloArticolo = PQgetvalue(res, i, 7);
        const char* marcaArticolo = PQgetvalue(res, i, 8);
        const char* descrizioneArticolo = PQgetvalue(res, i, 9);
        int disponibilitaArticolo = atoi(PQgetvalue(res, i, 10));
        double prezzoArticolo = atof(PQgetvalue(res, i, 11));
        const char* pubblicazioneArticolo = PQgetvalue(res, i, 12);
        bool offertaArticolo = strcmp(PQgetvalue(res, i, 13), "t") == 0;
        const char* inizioOffertaArticolo = PQgetvalue(res, i, 14);
        double percentualeSconto = atof(PQgetvalue(res, i, 15));
        int durataOfferta = atoi(PQgetvalue(res, i, 16));
        char* categoriaNome = PQgetvalue(res, i, 17);
        int idNegozio = atoi(PQgetvalue(res, i, 18));

        Categoria c(categoriaNome);
        Negozio negozio(idNegozio);
        Articolo articolo(idArticolo, nomeArticolo, modelloArticolo, marcaArticolo, descrizioneArticolo, disponibilitaArticolo, prezzoArticolo, pubblicazioneArticolo, offertaArticolo, inizioOffertaArticolo, percentualeSconto, durataOfferta, c, negozio);
        Ordine ordine(idOrdine, dataOrdine, statoOrdine, ut, Metodo_pagamento());
        ordini.push_back(ordine);
    }

    PQclear(res);
    return ordini;
}
*/