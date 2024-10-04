#include "database.h"
#include <cstring>

char* Database::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch().count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(value));

    std::time_t time = seconds.count();
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    char* timestamp = new char[strlen(buffer) + 1]; // Allocazione dinamica della memoria per il timestamp
    strcpy(timestamp, buffer); // Copia il contenuto del buffer nel nuovo array di char
    return timestamp;
}