CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC_DIR = src
OBJ_DIR = obj

#fichiers sources et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#executable
TARGET = run

all: $(TARGET)

#construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

#compilation des .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#creation du dossier obj si nécessaire
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

#nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

#pour eviter les bugs si un fichier clean ou all existe
.PHONY: all clean