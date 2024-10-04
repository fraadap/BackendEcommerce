#include "Ordine_corriere.h"

Ordine_corriere::Ordine_corriere(int id, const Ordine& ordine, const Corriere& corriere,
                                 const std::string& inizio, const std::string& fine)
    : id(id), ordine(ordine), corriere(corriere), inizio(inizio), fine(fine) {}
