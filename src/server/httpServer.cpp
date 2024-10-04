#include "httpServer.h"
#include <csignal>
#include <cstring>
#include <hiredis/hiredis.h>
#include <iostream>
#include <ostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <string.h>
#include "utils.h"
#include <chrono>
#include <thread>

#define USERREAD "userRead"
#define USERWRITE "userWrite"
#define SHOPREAD "shopRead"
#define SHOPWRITE "shopWrite"
#define ADMINREAD "adminRead"
#define ADMINWRITE "adminWrite"
#define DELIVERYREAD "deliveryRead"
#define DELIVERYWRITE "deliveryWrite"


// Variabile globale per verificare se è stato ricevuto il segnale SIGINT
volatile bool stopRequested = false;
redisContext* redis = nullptr;
pid_t mainProcessId = getpid();
// Variabili globali per tenere traccia dei processi figli e dei thread attivi
std::vector<pid_t> childProcesses;
std::vector<std::thread> threads;
int serverSocket = -1;

// Funzione per terminare i processi figli
void terminateChildProcesses() {
    for (pid_t pid : childProcesses) {
        kill(pid, SIGTERM); // Invia un segnale di terminazione a ciascun processo figlio
    }
}

// Funzione per terminare i thread
void terminateThreads() {
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join(); // Aspetta che il thread termini
        }
    }
}

// Funzione di gestione del segnale SIGINT
void signalHandler(int signal) {
    if (signal == SIGINT){
    // Verifica se questo è il processo principale
        if (getpid() == mainProcessId) {
            // gestore del segnale SIGINT
            stopRequested = true;
            std::cout << " Ricevuto segnale SIGINT, terminazione in corso..." << std::endl;
            terminateThreads(); // Termina i thread
            terminateChildProcesses(); // Termina i processi figli
            // Libera il contesto Redis
            if (redis != nullptr) {
                redisFree(redis); // Usare const_cast per rimuovere il modificatore const
            }
            if (serverSocket != -1) {
                close(serverSocket);
            }
            exit(EXIT_SUCCESS); // Termina il programma
        }
    }
    
}

void leggiThread(){
    // inizializzazione della connessione al db
    Con2DB con("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    Database db(&con);

    // inizializzazione della comunicazione Redis
    redisContext *conn = redisConnect("127.0.0.1", 6379);
    if (conn == nullptr || conn->err) {
        if (conn) {
            std::cerr << "Errore di connessione: " << conn->errstr << std::endl;
            redisFree(conn); // Usare const_cast per rimuovere il modificatore const
        } else {
            std::cerr << "Errore di allocazione contesto Redis" << std::endl;
        }
        return;
    }
    while(!stopRequested){
        // lettura da userWrite
        std::string richiesta = "XREADGROUP GROUP diameter username BLOCK 1 COUNT 1 NOACK STREAMS " + std::string(USERWRITE) + " >";
        redisReply *reply = (redisReply*)redisCommand(conn, richiesta.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            assertReply(conn, reply);
            int clientSocket = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
            std::string response = reply->element[0]->element[1]->element[0]->element[1]->element[3]->str;
            freeReplyObject(reply);
            
            // Invia la risposta al client
            write(clientSocket, response.c_str(), response.length());
            db.update_request_log(RequestLog(0, clientSocket, "", ""));
            close(clientSocket);
        }

        // lettura da shopWrite
        richiesta = "XREADGROUP GROUP diameter username BLOCK 1 COUNT 1 NOACK STREAMS " + std::string(SHOPWRITE) + " >";
        reply = (redisReply*)redisCommand(conn, richiesta.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            assertReply(conn, reply);
            int clientSocket = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
            std::string response = reply->element[0]->element[1]->element[0]->element[1]->element[3]->str;

            freeReplyObject(reply);
            // Invia la risposta al client
            write(clientSocket, response.c_str(), response.length());
            db.update_request_log(RequestLog(0, clientSocket, "", ""));
            close(clientSocket);
        }

        //lettura da deliveryWrite
        richiesta = "XREADGROUP GROUP diameter username BLOCK 1 COUNT 1 NOACK STREAMS " + std::string(DELIVERYWRITE) + " >";
        reply = (redisReply*)redisCommand(conn, richiesta.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            assertReply(conn, reply);
            int clientSocket = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
            std::string response = reply->element[0]->element[1]->element[0]->element[1]->element[3]->str;

            freeReplyObject(reply);

            // Invia la risposta al client
            write(clientSocket, response.c_str(), response.length());
            db.update_request_log(RequestLog(0, clientSocket, "", ""));
            close(clientSocket);
        }

        // lettura da adminWrite
        richiesta = "XREADGROUP GROUP diameter username BLOCK 1 COUNT 1 NOACK STREAMS " + std::string(ADMINWRITE) + " >";
        reply = (redisReply*)redisCommand(conn, richiesta.c_str());
        if (reply->type != REDIS_REPLY_NIL) {
            assertReply(conn, reply);
            int clientSocket = atoi(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
            std::string response = reply->element[0]->element[1]->element[0]->element[1]->element[3]->str;

            freeReplyObject(reply);

            // Invia la risposta al client
            write(clientSocket, response.c_str(), response.length());
            db.update_request_log(RequestLog(0, clientSocket, "", ""));
            close(clientSocket);
        }
    }
    redisFree(conn);
    return;
}

void HttpServer::start() {
    // gestore del segnale SIGINT
    signal(SIGINT, signalHandler);

    redisReply *reply;

    pid_t pid;

    Con2DB con("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    Database db(&con);

    // Creazione del socket TCP per il server
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        db.create_log(Log(0, "", "Server_error", "Errore durante la creazione del socket"));
        std::cerr << "Errore durante la creazione del socket" << std::endl;
        return;
    }

    // Associazione del socket all'indirizzo IP e alla porta
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port_);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) != 0) {
        db.create_log(Log(0, "", "Server_error", "Errore durante il binding del socket"));
        std::cerr << "Errore durante il binding del socket" << std::endl;
        close(serverSocket);
        return;
    }

    // inizializzazione della comunicazione Redis
    redis = redisConnect("127.0.0.1", 6379);
    if (redis == nullptr || redis->err) {
        if (redis) {
            db.create_log(Log(0, "", "Server_error", "Errore di connessione a redis"));
            std::cerr << "Errore di connessione a redis: " << redis->errstr << std::endl;
            redisFree(redis); // Usare const_cast per rimuovere il modificatore const
        } else {
            db.create_log(Log(0, "", "Server_error", "Errore di allocazione contesto Redis"));
            std::cerr << "Errore di allocazione contesto Redis" << std::endl;
        }
        return;
    }
    
    reply = RedisCommand(redis, "DEL %s", USERREAD);
    assertReply(redis, reply);

    reply = RedisCommand(redis, "DEL %s", USERWRITE);
    assertReply(redis, reply);

    reply = RedisCommand(redis, "DEL %s", ADMINREAD);
    assertReply(redis, reply);

    reply = RedisCommand(redis, "DEL %s", ADMINWRITE);
    assertReply(redis, reply);

    reply = RedisCommand(redis, "DEL %s", SHOPREAD);
    assertReply(redis, reply);

    reply = RedisCommand(redis, "DEL %s", SHOPWRITE);
    assertReply(redis, reply);

    reply = RedisCommand(redis, "DEL %s", DELIVERYREAD);
    assertReply(redis, reply);

    reply = RedisCommand(redis, "DEL %s", DELIVERYWRITE);
    assertReply(redis, reply);

    initStreams(redis, USERREAD);
    initStreams(redis, USERWRITE);
    initStreams(redis, ADMINREAD);
    initStreams(redis, ADMINWRITE);
    initStreams(redis, SHOPREAD);
    initStreams(redis, SHOPWRITE);
    initStreams(redis, DELIVERYREAD);
    initStreams(redis, DELIVERYWRITE);

    // creazione dei processi managers

    pid = fork();
    if (pid < 0){
        db.create_log(Log(0, "", "Server_error", "Errore durante la creazione del processo user"));
        std::cerr << "Errore durante la creazione del processo user" << std::endl;
    }else if (pid == 0) {
        // processo figlio
        UserManager().manage();
        return;
    }
    childProcesses.push_back(pid);

    pid = fork();
    if (pid < 0){
        db.create_log(Log(0, "", "Server_error", "Errore durante la creazione del processo shop"));
        std::cerr << "Errore durante la creazione del processo shop" << std::endl;
    }else if (pid == 0) {
        // processo figlio
        ShopManager().manage();
        return;
    }
    childProcesses.push_back(pid);

    pid = fork();
    if (pid < 0){
        db.create_log(Log(0, "", "Server_error", "Errore durante la creazione del processo admin"));
        std::cerr << "Errore durante la creazione del processo admin" << std::endl;
    }else if (pid == 0) {
        // processo figlio
        AdminManager().manage();
        return;
    }
    childProcesses.push_back(pid);

    pid = fork();
    if (pid < 0){
        db.create_log(Log(0, "", "Server_error", "Errore durante la creazione del processo delivery"));
        std::cerr << "Errore durante la creazione del processo delivery" << std::endl;
    }else if (pid == 0) {
        // processo figlio
        DeliverManager().manage();
        return;
    }
    childProcesses.push_back(pid);

    std::cout << "Processi managers creati con successo \n" << std::endl;


    // Mette il socket in ascolto per le connessioni in arrivo
    if (listen(serverSocket, SOMAXCONN) == -1) {
        db.create_log(Log(0, "", "Server_error", "Errore durante l'ascolto sul socket"));
        std::cerr << "Errore durante l'ascolto sul socket" << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Server HTTP in ascolto sulla porta \n" << port_ << "..." << std::endl;

    threads.emplace_back(leggiThread);

    // Loop principale per accettare le connessioni in arrivo
    while (!stopRequested) {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
        if (serverSocket == -1){ close(serverSocket); break; }
        if (clientSocket == -1) {
            db.create_log(Log(0, "", "Server_error", "Errore durante l'accettazione della connessione HTTP"));
            std::cerr << "Errore durante l'accettazione della connessione HTTP" << std::endl;
            continue;
        }
        // Gestisce la richiesta del client
            // analizza la richiesta, in base al path manda l'operazione a un manager
        // il manager fa l'operazione sul db e restituisce al thread per rispondere al client
        
        db.create_request_log(RequestLog(0, clientSocket, "", ""));
        char buffer[2048];
        memset(buffer, 0, sizeof(buffer));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            db.create_log(Log(0, "", "Server_error", "Errore durante la lettura della richiesta HTTP"));
            std::cerr << "Errore durante la lettura della richiesta HTTP" << std::endl;
            close(clientSocket);
            close(serverSocket);
            return;
        }

        // Analizza la richiesta HTTP per estrarre il percorso
        std::string request(buffer, bytesRead);
        
        std::istringstream iss(request);
        std::string method, path, version;
        iss >> method >> path >> version;

        

        std::vector<std::string> tokens = splitString(path);
        int tokenSize = tokens.size() - 1;
        int count = 1;
        if (tokens[count] == "login_user" && method=="POST") {
            // passa al manager user
            // Trova l'inizio del body JSON
            std::string operationId = "doLoginUser";
            char* bodyStart = strstr(buffer, "\r\n\r\n");
            if (bodyStart == nullptr) {
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            bodyStart += 4; // Spostati oltre il doppio CRLF

            std::string str(bodyStart);

            std::string body = replaseString(str);
            
            std::vector<std::string> loginUtente = splitStringJson(body);

            if ( loginUtente.size() != 4){
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            else{
                std::string username;
                std::string password;
                for(int x = 0; x < (loginUtente.size() - 1); x = x + 2 ){
                    if ( loginUtente[x] == "username" ) {
                        username = loginUtente[x + 1];
                    }
                    else if ( loginUtente[x] == "password" ){
                        password = loginUtente[x + 1];
                    }
                    else{
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                        write(clientSocket, response.c_str(), response.length());
                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                        close(clientSocket);
                    }
                }

                

                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %s %s %s", 
                    USERREAD, 
                    "idSocket", clientSocket, 
                    "operationId", operationId.c_str(), 
                    "username", username.c_str(), 
                    "password", password.c_str());
                freeReplyObject(reply);
            }
        } else if (tokens[count] == "registration_user" && method=="POST"){
            // passa al manager user
            // Trova l'inizio del body JSON
            std::string operationId = "doRegisterUser";
            char* bodyStart = strstr(buffer, "\r\n\r\n");
            if (bodyStart == nullptr) {
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            bodyStart += 4; // Spostati oltre il doppio CRLF

            std::string str(bodyStart);


            std::string body = replaseString(str);

            std::vector<std::string> json = splitStringJson(body);

            if( json.size() != 22){
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            else{
                std::string nazione;
                std::string citta;
                std::string cap;
                std::string via;
                std::string civico;
                int interno;
                std::string username;
                std::string nome;
                std::string cognome;
                std::string mail;
                std::string password;

                for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                    if ( json[x] == "nazione" ) {
                        nazione = json[x + 1];
                    }
                    else if ( json[x] == "citta" ){
                        citta = json[x + 1];
                    }
                    else if ( json[x] == "cap" ){
                        cap = json[x + 1];
                    }
                    else if ( json[x] == "via" ){
                        via = json[x + 1];
                    }
                    else if ( json[x] == "civico" ){
                        civico = json[x + 1];
                    }
                    else if ( json[x] == "interno" ){
                        interno = atoi(json[x + 1].c_str());
                    }
                    else if ( json[x] == "username" ){
                        username = json[x + 1];
                    }
                    else if ( json[x] == "nome" ){
                        nome = json[x + 1];
                    }
                    else if ( json[x] == "cognome" ){
                        cognome = json[x + 1];
                    }
                    else if ( json[x] == "mail" ){
                        mail = json[x + 1];
                    }
                    else if ( json[x] == "password" ){
                        password = json[x + 1];
                    }
                    else{
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                        write(clientSocket, response.c_str(), response.length());
                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                        close(clientSocket);
                    }
                }
                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %s %s %s %s %s %s %s %s %s %s %d %s %s %s %s %s %s %s %s %s %s", 
                    USERREAD, 
                    "idSocket", clientSocket, 
                    "operationId", operationId.c_str(), 
                    "nazione", nazione.c_str(), 
                    "citta", citta.c_str(), 
                    "cap", cap.c_str(), 
                    "via", via.c_str(), 
                    "civico", civico.c_str(), 
                    "interno", interno, 
                    "username", username.c_str(), 
                    "nome", nome.c_str(), 
                    "cognome", cognome.c_str(), 
                    "mail", mail.c_str(), 
                    "password", password.c_str());
                freeReplyObject(reply);
            }
        } else if (tokens[count] == "login_admin" && method=="POST"){
            // passa al manager admin
            // Trova l'inizio del body JSON
            std::string operationId = "doLoginAdmin";
            char* bodyStart = strstr(buffer, "\r\n\r\n");
            if (bodyStart == nullptr) {
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            bodyStart += 4; // Spostati oltre il doppio CRLF

            std::string str(bodyStart);

            std::string body = replaseString(str);
            
            std::vector<std::string> loginUtente = splitStringJson(body);

                
            if ( loginUtente.size() != 4){
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            else{
                std::string nome;
                std::string password;
                for(int x = 0; x < (loginUtente.size() - 1); x = x + 2 ){
                    if ( loginUtente[x] == "nome" ) {
                        nome = loginUtente[x + 1];
                    }
                    else if ( loginUtente[x] == "password" ){
                        password = loginUtente[x + 1];
                    }
                    else{
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                        write(clientSocket, response.c_str(), response.length());
                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                        close(clientSocket);
                    }
                }

                

                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %s %s %s", 
                    ADMINREAD, 
                    "idSocket", clientSocket, 
                    "operationId", operationId.c_str(), 
                    "nome", nome.c_str(), 
                    "password", password.c_str());
                freeReplyObject(reply);
            }
        } else if (tokens[count] == "registration_shop" && method=="POST"){
            // passa al manager shop
            // Trova l'inizio del body JSON
            std::string operationId = "doRegisterShop";
            char* bodyStart = strstr(buffer, "\r\n\r\n");
            if (bodyStart == nullptr) {
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            bodyStart += 4; // Spostati oltre il doppio CRLF

            std::string str(bodyStart);


            std::string body = replaseString(str);

            std::vector<std::string> json = splitStringJson(body);    

            if ( json.size() != 26 ){
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            else{
                std::string nazione;
                std::string citta;
                std::string cap;
                std::string via;
                std::string civico;
                int interno;
                std::string numero;
                std::string titolare;
                std::string scadenza;
                std::string nome;
                std::string telefono;
                std::string mail;
                std::string password;

                for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                    if ( json[x] == "nazione" ) {
                        nazione = json[x + 1];
                    }
                    else if ( json[x] == "citta" ){
                        citta = json[x + 1];
                    }
                    else if ( json[x] == "cap" ){
                        cap = json[x + 1];
                    }
                    else if ( json[x] == "via" ){
                        via = json[x + 1];
                    }
                    else if ( json[x] == "civico" ){
                        civico = json[x + 1];
                    }
                    else if ( json[x] == "interno" ){
                        interno = atoi(json[x + 1].c_str());
                    }
                    else if ( json[x] == "numero" ){
                        numero = json[x + 1];
                    }
                    else if ( json[x] == "titolare" ){
                        titolare = json[x + 1];
                    }
                    else if ( json[x] == "scadenza" ){
                        scadenza = json[x + 1];
                    }
                    else if ( json[x] == "nome" ){
                        nome = json[x + 1];
                    }
                    else if ( json[x] == "telefono" ){
                        telefono = json[x + 1];
                    }
                    else if ( json[x] == "mail" ){
                        mail = json[x + 1];
                    }
                    else if ( json[x] == "password" ){
                        password = json[x + 1];
                    }
                    else{
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                        write(clientSocket, response.c_str(), response.length());
                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                        close(clientSocket);
                    }
                }
                if (!isDate(scadenza.c_str())){
                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 32\r\n\r\nScadenza non rispetta i standard";
                    write(clientSocket, response.c_str(), response.length());
                    db.update_request_log(RequestLog(0, clientSocket, "", ""));
                    close(clientSocket);
                }
                else{
                    redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %s %s %s %s %s %s %s %s %s %s %d %s %s %s %s %s %s %s %s %s %s %s %s %s %s", 
                        SHOPREAD, 
                        "idSocket", clientSocket, 
                        "operationId", operationId.c_str(), 
                        "nazione", nazione.c_str(), 
                        "citta", citta.c_str(), 
                        "cap", cap.c_str(), 
                        "via", via.c_str(), 
                        "civico", civico.c_str(), 
                        "interno", interno, 
                        "numero", numero.c_str(), 
                        "titolare", titolare.c_str(), 
                        "scadenza", scadenza.c_str(), 
                        "nome", nome.c_str(), 
                        "telefono", telefono.c_str(), 
                        "mail", mail.c_str(), 
                        "password", password.c_str());
                    freeReplyObject(reply);
                }
            }
        } else if (tokens[count] == "login_shop" && method=="POST"){
            // passa al manager user
            // Trova l'inizio del body JSON
            std::string operationId = "doLoginShop";
            char* bodyStart = strstr(buffer, "\r\n\r\n");
            if (bodyStart == nullptr) {
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            bodyStart += 4; // Spostati oltre il doppio CRLF

            std::string str(bodyStart);

            std::string body = replaseString(str);

            

            std::vector<std::string> json = splitStringJson(body);

            if( json.size() != 4){
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            else{
                std::string nome;
                std::string password;
                for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                    if ( json[x] == "nome" ) {
                        nome = json[x + 1];
                    }
                    else if ( json[x] == "password" ){
                        password = json[x + 1];
                    }
                    else{
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                        write(clientSocket, response.c_str(), response.length());
                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                        close(clientSocket);
                    }
                }

                

                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %s %s %s", 
                    SHOPREAD, 
                    "idSocket", clientSocket, 
                    "operationId", operationId.c_str(), 
                    "nome", nome.c_str(), 
                    "password", password.c_str());
                freeReplyObject(reply);
            }
        } else if(tokens[count] == "register_courier" && method=="POST"){
            // passa al manager delivery

            // Trova l'inizio del body JSON
            std::string operationId = "doRegisterCourier";
            char* bodyStart = strstr(buffer, "\r\n\r\n");
            if (bodyStart == nullptr) {
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            bodyStart += 4; // Spostati oltre il doppio CRLF

            std::string str(bodyStart);


            std::string body = replaseString(str);

            

            std::vector<std::string> json = splitStringJson(body);


            if( json.size() != 4){
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            else{
                std::string nome;
                std::string cognome;
                for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                    if ( json[x] == "nome" ) {
                        nome = json[x + 1];
                    }
                    else if ( json[x] == "cognome" ){
                        cognome = json[x + 1];
                    }
                    else{
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                        write(clientSocket, response.c_str(), response.length());
                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                        close(clientSocket);
                    }
                }

                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %s %s %s", 
                    DELIVERYREAD, 
                    "idSocket", clientSocket, 
                    "operationId", operationId.c_str(), 
                    "nome", nome.c_str(), 
                    "cognome", cognome.c_str());
                freeReplyObject(reply);
            }
        } else if (tokens[count] == "users"){
            ++count;
            if (count != tokenSize){
                int idUtente = std::stoi(tokens[count]);
                ++count;
                if (count != tokenSize){
                    if (tokens[count] == "item"){
                        ++count;
                        int itemId = std::stoi(tokens[count]);
                        if (count != tokenSize){
                            ++count;
                            if (count != tokenSize){
                                if (tokens[count] == "favorite"){
                                    ++count;
                                    int favoriteId = std::stoi(tokens[count]);
                                    if (count == tokenSize){
                                        if (method == "DELETE"){
                                            // passa al manager user

                                            std::string operationId = "unFavoriteItem";

                                            redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                                                USERREAD, 
                                                "idSocket", clientSocket, 
                                                "operationId", operationId.c_str(), 
                                                "favoriteId", favoriteId);
                                            freeReplyObject(reply);
                                        }
                                        else {
                                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                            write(clientSocket, response.c_str(), response.length());
                                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                            close(clientSocket);
                                        }
                                    } else{
                                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                        write(clientSocket, response.c_str(), response.length());
                                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                        close(clientSocket);
                                    }
                                } 
                                else if (tokens[count] == "review"){
                                    ++count;
                                    int reviewId = std::stoi(tokens[count]);
                                    if (count == tokenSize){
                                        if (method == "DELETE"){
                                            // passa al manager user

                                            std::string operationId = "unReviewItem";

                                            redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                                                USERREAD, 
                                                "idSocket", clientSocket, 
                                                "operationId", operationId.c_str(), 
                                                "reviewId", reviewId);
                                            freeReplyObject(reply);
                                        }
                                        else {
                                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                            write(clientSocket, response.c_str(), response.length());
                                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                            close(clientSocket);
                                        }
                                    }
                                    else{
                                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                        write(clientSocket, response.c_str(), response.length());
                                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                        close(clientSocket);
                                    }
                                }
                                else{
                                    
                                }
                            } else {
                                if (tokens[count] == "review" && method == "POST"){
                                    // passa al manager user
                                    
                                    std::string valutazione;
                                    std::string descrizione;

                                    // Trova l'inizio del body JSON
                                    std::string operationId = "reviewItem";
                                    char* bodyStart = strstr(buffer, "\r\n\r\n");
                                    if (bodyStart == nullptr) {
                                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                                        write(clientSocket, response.c_str(), response.length());
                                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                        close(clientSocket);
                                    }
                                    bodyStart += 4; // Spostati oltre il doppio CRLF

                                    std::string str(bodyStart);


                                    std::string body = replaseString(str);

                                    

                                    std::vector<std::string> json = splitStringJson(body);


                                    if( json.size() != 4){
                                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                        write(clientSocket, response.c_str(), response.length());
                                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                        close(clientSocket);
                                    }
                                    else{
                                        std::string valutazione;
                                        std::string descrizione;
                                        for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                                            if ( json[x] == "valutazione" ) {
                                                valutazione = json[x + 1];
                                            }
                                            else if ( json[x] == "descrizione" ){
                                                descrizione = json[x + 1];
                                            }
                                            else{
                                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                                write(clientSocket, response.c_str(), response.length());
                                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                                close(clientSocket);
                                            }
                                        }
                                        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %d %s %s %s %s", 
                                            USERREAD, 
                                            "idSocket", clientSocket, 
                                            "operationId", operationId.c_str(), 
                                            "idUtente", idUtente, 
                                            "idItem", itemId, 
                                            "valutazione", valutazione.c_str(), 
                                            "descrizione", descrizione.c_str());
                                        freeReplyObject(reply);
                                    }
                                }
                                else if (tokens[count] == "favorite" && method == "PUT"){
                                    // passa al manager user

                                    std::string operationId = "favoriteItem";
                                    
                                    redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %d", 
                                        USERREAD, 
                                        "idSocket", clientSocket, 
                                        "operationId", operationId.c_str(), 
                                        "idUtente", idUtente, 
                                        "itemId", itemId);
                                    freeReplyObject(reply);
                                }
                                else {
                                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                    write(clientSocket, response.c_str(), response.length());
                                    db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                    close(clientSocket);
                                }
                            }  
                        }
                        else {
                            if (method == "DELETE"){
                                // passa al manager shop
                                
                                std::string operationId = "deeleteItem";

                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 24\r\n\r\nOperazione non esistente";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                            }
                            else {
                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                            }
                        }
                    }        
                    else if (tokens[count] == "payment"){
                        ++count;
                        int paymentid = std::stoi(tokens[count]);
                        if (count == tokenSize){
                            if(method == "DELETE"){
                                // passa al manager user
                                
                                // passa al manager user
                                std::string operationId = "removeMethodPaymentToUser";
                                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %d", 
                                    USERREAD, 
                                    "idSocket", clientSocket, 
                                    "operationId", operationId.c_str(), 
                                    "idUtente", idUtente, 
                                    "paymentId", paymentid);
                                freeReplyObject(reply);
                            }
                            else if (method == "PUT"){
                                // passa al manager user
                                std::string operationId = "addMethodPaymentToUser";
                                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %d", 
                                    USERREAD, 
                                    "idSocket", clientSocket, 
                                    "operationId", operationId.c_str(), 
                                    "idUtente", idUtente, 
                                    "paymentId", paymentid);
                                freeReplyObject(reply);
                            }
                            else{
                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                            }
                        }
                        else{
                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                            write(clientSocket, response.c_str(), response.length());
                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                            close(clientSocket);
                        }
                    }
                    else if (tokens[count] == "cart"){
                        ++count;
                        int cartItemId = std::stoi(tokens[count]);
                        if (count == tokenSize){
                            if(method == "PUT"){
                                // passa al manager user
                                
                                // Trova l'inizio del body JSON
                                std::string operationId = "cartItem";
                                char* bodyStart = strstr(buffer, "\r\n\r\n");
                                if (bodyStart == nullptr) {
                                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                                    write(clientSocket, response.c_str(), response.length());
                                    db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                    close(clientSocket);
                                }
                                bodyStart += 4; // Spostati oltre il doppio CRLF

                                std::string str(bodyStart);

                                std::string body = replaseString(str);

                                std::vector<std::string> loginUtente = splitStringJson(body);

                                    

                                if ( loginUtente.size() != 2){
                                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                    write(clientSocket, response.c_str(), response.length());
                                    db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                    close(clientSocket);
                                }
                                else{
                                    std::string quantita;
                                    if ( loginUtente[0] == "quantita" ) {
                                        quantita = loginUtente[1];
                                    }
                                    else{
                                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                        write(clientSocket, response.c_str(), response.length());
                                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                        close(clientSocket);
                                    }

                                    redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %d %s %s", 
                                        USERREAD, 
                                        "idSocket", clientSocket, 
                                        "operationId", operationId.c_str(), 
                                        "idUtente", idUtente, 
                                        "itemId", cartItemId, 
                                        "quantita", quantita.c_str());
                                    freeReplyObject(reply);
                                }
                            }
                            else if (method=="GET"){
                                // passa al manager user
                                
                                std::string operationId = "getMyCartPage";

                                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                                    USERREAD, 
                                    "idSocket", clientSocket, 
                                    "operationId", operationId.c_str(), 
                                    "idCart", cartItemId);
                                freeReplyObject(reply);
                            }
                            else if(method == "DELETE"){
                                // passa al manager user
                                
                                std::string operationId = "deleteCart";

                                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                                    USERREAD, 
                                    "idSocket", clientSocket, 
                                    "operationId", operationId.c_str(), 
                                    "itemId", cartItemId);
                                freeReplyObject(reply);                                
                            }
                            else{
                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                            }
                        }
                        else{
                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                            write(clientSocket, response.c_str(), response.length());
                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                            close(clientSocket);
                        }
                    }
                    else if (tokens[count] == "order"){
                        ++count;
                        int orderId = std::stoi(tokens[count]);
                        if (count != tokenSize){
                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                            write(clientSocket, response.c_str(), response.length());
                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                            close(clientSocket);
                        } else {
                            if ( method == "DELETE" ){
                                // passa al manager user

                                std::string operationId = "deleteOrder";
                                
                                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                                    USERREAD, 
                                    "idSocket", clientSocket, 
                                    "operationId", operationId.c_str(), 
                                    "idOrder", orderId);
                                freeReplyObject(reply);
                            }
                        }
                    }
                } else {
                    // profile, home page, favorite, payment, cart, item, categories
                    if (tokens[count] == "profile" && method=="GET"){
                        // passa al manager user
                        std::string operationId = "getUserProfile";
                        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                            USERREAD, 
                            "idSocket", clientSocket, 
                            "operationId", operationId.c_str(), 
                            "idUtente", idUtente);
                        freeReplyObject(reply);
                    }
                    else if (tokens[count] == "home_page" && method=="GET"){
                        // passa al manager user
                        std::string operationId = "getMyHomePage";
                        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s", 
                            USERREAD, 
                            "idSocket", clientSocket, 
                            "operationId", operationId.c_str());
                        freeReplyObject(reply);
                    }
                    else if (tokens[count] == "favorites" && method=="GET"){
                        // passa al manager user
                        
                        std::string operationId = "getMyFavoritePage";

                        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                            USERREAD, 
                            "idSocket", clientSocket, 
                            "operationId", operationId.c_str(), 
                            "idUtente", idUtente);
                        freeReplyObject(reply);
                    }
                    else if (tokens[count] == "payment" && method=="POST"){
                        // passa al manager user

                        std::string numero;
                        std::string cvv; 
                        std::string titolare; 
                        std::string scadenza;

                        // Trova l'inizio del body JSON
                        std::string operationId = "addPayment";
                        char* bodyStart = strstr(buffer, "\r\n\r\n");
                        if (bodyStart == nullptr) {
                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                            write(clientSocket, response.c_str(), response.length());
                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                            close(clientSocket);
                        }
                        bodyStart += 4; // Spostati oltre il doppio CRLF

                        std::string str(bodyStart);


                        std::string body = replaseString(str);

                        

                        std::vector<std::string> json = splitStringJson(body);


                        if( json.size() != 8){
                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                            write(clientSocket, response.c_str(), response.length());
                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                            close(clientSocket);
                        }
                        else{
                            for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                                if ( json[x] == "numero" ) {
                                    numero = json[x + 1];
                                }
                                else if ( json[x] == "cvv" ){
                                    cvv = json[x + 1];
                                }
                                else if ( json[x] == "titolare" ){
                                    titolare = json[x + 1];
                                }
                                else if ( json[x] == "scadenza" ){
                                    scadenza = json[x + 1];
                                }
                                else{
                                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                    write(clientSocket, response.c_str(), response.length());
                                    db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                    close(clientSocket);
                                }
                            }

                            redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %s %s %s %s %s %s %s", 
                                USERREAD, 
                                "idSocket", clientSocket, 
                                "operationId", operationId.c_str(), 
                                "idUtente", idUtente, 
                                "numero", numero.c_str(), 
                                "cvv", cvv.c_str(), 
                                "titolare", titolare.c_str(), 
                                "scadenza", scadenza.c_str());
                            freeReplyObject(reply);
                        }
                    }
                    else if ( tokens[count] == "order" ){
                        if ( method == "GET" ){
                                // passa al manager user

                                std::string operationId = "getOrder";

                                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %s %s %d", 
                                    USERREAD, 
                                    "miao", "world!", 
                                    "idSocket", clientSocket);
                                freeReplyObject(reply);
                            }
                        else if(method == "POST"){
                            // passa al manager user
                            
                            std::string operationId = "newOrder";
                            char* bodyStart = strstr(buffer, "\r\n\r\n");
                            if (bodyStart == nullptr) {
                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                            }
                            bodyStart += 4; // Spostati oltre il doppio CRLF

                            std::string str(bodyStart);


                            std::string body = replaseString(str);

                            

                            std::vector<std::string> json = splitStringJson(body);


                            if( json.size() != 4){
                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                            }
                            else{
                                std::string stato;
                                std::string idCarta;

                                for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                                    if ( json[x] == "stato" ) {
                                        stato = json[x + 1];
                                    }
                                    else if ( json[x] == "idCarta" ){
                                        idCarta = json[x + 1];
                                    }
                                    else{
                                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                        write(clientSocket, response.c_str(), response.length());
                                        close(clientSocket);
                                    }
                                }
                                redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %s %s %s", 
                                    USERREAD, 
                                    "clientSocket", clientSocket, 
                                    "operazionId", operationId.c_str(), 
                                    "idNegozio", idUtente,
                                    "stato", stato.c_str(), 
                                    "idCarta", idCarta.c_str());

                                freeReplyObject(reply);
                            }
                        }
                        else {
                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                            write(clientSocket, response.c_str(), response.length());
                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                            close(clientSocket);
                        }
                    }
                    else {
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                        write(clientSocket, response.c_str(), response.length());
                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                        close(clientSocket);
                    }
                }
            }
            else if (tokens[count] == "categories" && method=="GET"){
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nL'operazione non esiste";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
            else{
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
        }
        else if ( tokens[count] == "shop" ){
            ++count;
            if (count != tokenSize){
                int idShop = std::stoi(tokens[count]);
                ++count;
                if ( tokens[count] == "order" ){
                    if ( tokenSize == count ){
                        if ( method == "GET" ){
                            // passa al manager shop

                            std::string operationId = "getOrdineShop";

                            redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                                SHOPREAD, 
                                "idSocket", clientSocket, 
                                "operationId", operationId.c_str(), 
                                "idShop", idShop);
                            freeReplyObject(reply);
                        }
                    }
                    else {
                        ++count;
                        if ( tokenSize != count ){
                            int orderId = std::stoi(tokens[count]);
                            ++count;
                            if (tokens[count] == "item"){
                                ++count;
                                if (count == tokenSize){
                                    int itemId = std::stoi(tokens[count]);
                                    if (method == "PUT"){
                                        // passa al manager user

                                        std::string operationId = "addItemOrder";

                                        char* bodyStart = strstr(buffer, "\r\n\r\n");
                                        if (bodyStart == nullptr) {
                                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                                            write(clientSocket, response.c_str(), response.length());
                                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                            close(clientSocket);
                                        }
                                        bodyStart += 4; // Spostati oltre il doppio CRLF

                                        std::string str(bodyStart);


                                        std::string body = replaseString(str);

                                        

                                        std::vector<std::string> json = splitStringJson(body);

                                        std::string quantita;

                                        if( json.size() != 2){
                                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                            write(clientSocket, response.c_str(), response.length());
                                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                            close(clientSocket);
                                        }
                                        else{
                                            if ( json[0] == "quantita" ) {
                                                quantita = json[1];
                                            }
                                            else{
                                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                                write(clientSocket, response.c_str(), response.length());
                                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                                close(clientSocket);
                                            }

                                            redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %d %s %s", 
                                                USERREAD, 
                                                "idSocket", clientSocket, 
                                                "operationId", operationId.c_str(), 
                                                "orderID", orderId, 
                                                "itemId", itemId, 
                                                "quantita", quantita.c_str());
                                            freeReplyObject(reply);
                                        }
                                    }
                                    else if (method == "DELETE"){
                                        // passa al manager user

                                        std::string operationId = "removeItemOrder";
                                        // da finire perché manca la parte del db
                                        
                                        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %s %s %d", 
                                            USERREAD, 
                                            "idSocket", clientSocket, 
                                            "operationId", operationId.c_str());
                                        freeReplyObject(reply);
                                    }
                                    else{
                                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                        write(clientSocket, response.c_str(), response.length());
                                        db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                        close(clientSocket);
                                    }
                                }
                                else{
                                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                                    write(clientSocket, response.c_str(), response.length());
                                    db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                    close(clientSocket);
                                }
                            }
                        }
                    }
                }
                else if (tokens[count] == "item" && method=="POST"){
                    // passa al manager shop

                    std::string operationId = "addItem";
                    char* bodyStart = strstr(buffer, "\r\n\r\n");
                    if (bodyStart == nullptr) {
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                    }
                    bodyStart += 4; // Spostati oltre il doppio CRLF

                    std::string str(bodyStart);


                    std::string body = replaseString(str);

                    

                    std::vector<std::string> json = splitStringJson(body);


                    if( json.size() != 24){
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                    }
                    else{
                        std::string categoria;
                        std::string nome;
                        std::string modello;
                        std::string marca;
                        std::string descrizione;
                        std::string disponibilita;
                        std::string prezzo;
                        std::string pubblicazione;
                        std::string offerta;
                        std::string inizio_offerta;
                        std::string percentuale;
                        std::string durata_ore;

                        for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                            if ( json[x] == "nome" ) {
                                nome = json[x + 1];
                            }
                            else if ( json[x] == "categoria" ){
                                categoria = json[x + 1];
                            }
                            else if ( json[x] == "modello" ){
                                modello = json[x + 1];
                            }
                            else if ( json[x] == "marca" ){
                                marca = json[x + 1];
                            }
                            else if ( json[x] == "descrizione" ){
                                descrizione = json[x + 1];
                            }
                            else if ( json[x] == "disponibilita" ){
                                disponibilita = json[x + 1];
                            }
                            else if ( json[x] == "prezzo" ){
                                prezzo = json[x + 1].c_str();
                            }
                            else if ( json[x] == "pubblicazione" ){
                                pubblicazione = json[x + 1];
                            }
                            else if ( json[x] == "offerta" ){
                                offerta = json[x + 1];
                            }
                            else if ( json[x] == "inizio_offerta" ){
                                inizio_offerta = json[x + 1];
                            }
                            else if ( json[x] == "percentuale" ){
                                percentuale = json[x + 1];
                            }
                            else if ( json[x] == "durata_ore" ){
                                durata_ore = json[x + 1];
                            }
                            else{
                                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                                write(clientSocket, response.c_str(), response.length());
                                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                                close(clientSocket);
                            }
                        }
                        redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", 
                            SHOPREAD, 
                            "clientSocket", clientSocket, 
                            "operazionId", operationId.c_str(), 
                            "idNegozio", idShop,
                            "nome", nome.c_str(), 
                            "modello", modello.c_str(), 
                            "marca", marca.c_str(),
                            "descrizione",  descrizione.c_str(),
                            "disponibilità", disponibilita.c_str(), 
                            "prezzo",  prezzo.c_str(),
                            "pubblicazione", pubblicazione.c_str(), 
                            "offerta", offerta.c_str(),
                            "inizio_offerta", inizio_offerta.c_str(),
                            "percentuale", percentuale.c_str(),
                            "durata_ore", durata_ore.c_str(), 
                            "categoria", categoria.c_str());

                        freeReplyObject(reply);
                    }
                }
            }
            else if (tokens[count] == "categories" && method=="POST"){
                // passa al manager user

                std::string operationId = "newCategory";
                char* bodyStart = strstr(buffer, "\r\n\r\n");
                if (bodyStart == nullptr) {
                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non presente";
                    write(clientSocket, response.c_str(), response.length());
                    db.update_request_log(RequestLog(0, clientSocket, "", ""));
                    close(clientSocket);
                }
                bodyStart += 4; // Spostati oltre il doppio CRLF

                std::string str(bodyStart);


                std::string body = replaseString(str);

                

                std::vector<std::string> json = splitStringJson(body);


                if( json.size() != 4){
                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                    write(clientSocket, response.c_str(), response.length());
                    db.update_request_log(RequestLog(0, clientSocket, "", ""));
                    close(clientSocket);
                }
                else{
                    std::string nome;
                    std::string padre;

                    for(int x = 0; x < (json.size() - 1); x = x + 2 ){
                        if ( json[x] == "nome" ) {
                            nome = json[x + 1];
                        }
                        else if ( json[x] == "padre" ){
                            padre = json[x + 1];
                        }
                        else{
                            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nBody non corretto";
                            write(clientSocket, response.c_str(), response.length());
                            db.update_request_log(RequestLog(0, clientSocket, "", ""));
                            close(clientSocket);
                        }
                    }
                    redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %s %s %s", 
                        SHOPREAD, 
                        "clientSocket", clientSocket, 
                        "operazionId", operationId.c_str(), 
                        "nome", nome.c_str(),
                        "padre", padre.c_str());

                    freeReplyObject(reply);
                }
            }
            
            else{
                std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
                write(clientSocket, response.c_str(), response.length());
                db.update_request_log(RequestLog(0, clientSocket, "", ""));
                close(clientSocket);
            }
        }
        else if ( tokens[count] == "delivery" ){
            ++count;
            int idCourier = std::stoi(tokens[count]);
            ++count;
            if (tokens[count] == "order"){
                if ( tokenSize == count ){
                    if ( method == "GET" ){
                        // passa al manager delivery

                            std::string operationId = "getOrdineCourier";
                            
                            redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d", 
                                DELIVERYREAD, 
                                "idSocket", clientSocket, 
                                "operationId", operationId.c_str(), 
                                "idCourier", idCourier);
                            freeReplyObject(reply);
                    }
                }
                else{
                    ++count;
                    if ( tokenSize == count ){
                        int orderId = std::stoi(tokens[count]);
                        
                        if ( method == "PUT" ){
                            // passa al manager delivery

                            std::string operationId = "addItemOrderCourier";
                            redisReply *reply = (redisReply*)redisCommand(redis, "XADD %s * %s %d %s %s %s %d %s %d", 
                                DELIVERYREAD, 
                                "idSocket", clientSocket, 
                                "operationId", operationId.c_str(), 
                                "idCourier", idCourier, 
                                "idOrder", orderId);
                            freeReplyObject(reply);
                        }
                    }
                }
            }
        }
        
        else if (path == "/stop"){
            std::cout << "Chiusura del server web in corso.. " << method << " " << path << std::endl;
        }

        else{
            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPath not valid";
            write(clientSocket, response.c_str(), response.length());
            db.update_request_log(RequestLog(0, clientSocket, "", ""));
            close(clientSocket);
        }
        
    }
    this->stop();

}

void HttpServer::stop() {
    // Chiude il socket del server
    if (serverSocket != -1) {
        close(serverSocket);
    }

}