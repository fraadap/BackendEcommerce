#include "database.h"

// restituisce gli ultimi 10 articoli inseriti dai negozi
std::vector<Articolo> Database::get_home_page(){


    PGresult *res;
    char sqlcmd[1000];

    std::vector<Articolo> home_page; 

    sprintf(sqlcmd, 
    "SELECT articolo.id, articolo.descrizione, "
    "articolo.nome, articolo.modello, articolo.marca, articolo.disponibilita, articolo.prezzo, articolo.pubblicazione,"
    " articolo.offerta, articolo.inizio_offerta, articolo.percentuale_sconto, articolo.durata_ore, articolo.categoria, articolo.negozio "
    "FROM articolo ORDER BY articolo.pubblicazione DESC LIMIT 10");

    res = conn->ExecSQLtuples(sqlcmd);

    int numRows = PQntuples(res);
    for (int i = 0; i < numRows; ++i) {
        int idArticolo = atoi(PQgetvalue(res, i, 0));
        const char* descrizioneArticolo = PQgetvalue(res, i, 1);
        const char* nomeArticolo = PQgetvalue(res, i, 2);
        const char* modelloArticolo = PQgetvalue(res, i, 3);
        const char* marcaArticolo = PQgetvalue(res, i, 4);
        int disponibilitaArticolo = atoi(PQgetvalue(res, i, 5));
        double prezzoArticolo = atof(PQgetvalue(res, i, 6));
        const char* pubblicazioneArticolo = PQgetvalue(res, i, 7);
        bool offertaArticolo = (strcmp(PQgetvalue(res, i, 8), "t") == 0);
        const char* inizioOffertaArticolo = PQgetvalue(res, i, 9);
        double percentualeSconto = atof(PQgetvalue(res, i, 10));
        int durataOfferta = atoi(PQgetvalue(res, i, 11));
        const char* categoriaArticolo = PQgetvalue(res, i, 12);
        int idNegozio = atoi(PQgetvalue(res, i, 13));

        Negozio ne(idNegozio);
        Categoria cat(categoriaArticolo);
        Articolo ar(idArticolo, nomeArticolo, modelloArticolo, marcaArticolo, descrizioneArticolo, disponibilitaArticolo, prezzoArticolo, 
        pubblicazioneArticolo, offertaArticolo, inizioOffertaArticolo, percentualeSconto, durataOfferta, cat, ne);
        home_page.push_back(ar);
    }
    PQclear(res);

    return home_page;
}