#ifndef REQUESTLOG_H
#define REQUESTLOG_H
#include <cstring>

class RequestLog {
    public:
        // COSTRUTTORE
        RequestLog(){}
        RequestLog( int id, int fd, const char* inizio, const char* fine):id(id), 
        fd(fd), inizio(strdup(inizio)), fine(strdup(fine)){}

    int getId() const {return id;}
    void setId(int id) {this->id = id;}
    int getFd() const {return fd;}
    char* getInizio() const {return inizio;}
    char* getFine() const {return fine;}

    private:
        int id;
        int fd;
        char* inizio;
        char* fine;
};

#endif