CC=g++
CFLAGS=-g
RM=rm -rf
MKDIR_P=mkdir -p
SRC=$(wildcard $(SRC_DIR)/*.cpp)
MAIN_SRC=$(SRC_DIR)/main.cpp
TEST_SRC=$(TEST_DIR)/test.cpp
INCLUDES=$(wildcard $(INC_DIR)/*.hpp)
INC=-I$(INC_DIR) -I/usr/include/geotiff -I/usr/include/gdal
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
LIB=$(ELEVATION_LIB) -lboost_system -lboost_filesystem \
	-lboost_program_options -lboost_iostreams -pthread -lgdal -lm 
ELEVATION_LIB=$(LIB_DIR)/libelevation.a
TEST_BIN=$(BIN_DIR)/test
BIN=$(BIN_DIR)/main

DIRS=$(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)
SRC_DIR=./src
INC_DIR=./inc
TEST_DIR=./test
OBJ_DIR=./obj
LIB_DIR=./lib
BIN_DIR=./bin

TEMP_OBJ:=$(OBJ)
OBJ=$(filter-out ./obj/main.o, $(TEMP_OBJ)) 

mod: $(DIRS) $(BIN)
tests: $(DIRS) $(TEST_BIN)

$(BIN): $(MAIN_SRC) $(ELEVATION_LIB)
	$(CC) $(CFLAGS) $(MAIN_SRC) $(INC) $(LIB) -o $(BIN)

$(TEST_BIN): $(TEST_SRC) $(ELEVATION_LIB)
	$(CC) $(CFLAGS) $(TEST_SRC) $(INC) $(LIB) -lgtest -o $(TEST_BIN)

$(ELEVATION_LIB): $(OBJ) $(INCLUDES)
	ar rvs $(ELEVATION_LIB) $(OBJ)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(DIRS):
	$(MKDIR_P) $(OBJ_DIR)
	$(MKDIR_P) $(LIB_DIR)
	$(MKDIR_P) $(BIN_DIR)

.PHONY: tags
tags:
	ctags -R ./

.PHONY: clean
clean:
	$(RM) $(DIRS)
