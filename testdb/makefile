# Variabili per i comandi del compilatore
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11

# Nome dell'eseguibile da creare
TARGET := main

# Percorsi delle directory
SRCDIR := .
DBDIR := ../src/db
MODELSDIR := ../src/models
OBJDIR := obj/db
CON2DBDIR := ../src/db/con2db

# Lista dei file sorgente e oggetto
SRCS := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(DBDIR)/*.cpp) $(wildcard $(CON2DBDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Aggiunta dei file di intestazione .h dalla cartella models
INCLUDES := $(wildcard $(MODELSDIR)/*.h)

# Regole di compilazione
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDES)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regola per l'eseguibile
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lpq -lssl -lcrypto

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Phony target per evitare conflitti con nomi di file
.PHONY: clean
