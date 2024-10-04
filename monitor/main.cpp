#include "../src/db/database.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>


// Funzione per il parsing dei timestamp con microsecondi
std::chrono::system_clock::time_point parseTimestamp(const std::string& ts) {
    std::tm t = {};
    std::chrono::microseconds us;
    std::istringstream ss(ts);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    if (ss.peek() == '.') {
        ss.ignore();
        int microseconds;
        ss >> microseconds;
        us = std::chrono::microseconds(microseconds);
    }
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&t));
    tp += us;
    return tp;
}

int main() {
    // Inizializzazione della connessione al database
    Con2DB con("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    Database db(&con);

    try {

        int input;
        std::string inizio, fine;

        // Richiesta input per la selezione del tipo di errore
        std::cout << "Inserire:\n 0 per errori del server\n "
        "1 per errori dei manager\n 2 per entrambi gli errori\n 3 per le statistiche relative ai tempi delle richieste\n\n";
        std::cin >> input;

        // Richiesta delle date di inizio e fine
        std::cout << "\nInserire la data di inizio (YYYY-MM-DD): ";
        std::getline(std::cin >> std::ws, inizio); 
        std::cout << "\nInserire la data di fine (YYYY-MM-DD): "; 
        std::getline(std::cin >> std::ws, fine);

        const char* c_inizio = inizio.c_str();
        const char* c_fine = fine.c_str();

        if (input != 3){
            std::vector<Log> logs;
            std::string tag;
            if (input == 0) {
                tag = "Server_error";
            } else if (input == 1) {
                tag = "Manager_error";
            } else {
                tag = "";
            }
            // Ottenere i log dal database
            logs = db.get_log(c_inizio, c_fine, tag.c_str());
            // Visualizzazione dei log
            for (const Log& log : logs) {
                printf("\nLog: %d, time %s, tag: %s, info: %s",
                    log.getId(), log.getTimestamp(), log.getTag(), log.getInfo());
            }
        } else{
            std::vector<RequestLog> logs;
            // Ottenere i request log dal database
            logs = db.get_request_log(c_inizio, c_fine);

            double sum = 0.0;
            double min = std::numeric_limits<double>::max();
            double max = std::numeric_limits<double>::lowest();
            int count = 0;
            for (const RequestLog& log : logs) {
                 std::string start = log.getInizio();
                std::string end = log.getFine();
                auto start_tp = parseTimestamp(start);
                auto end_tp = parseTimestamp(end);
                
                std::chrono::duration<double> duration = end_tp - start_tp;
                double duration_seconds = duration.count();

                if (duration_seconds<0){
                    duration_seconds*=-1;
                }
                std::cout << "LogID: "<< log.getId() << " client socket: "<< log.getFd()<< 
                " Durata della richiesta in secondi: " << duration_seconds << "s\n";
                sum += duration_seconds;
                min = std::min(min, duration_seconds);
                max = std::max(max, duration_seconds);
                ++count;
                

            }

            if (count > 0) {
                double average = sum / count;
                std::cout << "\n\nNumero di log: " << count << "\n";
                std::cout << "Media dei secondi: " << average << "s\n";
                std::cout << "Minimo dei secondi: " << min << "s\n";
                std::cout << "Massimo dei secondi: " << max << "s\n";
            } else {
                std::cout << "Nessun log da processare.\n";
            }

        }
    } catch (const std::exception& e) {
        std::cout << "Errore: " << e.what() << "\n";
    }

    return 0;
}
