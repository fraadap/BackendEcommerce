#include "Metodo_pagamento.h"

Metodo_pagamento::Metodo_pagamento(const int id, const int numero, const int cvv, const std::string& titolare, const std::string& scadenza)
    : id(id), numero(numero), cvv(cvv), titolare(titolare), scadenza(scadenza) {}
