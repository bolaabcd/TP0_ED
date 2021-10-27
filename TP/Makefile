#---------------------------------------------------------------------
# Arquivo	: Makefile
# Conteúdo	: compilar o programa matop
# Autor		: Artur Gaspar da Silva (artur.gaspar@dcc.ufmg.br)
# Histórico	: 2021-10-27 - arquivo criado
#		: 2021-10-27 - estrutura de diretórios
#---------------------------------------------------------------------
# Opções	: make all - compila tudo
#		: make clean - remove objetos e executável
#---------------------------------------------------------------------

# Diretórios
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# Opções do compilador
CC = g++
CFLAGS = -Wall -c -I$(INCLUDE_FOLDER)
LIBS = -lm

# Arquivos
TARGET = $(BIN_FOLDER)/matop
HDR_FILES = $(INCLUDE_FOLDER)/mat.h $(INCLUDE_FOLDER)/memlog.h $(INCLUDE_FOLDER)/msgassert.h
OBJ_FILES = $(OBJ_FOLDER)/matop.o $(OBJ_FOLDER)/mat.o $(OBJ_FOLDER)/memlog.o

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(LIBS) $(OBJ_FILES)

$(OBJ_FOLDER)/matop.o: $(HDR_FILES) $(SRC_FOLDER)/matop.cpp
	$(CC) $(CFLAGS) -o $(OBJ_FOLDER)/matop.o $(SRC_FOLDER)/matop.cpp 

$(OBJ_FOLDER)/mat.o: $(HDR_FILES) $(SRC_FOLDER)/mat.cpp
	$(CC) $(CFLAGS) -o $(OBJ_FOLDER)/mat.o $(SRC_FOLDER)/mat.cpp 

$(OBJ_FOLDER)/memlog.o: $(HDR_FILES) $(SRC_FOLDER)/memlog.cpp
	$(CC) $(CFLAGS) -o $(OBJ_FOLDER)/memlog.o $(SRC_FOLDER)/memlog.cpp 
	
clean:
	rm $(TARGET) $(OBJ_FILES) 
