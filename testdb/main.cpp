#include "../src/db/database.h"
#include <cstdio>

void testGetOrdiniCorriere(Database db){  // testato
    Corriere co(1);
        try{

        std::vector<Ordine_corriere> ordini = db.get_ordini_corriere(co);

        for (const auto& ord : ordini) {
            printf("ciao");
            
        }
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testGetOrdiniNegozio(Database db){  // testato
    try{
        Negozio ne(1);
        std::vector<Articolo_ordine> ordini = db.get_ordini_negozio(ne);

        for (const auto& ord : ordini) {
            printf("ciao");
            
        }
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}


void testGetLogs(Database db){  // testato
    try{
        std::vector<Log> logs;
        logs = db.get_log("", "", "Error_server");
        for (Log log : logs) {
            printf("Log: %d, time %s, tag:%s, info: %s\n",
            log.getId(), log.getTimestamp(), log.getTag(), log.getInfo());
        }
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    } 
}

void testCreateLog(Database db){  // testato
    try{
        Log log(0,"","PROBLEM","Internal Error" );
        log = db.create_log(log);
        printf("Log inserito: %d\n", log.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    } 
}



void testGetProfilo(Database db){
    try{
        Utente ut(1);
        Profilo p = db.get_profilo(ut);

        printf("profilo dell'utente: %d", p.getUtente().getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }   
}

void testGetHomePage(Database db){ // testato
    try{
        
        std::vector<Articolo> home_page = db.get_home_page();

        for (const auto& ar : home_page) {
            printf("Articolo: %d, %s\n",
            ar.getId(), ar.getNome());
        }
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testGetListaDesideri(Database db){ // testato
    Utente ut(1);
    try{
        
        std::vector<Lista_desideri> lista = db.get_lista_desideri(ut);

        for (const auto& li : lista) {
            printf("Lista dei desideri con id: %d, Articolo: %d (%s), Utente:%d\n",
            li.getId(), li.getArticolo().getId(), li.getArticolo().getNome(), li.getUtente().getId());
        }
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testGetCarrelli(Database db){ // testato
    Utente ut(1);
    try{
        
        std::vector<Carrello> carrelli = db.get_carrelli(ut);

        for (const auto& carrello : carrelli) {
            printf("Carrello con id: %d, Articolo: %d (%s), Utente:%d\n",
            carrello.getId(), carrello.getArticolo().getId(), carrello.getArticolo().getNome(), carrello.getUtente().getId());
        }
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testRemoveMetodoFromUtente(Database db){ // testato
/*
    // modo 1
    Utente ut(1);
    Metodo_pagamento mp(1);
    Utente_metodo um(0, ut, mp);
*/

    // modo 2
    Utente_metodo um(1);

    try{
        db.remove_metodo_pagamento_from_utente(um);
        printf("Metodo eliminato con successo dall'utente");
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testDeleteOrdine(Database db){ // testato
    Ordine ord(1);
    try{
        db.delete_ordine(ord);
        printf("Ordine %d eliminato con successo", ord.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testDeleteRecensione(Database db){ // testato
    Recensione r(1);
    try{
        db.delete_recensione(r);
        printf("Recensione %d eliminata con successo", r.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testDeleteListaDesideri(Database db){ // testato
    Lista_desideri ld(1);
    try{
        db.delete_lista_preferiti(ld);
        printf("Lista preferiti %d eliminata con successo", ld.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testDeleteCarrello(Database db){ // testato
    Carrello c(1);
    try{
        db.delete_carrello(c);
        printf("carrello %d eliminato con successo", c.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testLoginNegozio(Database db){ // testato
    Negozio ne("Nike", "pass");
    try{
        ne = db.login_negozio(ne);
        printf("Login negozio effettuato con successo, id: %d \n", ne.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testLoginUtente(Database db){ // testato
    Utente ut("fraadap", "pass");
    try{
        ut = db.login_utente(ut);
        printf("Login utente effettuato con successo, id: %d \n", ut.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testCreateRecensione(Database db){ // testato
    Articolo a(1);
    Utente ut(1);
    Recensione r(0, ut, a, "", 3, "Non male");
    try{
        r = db.create_recensione(r);
        printf("Recensione inserita con id: %d \n", r.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testAddArticoloToORdine(Database db){ //testato
    Articolo a(1);
    Ordine ord(1);
    Articolo_ordine ao(0, ord, a, 2);
    try{
        ao = db.add_articolo_to_ordine(ao);
        printf("Articolo %d aggiunto all'ordine %d:  Assegnazione con id: %d \n", ao.getArticolo().getId(), ao.getOrdine().getId(), ao.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testAddOrdineToCorriere(Database db){ // testato
    Corriere c(1);
    Ordine ord(1);
    Ordine_corriere oc(0,ord, c, "", "");
    try{
        oc = db.add_ordine_to_corriere(oc);
        printf("Ordine %d aggiunto a Corriere %d:  Assegnazione con id: %d \n",oc.getOrdine().getId(), oc.getCorriere().getId(), oc.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testCreateCorriere(Database db){ // testato
    Corriere c(0, "Mario", "rossi");
    try{
        c = db.create_corriere(c);
        printf("Corriere %s %s aggiunto con id:  %d \n",c.getNome(), c.getCognome(), c.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}


void testCreateOrdine(Database db){ // testato
    Utente ut(1);
    Metodo_pagamento mp(1);
    Ordine ord(0, "", "in elaborazione", 1, 1);
    try{
        ord = db.create_ordine(ord);
        printf("Ordine aggiunto con id:  %d \n",ord.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testCreateListaDesideri(Database db) { // testato
    Articolo a(1);
    Utente ut(1);
    Lista_desideri ld(0, ut, a, "");
    try{
        ld = db.create_lista_desideri(ld);
        printf("Lista desideri inserita con id:  %d \n",ld.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testCreateCarrello(Database db) { // testato
    Articolo a(1);
    Utente ut(1);
    Carrello c(0, ut, a, "", 2);
    try{
        c = db.create_carrello(c);
        printf("Carrello inserito con id:  %d\n",c.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testCreateSpesaSpedizione(Database db) { // testato
    Articolo a(1);
    SpesaSpedizione ss(0, "comunale", 10.2, a);
    try{
        ss = db.create_spesa_spedizione(ss);
        printf("Spesa spedizione: %s inserita con id:  %d \n", ss.getTipologia(), ss.getId());
    } catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testCreateArticolo(Database db) { // testato
    Negozio n(1);
    Categoria c("Scarpe", "");
    Articolo a(0, "Stan Smit", "Classic", "Adidas", "Stan Smith Adidas taglia 44",
    1, 120, "", false,
     "", 0, 0, c, n);
    try{
        a = db.create_articolo(a);
        printf("Articolo: %s Inserito con ID: %d \n", a.getNome(), a.getId());
    }catch(const std::exception& e){  
        printf("%s \n", e.what());
    }
}

void testCreateCategoria(Database db){ // testato
    Categoria c("Scarpe", "");
    Categoria c1("Scarpe da calcio", "Scarpe");
    try{
        db.create_categoria(c);
        db.create_categoria(c1);
        printf("creata categoria %s con padre %s \n", c.getNome(), c.getPadre());

    }catch(const std::exception& e){  
        printf("%s \n", e.what());
    }  
}

void testAddMetodoToUtente(Database db){ // testato
    Metodo_pagamento mp(1);
    Indirizzo in(1);
    Utente ut(1);
    Utente_metodo um(0, ut, mp, "");
    try{
        um = db.add_metodo_pagamento_to_utente(um);
        printf("metodo: %d Aggiunto a utente: %d. Utente metodo con id: %d \n", mp.getId(), ut.getId(), um.getId());
    }catch(const std::exception& e){
        printf("%s \n", e.what());
    }
}

void testCreateUtente(Database db){ // testato
    Nazione n("Italia");
    Citta c(0, "Roma", n);
    Cap cap("00198", c);
    Indirizzo in(0,"donizetti","7", 0, cap);
    Utente ut(0,"fraadap","francesco","daprile","","daprile@f.com", in, "pass");

    try{
        ut = db.create_utente(ut);
        printf("Username: %s Con ID: %d \n", ut.getUsername(), ut.getId());
    }catch(const std::exception& e){
        printf("%s \n", e.what());
    }
}

void testCreateNegozio(Database db){ // testato
    Nazione n("Italia");
    Citta c(0, "Roma", n);
    Cap cap("00198", c);
    Indirizzo in(0,"donizetti","7", 0, cap);
    Metodo_pagamento mp(0, "122234556", "IO", "2025-04-30");
    Negozio ne(0,"Nike","32283999","nike@n.it", in, mp, "pass");

    try{
        ne = db.create_negozio(ne);
        printf("Negozio: %s Con ID: %d \n", ne.getNome(), ne.getId());
    }catch(const std::exception& e){
        printf("%s \n", e.what());
    }
}

int main(){

    Con2DB con("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    Database db(&con);

    //testCreateUtente(db);
    //testCreateNegozio(db);
    //testAddMetodoToUtente(db);
    //testCreateCategoria(db);
    //testCreateArticolo(db);
    //testCreateSpesaSpedizione(db);
    //testCreateCarrello(db);
    //testCreateListaDesideri(db);
    //testCreateOrdine(db);
    //testCreateCorriere(db);
    //testAddOrdineToCorriere(db);
    //testAddArticoloToORdine(db);
    //testCreateRecensione(db);
    //testLoginUtente(db);
    //testLoginNegozio(db);
    //testDeleteCarrello(db);
    //testDeleteListaDesideri(db);
    //testDeleteRecensione(db);
    //testDeleteOrdine(db);
    //testRemoveMetodoFromUtente(db);
    //testGetCarrelli(db);
    //testGetListaDesideri(db);
    //testGetHomePage(db);
    //testGetProfilo(db);
    //testCreateLog(db);
    //testGetLogs(db);
    //testGetOrdiniNegozio(db);
    testGetOrdiniCorriere(db);
    return 0;
}