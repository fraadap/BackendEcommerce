\c :dbname postgres

-- user already exists
GRANT ALL PRIVILEGES ON DATABASE :dbname to :username ;


ALTER TABLE TimeVar OWNER TO :username ;
ALTER TABLE LogTable OWNER TO :username ;
ALTER TABLE Nazione OWNER TO :username ;
ALTER TABLE Citta OWNER TO :username ;
ALTER TABLE CAP OWNER TO :username ;
ALTER TABLE Indirizzo OWNER TO :username ;
ALTER TABLE Metodo_pagamento OWNER TO :username ;
ALTER TABLE Utente OWNER TO :username ;
ALTER TABLE Negozio OWNER TO :username ;
ALTER TABLE Utente_metodo OWNER TO :username ;
ALTER TABLE Categoria OWNER TO :username ;
ALTER TABLE Articolo OWNER TO :username ;
ALTER TABLE Spesa_spedizione OWNER TO :username ;
ALTER TABLE Carrello OWNER TO :username ;
ALTER TABLE Lista_desideri OWNER TO :username ;
ALTER TABLE Ordine OWNER TO :username ;
ALTER TABLE Corriere OWNER TO :username ;
ALTER TABLE Ordine_corriere OWNER TO :username ;ALTER TABLE Spesa_spedizione OWNER TO :username ;
ALTER TABLE Articolo_ordine OWNER TO :username ;
ALTER TABLE Recensione OWNER TO :username ;
ALTER TABLE LogTable OWNER TO :username ;
ALTER TABLE RequestLogTable OWNER TO :username ;
ALTER TABLE Admin OWNER TO :username ;

-- grant all privileges on all tables in schema public to :username ;
-- grant all privileges on all sequences in schema public to :username ;

GRANT ALL ON SCHEMA public TO :username ;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :username ;
