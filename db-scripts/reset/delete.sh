# Questo file elimina tutte le tabelle e il loro contenuto per resettare il database

sudo -u postgres psql postgres -f deleteAll.sql