#include "Negozio.h"

Negozio::Negozio(const int id, const std::string& nome, const std::string& telefono, const std::string& email, const Indirizzo& indirizzo, Metodo_pagamento& metodo_pagamento)
    : id(id), nome(nome), telefono(telefono), email(email), indirizzo(indirizzo), metodo_pagamento(metodo_pagamento) {}
