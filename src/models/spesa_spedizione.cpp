#include "SpesaSpedizione.h"

SpesaSpedizione::SpesaSpedizione(int id, const std::string& tipologia, double prezzo, const Articolo& articolo)
    : id(id), tipologia(tipologia), prezzo(prezzo), articolo(articolo) {}
