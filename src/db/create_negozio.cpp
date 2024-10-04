#include "database.h"


Negozio Database::create_negozio(Negozio ne){
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(ne.getNome()) == 0 || strlen(ne.getTelefono()) == 0 || strlen(ne.getEmail()) == 0 || strlen(ne.getIndirizzo().getVia()) == 0 || strlen(ne.getMetodo_pagamento().getNumero()) == 0 || strlen(ne.getPassword())==0){
        std::sprintf(errMsg, "Uno dei parametri del negozio è null: %s %s %s %s %s", ne.getNome(), ne.getTelefono(), ne.getEmail(), ne.getIndirizzo().getVia(), ne.getMetodo_pagamento().getNumero());
        throw NullParameterException(errMsg);
        return ne;
    }    

    if (strlen(ne.getNome()) > 20 || strlen(ne.getTelefono()) > 15){
        std::sprintf(errMsg, "Uno dei parametri del negozio non rispetta la lunghezza desiderata: %s %s", ne.getNome(), ne.getTelefono());
        throw InvalidParameterException(errMsg);
        return ne;
    }    

    sprintf(sqlcmd, "SELECT ID FROM negozio WHERE nome = '%s'", ne.getNome());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        int id = atoi(PQgetvalue(res, 0, 0));
        char message[100];
        std::sprintf(message, "Il negozio '%s' esiste già con id: %d", ne.getNome(), id);
        throw DuplicateKeyException(message);
    } else {
        try{
            Indirizzo in = ne.getIndirizzo();
            ne.setIndirizzo(this->create_indirizzo(in));

            Metodo_pagamento mp = ne.getMetodo_pagamento();
            ne.setMetodoPagamento(this->create_metodo_pagamento(mp));

        }catch(const std::exception& e){
            throw;
            return ne;
        }


        char* passHashed = this->computeHash(ne.getPassword());


        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO negozio VALUES (DEFAULT, '%s', '%s', '%s', '%d', '%d', '%s' ) RETURNING ID", ne.getNome(), ne.getTelefono(), ne.getEmail(), ne.getIndirizzo().getId(), ne.getMetodo_pagamento().getId(), passHashed);
        res = conn->ExecSQLtuples(sqlcmd);
        
        int id = atoi(PQgetvalue(res, 0, 0));
        ne.setId(id);

        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
    }
    return ne;
}