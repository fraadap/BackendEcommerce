#include "database.h"
#include <postgresql/libpq-fe.h>

Articolo Database::create_articolo(Articolo ar) {
    PGresult *res;
    char sqlcmd[2000];
    char errMsg[100];

    // Verifica dei parametri dell'articolo e lancio dell'eccezione
    if (strlen(ar.getNome()) == 0 || strlen(ar.getModello()) == 0 || strlen(ar.getMarca()) == 0 || ar.getDisponibilita() < 0 || ar.getPrezzo() <= 0 || ar.getNegozio().getId() == 0 || strlen(ar.getCategoria().getNome()) == 0)  {
        sprintf(errMsg, "Uno dei parametri dell'articolo è nullo o non valido");
        throw InvalidParameterException(errMsg);
    }

    // Costruzione del comando SQL per verificare l'esistenza dell'articolo
    sprintf(sqlcmd, "SELECT ID FROM articolo WHERE Nome = '%s' AND Modello = '%s' AND Marca = '%s' AND Negozio = %d", ar.getNome(), ar.getModello(), ar.getMarca(), ar.getNegozio().getId());
    res = conn->ExecSQLtuples(sqlcmd);

    // Se l'articolo esiste già, solleva un'eccezione
    if (PQntuples(res) > 0) {
        sprintf(errMsg, "L'articolo '%s %s %s' esiste già", ar.getNome(), ar.getModello(), ar.getMarca());
        throw DuplicateKeyException(errMsg);
    } else {

        sprintf(sqlcmd, "SELECT ID FROM negozio WHERE id = %d", ar.getNegozio().getId());
        res = conn->ExecSQLtuples(sqlcmd);
        if (PQntuples(res) == 0){
            sprintf(errMsg, "Il negozio non esiste");
            throw InvalidParameterException(errMsg);
        }
        sprintf(sqlcmd, "SELECT nome FROM categoria WHERE nome = '%s'", ar.getCategoria().getNome());
        res = conn->ExecSQLtuples(sqlcmd);
        if (PQntuples(res) == 0){
            sprintf(errMsg, "La categoria non esiste");
            throw InvalidParameterException(errMsg);
        }
        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        if(ar.isOfferta()){
                sprintf(sqlcmd, "INSERT INTO articolo (Nome, Modello, Marca, Descrizione, Disponibilita, Prezzo, Pubblicazione, Offerta, Inizio_offerta, Percentuale_sconto, Durata_ore, Categoria, Negozio) VALUES ('%s', '%s', '%s', '%s', %d, %.2f, CURRENT_TIMESTAMP, %s, %s, %.2f, %d, '%s', %d) RETURNING ID",
                ar.getNome(), ar.getModello(), ar.getMarca(), ar.getDescrizione(), ar.getDisponibilita(), ar.getPrezzo(), "TRUE",
                "CURRENT_TIMESTAMP", ar.getPercentuale(), ar.getDurataOre(), ar.getCategoria().getNome(), ar.getNegozio().getId());
                res = conn->ExecSQLtuples(sqlcmd);
        
        } else{
                sprintf(sqlcmd, "INSERT INTO articolo (Nome, Modello, Marca, Descrizione, Disponibilita, Prezzo, Pubblicazione, Offerta, Inizio_offerta, Percentuale_sconto, Durata_ore, Categoria, Negozio) VALUES ('%s', '%s', '%s', '%s', %d, %.2f, CURRENT_TIMESTAMP, FALSE, NULL, NULL, NULL, '%s', %d) RETURNING ID",
                ar.getNome(), ar.getModello(), ar.getMarca(), ar.getDescrizione(), ar.getDisponibilita(), ar.getPrezzo(),
                ar.getCategoria().getNome(), ar.getNegozio().getId());
                res = conn->ExecSQLtuples(sqlcmd);
        }

        int id = atoi(PQgetvalue(res, 0, 0));
        PQclear(res);

        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);


        ar.setId(id);

    }

    return ar;
}
