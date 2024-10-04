#include "Ordine.h"

Ordine::Ordine(int id, const std::string& istante, const std::string& stato, const Utente& utente, const Metodo_pagamento& metodo_pagamento)
    : id(id), istante(istante), stato(stato), utente(utente), metodo_pagamento(metodo_pagamento) {}
