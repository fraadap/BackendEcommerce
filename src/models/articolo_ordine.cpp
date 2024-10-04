#include "articolo_ordine.h"

Articolo_ordine::Articolo_ordine(int id, const Ordine& ordine, const Articolo& articolo, int quantita)
    : id(id), ordine(ordine), articolo(articolo), quantita(quantita) {}


