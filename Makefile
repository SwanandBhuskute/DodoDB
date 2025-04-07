CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC_DIR = src
UTILS_DIR = utils
BIN = dodoDb

SRCS = $(SRC_DIR)/main.cpp $(UTILS_DIR)/string_utils.cpp

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
