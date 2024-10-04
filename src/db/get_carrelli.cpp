#include "database.h"

std::vector<Carrello> Database::get_carrelli(Utente ut){

    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    std::vector<Carrello> carrelli;
    if (ut.getId() == 0 ) {
        std::sprintf(errMsg, "L'id di utente è null");
        throw NullParameterException(errMsg);
    }    
    

    sprintf(sqlcmd, "SELECT ID FROM utente WHERE id = %d", ut.getId());
    res = conn->ExecSQLtuples(sqlcmd);
    if (PQntuples(res) == 0) {
        std::sprintf(errMsg, "Utente specificato non trovato");
        throw RecordNotFoundException(errMsg);
    }
    
    sprintf(sqlcmd, 
    "SELECT carrello.id, carrello.utente, carrello.articolo, carrello.aggiunta, carrello.quantita, "
    "articolo.nome, articolo.modello, articolo.marca, articolo.disponibilita, articolo.prezzo, articolo.pubblicazione,"
    " articolo.offerta, articolo.inizio_offerta, articolo.percentuale_sconto, articolo.durata_ore, articolo.categoria, articolo.negozio, articolo.descrizione "
    "FROM carrello JOIN articolo ON carrello.articolo = articolo.id WHERE carrello.utente = %d",
    ut.getId());

    res = conn->ExecSQLtuples(sqlcmd);

    int numRows = PQntuples(res);
    for (int i = 0; i < numRows; ++i) {
        int idCarrello = atoi(PQgetvalue(res, i, 0));
        int idUtente = atoi(PQgetvalue(res, i, 1));
        int idArticolo = atoi(PQgetvalue(res, i, 2));
        const char* aggiunta = PQgetvalue(res, i, 3);
        int quantita = atoi(PQgetvalue(res, i, 4));
        const char* nomeArticolo = PQgetvalue(res, i, 5);
        const char* modelloArticolo = PQgetvalue(res, i, 6);
        const char* marcaArticolo = PQgetvalue(res, i, 7);
        int disponibilitaArticolo = atoi(PQgetvalue(res, i, 8));
        double prezzoArticolo = atof(PQgetvalue(res, i, 9));
        const char* pubblicazioneArticolo = PQgetvalue(res, i, 10);
        bool offertaArticolo = (strcmp(PQgetvalue(res, i, 11), "t") == 0);
        const char* inizioOffertaArticolo = PQgetvalue(res, i, 12);
        double percentualeSconto = atof(PQgetvalue(res, i, 13));
        int durataOfferta = atoi(PQgetvalue(res, i, 14));
        const char* categoriaArticolo = PQgetvalue(res, i, 15);
        int idNegozio = atoi(PQgetvalue(res, i, 16));
        const char* descrizioneArticolo = PQgetvalue(res, i, 17);
        
        Utente ut(idUtente);
        Negozio ne(idNegozio);
        Categoria cat(categoriaArticolo);
        Articolo ar(idArticolo, nomeArticolo, modelloArticolo, marcaArticolo, descrizioneArticolo, disponibilitaArticolo, prezzoArticolo, 
        pubblicazioneArticolo, offertaArticolo, inizioOffertaArticolo, percentualeSconto, durataOfferta, cat, ne);
        Carrello carrello(idCarrello, ut, ar, aggiunta, quantita);
        carrelli.push_back(carrello);
    }
    PQclear(res);

    return carrelli;
}