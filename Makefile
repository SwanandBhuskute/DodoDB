CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall
CFLAGS = -Wall

SRC_DIR = src
UTILS_DIR = utils
TABLES_DIR = src/tables
BIN = dodoDb

CPP_SRCS = $(SRC_DIR)/main.cpp $(UTILS_DIR)/string_utils.cpp $(TABLES_DIR)/table.cpp $(TABLES_DIR)/table_manager.cpp
C_SRCS = $(UTILS_DIR)/linenoise.c

OBJ_CPP = $(CPP_SRCS:.cpp=.o)
OBJ_C = $(C_SRCS:.c=.o)

all: $(BIN)

$(UTILS_DIR)/linenoise.o: $(UTILS_DIR)/linenoise.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(OBJ_CPP) $(UTILS_DIR)/linenoise.o
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

clean:
	del /q $(BIN) $(OBJ_CPP) $(UTILS_DIR)\\linenoise.o 2>nul

force-rebuild:
	-taskkill /IM $(BIN).exe /F >nul 2>&1 || exit 0
	make clean
	make run
