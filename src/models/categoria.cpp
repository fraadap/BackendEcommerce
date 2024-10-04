#include "categoria.h"

// Costruttore
Categoria::Categoria(const std::string& nome, Categoria& categoria_padre)
    : nome(nome), categoria_padre(&categoria_padre) {}
