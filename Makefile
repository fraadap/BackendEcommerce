# Per cambiare percorso dove salvare l'eseguibile cambiare questo
TARGET = main

# Compilatore
CXX = g++

# Opzioni di compilazione
CXXFLAGS = -std=c++11

# Librerie
LIBS = -lhiredis -pthread -lpq -lm -lssl -lcrypto

# Percorsi per i file sorgente e gli header
SRCDIR = src
MODELSDIR = src/models
DBDIR = src/db
SERVERDIR = src/server
MANAGERSDIR = src/server/managers
BUILDDIR = obj

CON2REDISDIR = src/server/con2redis
CON2DBDIR = src/db/con2db

INCDIR = include

# Trova tutti i file sorgente
SRCS = $(wildcard $(SERVERDIR)/*.cpp) \
	   $(wildcard $(DBDIR)/*.cpp) $(wildcard $(MODELDIR)/*.cpp) \
	   $(wildcard $(MANAGERSDIR)/admin/*.cpp) \
       $(wildcard $(MANAGERSDIR)/user/*.cpp) \
       $(wildcard $(MANAGERSDIR)/deliver/*.cpp) \
       $(wildcard $(MANAGERSDIR)/shop/*.cpp)$(wildcard $(MANAGERSDIR)/*.cpp) \
	   

# Crea una lista di file oggetto
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Dipendenze degli header
DEPS = $(wildcard $(INCDIR)/*.h) \
       $(wildcard $(INCDIR)/managers/*.h) \
       $(wildcard $(INCDIR)/managers/admin/*.h) \
       $(wildcard $(INCDIR)/managers/user/*.h) \
       $(wildcard $(INCDIR)/managers/deliver/*.h) \
       $(wildcard $(INCDIR)/managers/shop/*.h)

# Aggiungi i file oggetto con2redis e con2db
CON2REDIS_OBJS = $(CON2REDISDIR)/obj/redisfun.o $(CON2REDISDIR)/obj/readreply.o
CON2DB_OBJS = $(CON2DBDIR)/pgsql.o

# Regola predefinita: build del target
all: $(TARGET)

# Regola per il target
$(TARGET): $(OBJS) $(CON2REDIS_OBJS) $(CON2DB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Regola per i file oggetto
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Pulizia
clean:
	find $(BUILDDIR) -name "*.o" -type f -delete