
-- database already exists

\c :dbname 


-- Pick the one most appropriate for your application
-- CREATE DOMAIN TimePoint AS date ;
create domain TimePoint as timestamp ;
-- create domain TimePoint as timestamp with time zone;

CREATE DOMAIN  String4VarName AS VARCHAR(30) ;
CREATE DOMAIN  String4Info as VARCHAR(200) ;

-- Pick the one most appropriate for your application
--CREATE DOMAIN VarType AS real ;
create domain VarType as double precision ;
-- create domain VarType as int ;


CREATE TYPE DomainType AS ENUM ('discrete', 'real', 'double');


-- Timescaledb extension for time series
-- Do not forget to create timescaledb extension
-- CREATE EXTENSION timescaledb;

CREATE TABLE IF NOT EXISTS LogTable (
        ID SERIAL PRIMARY KEY,
        Timestamp TIMESTAMP NOT NULL,		-- nanseconds after second
        Tag VARCHAR(20) NOT NULL,
        Info VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS RequestLogTable (
        ID SERIAL PRIMARY KEY,
        Fd INTEGER,
        Inizio TIMESTAMP,	
        Fine TIMESTAMP
);

-- INIZIO DELLE NOSTRE TABELLE
CREATE TYPE OrderStatus AS ENUM (
    'Consegnato', 'In consegna', 'In transito', 'Spedito', 'In elaborazione', 'Confermato', 'Rifiutato', 'Errore'
);

CREATE TABLE IF NOT EXISTS Nazione (
    Nome VARCHAR(255) PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS Citta (
    ID SERIAL PRIMARY KEY,
    Nome VARCHAR(255) NOT NULL,
    Nazione VARCHAR(255) NOT NULL,
    FOREIGN KEY (Nazione) REFERENCES Nazione(Nome),
    UNIQUE (Nome, Nazione)
);

CREATE TABLE IF NOT EXISTS CAP (
    Codice CHAR(5) PRIMARY KEY,
    Citta INT NOT NULL,
    FOREIGN KEY (Citta) REFERENCES Citta(ID)
);

CREATE TABLE IF NOT EXISTS Indirizzo (
    ID SERIAL PRIMARY KEY,
    Via VARCHAR(30) NOT NULL,
    Civico VARCHAR(10) NOT NULL,
    Interno INT,
    Zona CHAR(5) NOT NULL,
    FOREIGN KEY (Zona) REFERENCES CAP(Codice),
    UNIQUE(Via, Civico, Interno, Zona)
);

CREATE TABLE IF NOT EXISTS Metodo_pagamento (
    ID SERIAL PRIMARY KEY,
    Numero VARCHAR(40) NOT NULL UNIQUE,
    Titolare VARCHAR(100) NOT NULL,
    Scadenza DATE NOT NULL
);

CREATE TABLE IF NOT EXISTS Utente (
    ID SERIAL PRIMARY KEY,
    Username VARCHAR(20) NOT NULL UNIQUE,
    Nome VARCHAR(20) NOT NULL,
    Cognome VARCHAR(20) NOT NULL,
    Registrazione TIMESTAMP NOT NULL,
    Email TEXT NOT NULL,
    Indirizzo INT NOT NULL,
    Pass TEXT NOT NULL,
    FOREIGN KEY (Indirizzo) REFERENCES Indirizzo(ID)
);

CREATE TABLE IF NOT EXISTS Negozio (
    ID SERIAL PRIMARY KEY,
    Nome VARCHAR(20) NOT NULL UNIQUE,
    Telefono VARCHAR(15) NOT NULL,
    Email TEXT NOT NULL,
    Indirizzo INT NOT NULL,
    Metodo_pagamento INT NOT NULL, 
    Pass TEXT NOT NULL,
    FOREIGN KEY (Indirizzo) REFERENCES Indirizzo(ID),
    FOREIGN KEY (Metodo_pagamento) REFERENCES Metodo_pagamento(ID)
);

CREATE TABLE IF NOT EXISTS Utente_metodo (
    ID SERIAL PRIMARY KEY,
    Utente INT NOT NULL,
    Metodo_pagamento INT NOT NULL,
    Aggiunta TIMESTAMP NOT NULL,
    FOREIGN KEY (Utente) REFERENCES Utente(ID),
    FOREIGN KEY (Metodo_pagamento) REFERENCES Metodo_pagamento(ID)
);

CREATE TABLE IF NOT EXISTS Categoria (
    Nome VARCHAR(255) PRIMARY KEY NOT NULL,
    Categoria_padre VARCHAR(255),
    FOREIGN KEY (Categoria_padre) REFERENCES Categoria(Nome)
);

CREATE TABLE IF NOT EXISTS Articolo (
    ID SERIAL PRIMARY KEY,
    Nome VARCHAR(255) NOT NULL,
    Modello VARCHAR(255) NOT NULL,
    Marca VARCHAR(255) NOT NULL,
    Descrizione VARCHAR(255) NOT NULL,
    Disponibilita INT NOT NULL CHECK (Disponibilita >= 0),
    Prezzo NUMERIC(10, 2) NOT NULL CHECK (Prezzo > 0),
    Pubblicazione TIMESTAMP,
    Offerta BOOLEAN NOT NULL,
    Inizio_offerta TIMESTAMP,
    Percentuale_sconto NUMERIC(5, 2) CHECK (Percentuale_sconto >= 0 AND Percentuale_sconto <= 100),
    Durata_ore INT,
    Categoria VARCHAR(255) NOT NULL,
    Negozio INT NOT NULL,
    FOREIGN KEY (Categoria) REFERENCES Categoria(Nome),
    FOREIGN KEY (Negozio) REFERENCES Negozio(ID),
    CHECK(
        (Offerta = TRUE AND Inizio_offerta IS NOT NULL AND Percentuale_sconto IS NOT NULL AND Durata_ore IS NOT NULL) OR
        (Offerta = FALSE AND Inizio_offerta IS NULL AND Percentuale_sconto IS NULL AND Durata_ore IS NULL)
    )
);

CREATE TABLE IF NOT EXISTS Spesa_spedizione (
    ID SERIAL PRIMARY KEY,
    Tipologia VARCHAR(255) NOT NULL,
    Prezzo NUMERIC(10, 2) NOT NULL CHECK (Prezzo > 0),
    Articolo INT NOT NULL,
    FOREIGN KEY (Articolo) REFERENCES Articolo(ID),
    UNIQUE (Tipologia, Articolo)
);

CREATE TABLE IF NOT EXISTS Carrello (
    ID SERIAL PRIMARY KEY,
    Utente INT NOT NULL,
    Articolo INT NOT NULL,
    Aggiunta TIMESTAMP NOT NULL,
    Quantita INT NOT NULL CHECK (Quantita >= 0),
    FOREIGN KEY (Utente) REFERENCES Utente(ID),
    FOREIGN KEY (Articolo) REFERENCES Articolo(ID)
);

CREATE TABLE IF NOT EXISTS Lista_desideri (
    ID SERIAL PRIMARY KEY,
    Utente INT NOT NULL,
    Articolo INT NOT NULL,
    Aggiunta TIMESTAMP NOT NULL,
    FOREIGN KEY (Utente) REFERENCES Utente(ID),
    FOREIGN KEY (Articolo) REFERENCES Articolo(ID)
);

CREATE TABLE IF NOT EXISTS Ordine (
    ID SERIAL PRIMARY KEY,
    Istante TIMESTAMP NOT NULL,
    OrderStatus VARCHAR(255) NOT NULL,
    Utente INT NOT NULL,
    Metodo_pagamento INT NOT NULL,
    FOREIGN KEY (Utente) REFERENCES Utente(ID),
    FOREIGN KEY (Metodo_pagamento) REFERENCES Metodo_pagamento(ID)
);

CREATE TABLE IF NOT EXISTS Corriere (
    ID SERIAL PRIMARY KEY,
    Nome VARCHAR(30) NOT NULL,
    Cognome VARCHAR(30) NOT NULL
);

CREATE TABLE IF NOT EXISTS Ordine_corriere (
    ID SERIAL PRIMARY KEY,
    Ordine INT NOT NULL,
    Corriere INT NOT NULL,
    Inizio TIMESTAMP NOT NULL,
    Fine TIMESTAMP,
    FOREIGN KEY (Ordine) REFERENCES Ordine(ID),
    FOREIGN KEY (Corriere) REFERENCES Corriere(ID),
    CHECK(Fine IS NULL OR Inizio > Fine)
);

CREATE TABLE IF NOT EXISTS Articolo_ordine (
    ID SERIAL PRIMARY KEY,
    Ordine INT NOT NULL,
    Articolo INT NOT NULL,
    Quantita INT NOT NULL CHECK (Quantita > 0),
    FOREIGN KEY (Ordine) REFERENCES Ordine(ID),
    FOREIGN KEY (Articolo) REFERENCES Articolo(ID)
);

CREATE TABLE IF NOT EXISTS Recensione (
    ID SERIAL PRIMARY KEY,
    Utente INT NOT NULL,
    Articolo INT NOT NULL,
    Aggiunta TIMESTAMP NOT NULL,
    Valutazione INT NOT NULL CHECK (Valutazione >= 0 AND Valutazione <= 5),
    Descrizione TEXT,
    FOREIGN KEY (Utente) REFERENCES Utente(ID),
    FOREIGN KEY (Articolo) REFERENCES Articolo(ID),
    UNIQUE(Utente, Articolo)
);

CREATE TABLE IF NOT EXISTS Admin (
    ID SERIAL PRIMARY KEY,
    Nome VARCHAR(255) NOT NULL UNIQUE,
    Pass TEXT NOT NULL
);
