# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Dossiers
SRC_DIR = src
OBJ_DIR = obj

# Fichiers sources et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Exécutable
TARGET = run

# Règle principale
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# Compilation des .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier obj si nécessaire
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean