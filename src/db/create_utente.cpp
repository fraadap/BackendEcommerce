#include "database.h"


Utente Database::create_utente(Utente ut){ // si deve pensare alla password
    PGresult *res;
    char sqlcmd[1000];
    char errMsg[100];

    if (strlen(ut.getUsername()) == 0 || strlen(ut.getNome()) == 0 || strlen(ut.getCognome()) == 0 || strlen(ut.getEmail()) == 0 || strlen(ut.getIndirizzo().getCivico()) == 0 || strlen(ut.getPassword()) == 0){
        std::sprintf(errMsg, "Uno dei parametri di utente è null");
        throw NullParameterException(errMsg);
        return ut;
    }    

    if (strlen(ut.getUsername())>20 || strlen(ut.getNome())>20 || strlen(ut.getCognome())>20){
        std::sprintf(errMsg, "Uno dei parametri di utente ha lunghezza maggiore di 20: %s %s %s", ut.getUsername(), ut.getNome(), ut.getCognome());
        throw InvalidParameterException(errMsg);
        return ut;
    }    

    sprintf(sqlcmd, "SELECT ID FROM utente WHERE username = '%s'", ut.getUsername());
    res = conn->ExecSQLtuples(sqlcmd);

    if (PQntuples(res) > 0) {
        int id = atoi(PQgetvalue(res, 0, 0));
        char message[100];
        std::sprintf(message, "L'username '%s' esiste già con id: %d", ut.getUsername(), id);
        throw DuplicateKeyException(message);
    } else {
        try{
            Indirizzo in = ut.getIndirizzo();
            ut.setIndirizzo(this->create_indirizzo(in));
        }catch(const std::exception& e){
            throw;
            return ut;
        }

        char* passHashed = this->computeHash(ut.getPassword());

        char* timestamp = getCurrentTimestamp();
        ut.setRegistrazione(timestamp);

        sprintf(sqlcmd, "BEGIN"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);

        sprintf(sqlcmd, "INSERT INTO utente VALUES (DEFAULT, '%s', '%s', '%s', '%s', '%s', '%d', '%s' ) RETURNING ID", ut.getUsername(), ut.getNome(), ut.getCognome(), ut.getRegistrazione(), ut.getEmail(), ut.getIndirizzo().getId(), passHashed);
        res = conn->ExecSQLtuples(sqlcmd);
        
        int id = atoi(PQgetvalue(res, 0, 0));
        ut.setId(id);

        sprintf(sqlcmd, "COMMIT"); 
        res = conn->ExecSQLcmd(sqlcmd);
        PQclear(res);
    }
    return ut;
}