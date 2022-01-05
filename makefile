CC=g++
CFLAGS=-g
RM=rm -rf
MKDIR_P=mkdir -p
SRC=$(wildcard $(SRC_DIR)/*.cpp)
INC=-I/usr/include -I$(INC_DIR) -I/usr/include/geotiff -I/usr/include/gdal
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
LIB=-lgdal -lm
BIN=$(BIN_DIR)/main

DIRS=$(OBJ_DIR) $(BIN_DIR)
SRC_DIR=./src
INC_DIR=./inc
OBJ_DIR=./obj
BIN_DIR=./bin

mod: $(DIRS) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(INC) $(LIB) -o $(BIN)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(DIRS):
	$(MKDIR_P) $(OBJ_DIR)
	$(MKDIR_P) $(BIN_DIR)

.PHONY: tags
tags:
	ctags -R ./

.PHONY: clean
clean:
	$(RM) $(OBJ)
	$(RM) $(BIN)
	$(RM) $(DIRS)
