#include "Utente_metodo.h"

Utente_metodo::Utente_metodo(const int id, const Utente utente, const Metodo_pagamento& metodo_pagamento, const std::string& aggiunta)
    : id(id), utente(utente), metodo_pagamento(metodo_pagamento), aggiunta(aggiunta) {}
