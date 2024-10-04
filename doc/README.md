# Istruzioni per la compilazione del codice

Posizionarsi dentro la cartella principale del progetto e digitare make

# Istruzioni per l'esecuzione del server

Avviare il server redis digitando il comando redis-server in un altro terminale e dopodiché eseguire il comando ./main per avviare il programma.

# Istruzione per la creazione del DB

posizionarsi dentro la cartella ./db-script ed eseguire il file create.sh

# Istruzioni per la compilazione del monitor

Posizionarsi dentro la cartella ./monitor del progetto e digitare make

# Istruzioni per l'utilizzo del monitor

Avviare il monitor con il comando ./monitor/main, dopodiché selezionare la tipologia di log da controllare. Infine specificare un lasso di tempo di questi log in formato HH-MM-DD

# Istruzioni per l'uso del test generator

Una volta aver avviato il server, digitare il comando "python3 ./testGenerator/test.py". Esso genera 1000 richieste casuali ulizzando dati verosimili riempendo il db e ricercando in esso.
