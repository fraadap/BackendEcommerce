-- Creazione del database
DROP DATABASE IF EXISTS :dbname;
CREATE DATABASE :dbname;

-- Creazione dell'utente
REASSIGN OWNED BY :username TO postgres ;
REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :username ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :username ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :username ;
DROP OWNED BY :username;
DROP ROLE IF EXISTS :username;
CREATE USER :username WITH ENCRYPTED PASSWORD '47002';

-- Assegnazione dei permessi al database
GRANT ALL PRIVILEGES ON DATABASE :dbname TO :username;

-- Connettersi al database appena creato
\c :dbname postgres

-- Assegnazione dei permessi sullo schema pubblico
GRANT ALL ON SCHEMA public TO :username ;

-- Assegnazione dei permessi sulle tabelle
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :username;

GRANT ALL PRIVILEGES ON DATABASE :dbname to :username ;
