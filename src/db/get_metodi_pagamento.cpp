#include "database.h"

std::vector<Metodo_pagamento> Database::get_metodi_pagamento(Utente ut){

    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    std::vector<Metodo_pagamento> metodi;
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
    "SELECT mp.id, mp.numero, mp.titolare, mp.scadenza "
    " FROM utente_metodo JOIN metodo_pagamento AS mp ON utente_metodo.metodo_pagamento = mp.id WHERE utente_metodo.utente = %d",
    ut.getId());

    res = conn->ExecSQLtuples(sqlcmd);

    int numRows = PQntuples(res);
    for (int i = 0; i < numRows; ++i) {
        int idMetodo = atoi(PQgetvalue(res, i, 0));
        const char* numero = PQgetvalue(res, i, 1);
        const char* titolare = PQgetvalue(res, i, 2);
        const char* scadenza = PQgetvalue(res, i, 3);
        
        Metodo_pagamento mp(idMetodo, numero, titolare, scadenza);
        metodi.push_back(mp);
    }
    PQclear(res);

    return metodi;
}