#include "httpServer.h"
#include <hiredis/hiredis.h>


int main(){ 
    // controllo della presenza delle tabelle nel DB e funzionamento

    // avvio del server
    HttpServer(8080).start();;
    

    return 0;
}