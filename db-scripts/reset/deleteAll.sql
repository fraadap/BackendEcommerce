\c db_ecommerce postgres

BEGIN;
-- Disabilita il controllo sulle chiavi esterne
SET CONSTRAINTS ALL DEFERRED;

-- Cancella tutte le tabelle del database
DROP TABLE IF EXISTS LogTable CASCADE;
DROP TABLE IF EXISTS Articolo_ordine CASCADE;
DROP TABLE IF EXISTS Ordine_corriere CASCADE;
DROP TABLE IF EXISTS Corriere CASCADE;
DROP TABLE IF EXISTS Ordine CASCADE;
DROP TABLE IF EXISTS Lista_desideri CASCADE;
DROP TABLE IF EXISTS Carrello CASCADE;
DROP TABLE IF EXISTS Spesa_spedizione CASCADE;
DROP TABLE IF EXISTS Articolo CASCADE;
DROP TABLE IF EXISTS Categoria CASCADE;
DROP TABLE IF EXISTS Utente_metodo CASCADE;
DROP TABLE IF EXISTS Negozio CASCADE;
DROP TABLE IF EXISTS Utente CASCADE;
DROP TABLE IF EXISTS Metodo_pagamento CASCADE;
DROP TABLE IF EXISTS Indirizzo CASCADE;
DROP TABLE IF EXISTS CAP CASCADE;
DROP TABLE IF EXISTS Citta CASCADE;
DROP TABLE IF EXISTS Nazione CASCADE;
DROP TABLE IF EXISTS Recensione CASCADE;
DROP TABLE IF EXISTS Admin CASCADE;

-- Riattiva immediatamente tutti i controlli sulle chiavi esterne
SET CONSTRAINTS ALL IMMEDIATE;

COMMIT;



