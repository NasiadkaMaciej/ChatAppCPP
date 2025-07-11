# C++ Chat Client

A simple WebSocket-based chat client in C++ with an ncurses interface. Compatible with [JS ChatApp](https://github.com/NasiadkaMaciej/ChatApp).
![App screenshot](https://nasiadka.pl/project/cpp-chat-client/chat.png)

## Features
- Terminal-based UI with ncurses
- Split-screen layout with chat messages, user list and input area
- Join or create chat rooms
- See active users in rooms
- Message timestamps
- Chat history scrolling
- Resizable interface that adapts to terminal dimensions

## Building

### Dependencies
- IXWebSocket library
- nlohmann/json library
- ncurses library
- SSL/TLS support

### Install Dependencies and Build
```bash
# Install dependencies
make deps

# Build the client
make

# Optionally install
sudo make install
```

## Commands
- `/join <room> <username>` - Join a room with specified username
- `/help` - Show available commands
- `/rooms` - Show available rooms on the server
- `/exit` - Exit the application

## UI Navigation
- Arrow keys to scroll through chat history
- Type messages in the input area at the bottom
- Status information displayed in the bottom status bar
