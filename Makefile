CXX = g++
CXXFLAGS = -std=c++17 -Wall
# LDFLAGS = -lstdc++fs

SRC_DIR = src
UTILS_DIR = utils
TABLES_DIR = src/tables
BIN = dodoDb

SRCS = $(SRC_DIR)/main.cpp $(UTILS_DIR)/string_utils.cpp $(TABLES_DIR)/table.cpp ${TABLES_DIR}/table_manager.cpp

all: $(BIN)

$(BIN): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRCS)
run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

force-rebuild:
	taskkill /IM dodoDb.exe /F || exit 0
	make run
