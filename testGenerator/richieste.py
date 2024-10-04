import requests
import json
import utils
import random

def doLoginUser(path):

    # Dati da inviare nella richiesta POST
    data = {
        'username': utils.genera_nome_casuale(),
        'password': 'pass'
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def login_shop(path):
    # Dati da inviare nella richiesta POST
    data = {
        'nome': utils.genera_nome_casuale(),
        'password': 'pass'
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def login_admin(path):
    # Dati da inviare nella richiesta POST
    data = {
        'nome': 'ciao',
        'password': 'ciao'
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def doRegisterUser(path):
    # Dati da inviare nella richiesta POST
    data = {
        'nazione': utils.genera_nazione_casuale(),
        'citta': utils.genera_citta_casuale(),
        'cap': utils.genera_cap_casuale(),
        'via': utils.genera_via_casuale(),
        'civico': utils.genera_numeri_casuali(),
        'interno': random.randint(1, 20),
        'username': utils.genera_nome_casuale(),
        'nome': utils.genera_nome_casuale(),
        'cognome': utils.genera_nome_casuale(),
        'mail': utils.genera_mail_casuale(),
        'password': 'pass'
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def doRegisterShop(path):
    # Dati da inviare nella richiesta POST
    data = {
        'nazione': utils.genera_nazione_casuale(),
        'citta': utils.genera_citta_casuale(),
        'cap': utils.genera_cap_casuale(),
        'via': utils.genera_via_casuale(),
        'civico': utils.genera_numeri_casuali(),
        'interno': random.randint(1,20),
        'numero': utils.genera_numeri_casuali(),
        'titolare': utils.genera_nome_casuale(),
        'scadenza': utils.genera_futura_data_casuale(),
        'nome': utils.genera_nome_casuale(),
        'telefono': utils.genera_numero_telefono_casuale(),
        'mail': utils.genera_mail_casuale(),
        'password': 'pass'
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def doRegistrationCourier(path):
    # Dati da inviare nella richiesta POST
    data = {
        'nome': utils.genera_nome_casuale(),
        'cognome': utils.genera_nome_casuale(),
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def addPayment(path):
    # Dati da inviare nella richiesta POST
    data = {
        'numero': utils.genera_otto_numeri_casuali(),
        'cvv': utils.genera_numeri_casuali(),
        'titolare': utils.genera_nome_casuale(),
        'scadenza': utils.genera_futura_data_casuale()
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def removeMethodPaymentToUser(path):
    # Invia la richiesta POST con il corpo JSON
    response = requests.delete(path)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def addItem(path):
    # Dati da inviare nella richiesta POST
    data = {
        'categoria': 'scarpe da calcio',
        'nome': utils.genera_nome_articolo(),
        'modello': 'alla moda',
        'marca': utils.genera_marca(),
        'descrizione': utils.genera_descrizione(),
        'disponibilita': random.randint(1,100),
        'prezzo': random.uniform(0.0, 100.0),
        'pubblicazione': utils.genera_data_casuale(),
        'offerta': random.choice(["true", "false"]),
        'inizio_offerta': utils.genera_data_casuale(),
        'percentuale': str(round(random.uniform(0.0, 100.0), 2)),
        'durata_ore': random.randint(0, 100)
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def put(path):
    # Invia la richiesta POST con il corpo JSON
    response = requests.put(path)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def delete(path):
    # Invia la richiesta POST con il corpo JSON
    response = requests.delete(path)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def cartItem(path):
    # Dati da inviare nella richiesta POST
    data = {
        'quantita': random.randint(1,100),
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.put(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def get(path):
    # Invia la richiesta POST con il corpo JSON
    response = requests.get(path)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def newOrder(path):
    # Dati da inviare nella richiesta POST
    data = {
        'stato': utils.stato_consegna(),
        'idCarta': random.randint(1,10)
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def addItemOrder(path):
    # Dati da inviare nella richiesta POST
    data = {
        'quantita': random.randint(1,10)
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.put(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)
    
def newCategory(path):
    # Dati da inviare nella richiesta POST
    data = {
        'nome': utils.genera_categoria_casuale(),
        'padre': utils.genera_categoria_casuale()
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)

def reviewItem(path):
    
    # Dati da inviare nella richiesta POST
    data = {
        'valutazione': random.randint(1,5),
        'descrizione': utils.genera_descrizione()
    }

    # Converte i dati in formato JSON
    json_data = json.dumps(data)

    # Intestazioni della richiesta
    headers = {
        'Content-Type': 'application/json'
    }

    # Invia la richiesta POST con il corpo JSON
    response = requests.post(path, headers=headers, data=json_data)

    # Controllo del codice di stato della risposta
    if response.status_code == 200:
        # Stampa il contenuto della risposta
        print(response.text)
    else:
        print(f"Errore nella richiesta: {response.status_code}")
        print("Messaggio di errore:", response.text)