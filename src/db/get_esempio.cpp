#include "database.h"
#include <cstdio>

void Database::get_esempio() {
    char query [1000];
    PGresult *res;

    sprintf(query, "INSERT INTO nazione (nome) VALUES ('Italia')");
    res = conn->ExecSQLcmd(query);

    if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    // L'operazione di inserimento è stata eseguita con successo
    printf("Inserimento avvenuto con successo. Numero di righe inserite: %d\n", atoi(PQcmdTuples(res)));
    } else {
    // Stampa l'eccezione
    printf("Errore durante l'inserimento: %s\n", PQresultErrorMessage(res));
    }

    PQclear(res);

    sprintf(query,"SELECT nome FROM nazione;" );
    res = this->conn->ExecSQLtuples(query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Query fallita: " << PQresultErrorMessage(res) << std::endl;
        PQclear(res);
    } else {
        int rows = PQntuples(res); 
        int cols = PQnfields(res);
        
        // Itero su queste e stampo tutti i campi
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << PQgetvalue(res, i, j) << "\t"; // Accedi al valore tramite l'indice di riga e colonna
            }
            std::cout << std::endl;
        }
        PQclear(res);
    }
    
    this->conn->finish();

}