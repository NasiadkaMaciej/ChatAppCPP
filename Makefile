CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lixwebsocket -lz -lpthread -lssl -lcrypto

# Directory structure
SRC_DIR = src
BIN_DIR = bin

# Find all source files in src directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Generate object file paths in bin directory
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/chat

.PHONY: all clean install dirs

all: dirs $(TARGET)

# Create bin directory if it doesn't exist
dirs:
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile .cpp to .o files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

install: $(TARGET)
	install -m 755 $(TARGET) /usr/bin/
	
uninstall:
	rm -f /usr/bin/chat

# If dependencies need to be installed
deps:
	@echo "Installing dependencies..."
	@if [ ! -d "IXWebSocket" ]; then \
		git clone https://github.com/machinezone/IXWebSocket.git && \
		cd IXWebSocket && \
		mkdir -p build && cd build && \
		cmake .. && \
		make -j && \
		sudo make install; \
	fi
	@if [ ! -d "json" ]; then \
		git clone https://github.com/nlohmann/json && \
		cd json && \
		mkdir -p build && cd build && \
		cmake .. && \
		make -j && \
		sudo make install; \
	fi