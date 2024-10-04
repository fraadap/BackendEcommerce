import random
import string

def genera_nome_casuale():
    # Lista di nomi casuali
    nomi = ['Alice', 'Bob', 'Charlie', 'David', 'Emma', 'Frank', 'Grace', 'Henry', 'Isabella', 'Jack', 'Katie', 'Liam', 'Mia', 'Noah', 'Olivia', 'Peter', 'Quinn', 'Ryan', 'Sophia', 'Thomas', 'Uma', 'Victoria', 'William', 'Xavier', 'Yvonne', 'Zoe']
    
    # Scegli un nome casuale dalla lista
    nome_casuale = random.choice(nomi)
    
    # Restituisci il nome casuale
    return nome_casuale

def genera_cap_casuale():
    cap = ''
    for _ in range(5):
        cap += str(random.randint(0, 9))  # Genera una cifra casuale da 0 a 9 e la concatena a cap
    return cap

def genera_numeri_casuali():
    numero_casuale = ''
    for _ in range(3):  # Genera tre cifre casuali
        numero_casuale += str(random.randint(0, 9))  # Aggiunge una cifra casuale alla stringa
    return numero_casuale

def genera_mail_casuale():
    parte_nome = ''.join(random.choices(string.ascii_lowercase, k=8))  # Parte casuale del nome (8 caratteri)
    dominio = '@example.com'  # Dominio predefinito (può essere cambiato)
    return parte_nome + dominio

def genera_nazione_casuale():
    nazioni = ['Italia', 'Francia', 'Germania', 'Spagna', 'Regno Unito', 'Stati Uniti', 'Canada', 'Giappone', 'Cina', 'Australia']
    return random.choice(nazioni)

def genera_citta_casuale():
    citta = ['Roma', 'Parigi', 'Berlino', 'Madrid', 'Londra', 'New York', 'Toronto', 'Tokyo', 'Pechino', 'Sydney']
    return random.choice(citta)

def genera_data_casuale():
    # Genera un anno casuale tra il 1970 e il 2022
    anno = random.randint(1970, 2022)
    
    # Genera un mese casuale tra 1 e 12
    mese = random.randint(1, 12)
    
    # Determina il numero massimo di giorni in base al mese e all'anno
    giorni_nel_mese = 31
    if mese in [4, 6, 9, 11]:
        giorni_nel_mese = 30
    elif mese == 2:
        # Gestione degli anni bisestili
        giorni_nel_mese = 29 if anno % 4 == 0 and (anno % 100 != 0 or anno % 400 == 0) else 28
    
    # Genera un giorno casuale tra 1 e il numero massimo di giorni nel mese
    giorno = random.randint(1, giorni_nel_mese)
    
    # Formatta la data nel formato "anno-mese-giorno"
    data_casuale = f"{anno}-{mese:02d}-{giorno:02d}"
    
    return data_casuale

def genera_numero_telefono_casuale():
    numero_telefono = '+39 '  # Prefisso internazionale italiano
    
    # Genera i primi 3 numeri del prefisso
    for _ in range(3):
        numero_telefono += str(random.randint(0, 9))
    
    numero_telefono += ' '  # Aggiunge uno spazio dopo il prefisso
    
    # Genera i restanti 8 numeri del numero di telefono
    for _ in range(8):
        numero_telefono += str(random.randint(0, 9))
    
    return numero_telefono

def genera_via_casuale():
    nomi_strade = ['Via Roma', 'Via Garibaldi', 'Corso Italia', 'Via Vittorio Emanuele', 'Via Leonardo da Vinci', 'Via Dante', 'Corso Vittorio Emanuele II', 'Via Giuseppe Mazzini', 'Via Manzoni', 'Corso Matteotti']
    nome_strada = random.choice(nomi_strade)  # Sceglie casualmente un nome di strada dalla lista
    return nome_strada

def genera_otto_numeri_casuali():
    numero_casuale = ''
    for _ in range(8):  # Genera tre cifre casuali
        numero_casuale += str(random.randint(0, 9))  # Aggiunge una cifra casuale alla stringa
    return numero_casuale

def genera_futura_data_casuale():
    # Genera un anno casuale tra il 1970 e il 2022
    anno = random.randint(2025, 2030)
    
    # Genera un mese casuale tra 1 e 12
    mese = random.randint(1, 12)
    
    # Determina il numero massimo di giorni in base al mese e all'anno
    giorni_nel_mese = 31
    if mese in [4, 6, 9, 11]:
        giorni_nel_mese = 30
    elif mese == 2:
        # Gestione degli anni bisestili
        giorni_nel_mese = 29 if anno % 4 == 0 and (anno % 100 != 0 or anno % 400 == 0) else 28
    
    # Genera un giorno casuale tra 1 e il numero massimo di giorni nel mese
    giorno = random.randint(1, giorni_nel_mese)
    
    # Formatta la data nel formato "anno-mese-giorno"
    data_casuale = f"{anno}-{mese:02d}-{giorno:02d}"
    
    return data_casuale

def genera_categoria_casuale():
    categorie = [
        "Elettronica",
        "Abbigliamento",
        "Casa e Cucina",
        "Libri",
        "Giocattoli",
        "Sport e Tempo Libero",
        "Bellezza e Salute",
        "Auto e Moto",
        "Giardino e Fai da Te",
        "Informatica"
    ]
    return random.choice(categorie)

def genera_nome_articolo():
    parole1 = ['Maglia', 'Camicia', 'Pantaloni', 'Gonna', 'Vestito', 'Giacca', 'Cappotto', 'Felpa', 'Tuta', 'Jeans']
    parole2 = ['alla moda', 'elegante', 'sportivo', 'casuale', 'di lusso', 'colorato', 'moderno', 'vintage', 'alla moda', 'comodo']

    nome = random.choice(parole1) + ' ' + random.choice(parole2)
    return nome

def genera_marca():
    marche = ['Adidas', 'Nike', 'Puma', 'Gucci', 'Calvin Klein', 'Tommy Hilfiger', 'Zara', 'H&M', 'Armani', 'Levi\'s']

    marca = random.choice(marche)
    return marca

def genera_descrizione():
    colori = ['nero', 'blu', 'rosso', 'verde', 'bianco', 'giallo', 'rosa', 'grigio', 'viola', 'arancione']
    materiali = ['cotone', 'lana', 'seta', 'pelle', 'poliestere', 'denim', 'nylon', 'velluto', 'cashmere', 'satin']
    dettagli = ['con bottoni', 'con cerniera', 'con tasche', 'a righe', 'a pois', 'a quadri', 'con colletto', 'senza maniche', 'con cappuccio', 'con stampa']

    colore = random.choice(colori)
    materiale = random.choice(materiali)
    dettaglio = random.choice(dettagli)

    descrizione = f"Un articolo {colore} in {materiale} {dettaglio}"
    return descrizione

def stato_consegna():
    stati = [
        "Pacco spedito",
        "In transito",
        "Arrivato al centro di smistamento",
        "In consegna",
        "Consegnato",
        "Tentativo di consegna fallito",
        "Pacco restituito al mittente"
    ]
    return random.choice(stati)
