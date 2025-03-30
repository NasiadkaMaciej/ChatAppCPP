CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lixwebsocket -lz -lpthread -lssl -lcrypto -lncurses

# Directory structure
SRC_DIR = src
BIN_DIR = bin

# Find all source files in src directory and subdirectories
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
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
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

install: $(TARGET)
	install -m 755 $(TARGET) /usr/bin/
	
uninstall:
	rm -f /usr/bin/chat

# Install dependencies
# Change CMAKE of IXWebSocket to enable TLS support
deps:
	@echo "Installing dependencies..."
	mkdir -p deps
	@if [ ! -d "deps/IXWebSocket" ]; then \
		git clone https://github.com/machinezone/IXWebSocket.git deps/IXWebSocket && \
		cd deps/IXWebSocket && \
		sed -i 's/option(USE_TLS "Enable TLS support" FALSE)/option(USE_TLS "Enable TLS support" TRUE)/' CMakeLists.txt && \
		mkdir -p build && cd build && \
		cmake .. && \
		make -j && \
		sudo make install; \
	fi
	@if [ ! -d "deps/json" ]; then \
		git clone https://github.com/nlohmann/json.git deps/json && \
		cd deps/json && \
		mkdir -p build && cd build && \
		cmake .. && \
		make -j && \
		sudo make install; \
	fi