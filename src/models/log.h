#ifndef LOG_H
#define LOG_H
#include <cstring>

class Log {
    public:
        // COSTRUTTORE
        Log(){}
        Log( int id,  const char* timestamp, const char* tag, const char* info):id(id), 
        timestamp(strdup(timestamp)), tag(strdup(tag)), info(strdup(info)){}

    int getId() const {return id;}
    void setId(int id){this->id = id;}
    char* getTimestamp() const {return timestamp;}
    char* getTag() const {return tag;}
    char* getInfo() const {return info;}

    private:
        int id;
        char* timestamp;
        char* tag;
        char* info;
};

#endif