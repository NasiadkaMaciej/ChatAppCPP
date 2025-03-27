# NewChat Client

A simple WebSocket-based chat client in C++ with an ncurses interface. Compatible with [JS ChatApp](https://github.com/NasiadkaMaciej/ChatApp).
![App screenshot](https://nasiadka.pl/projects/CPPChat/chat.jpg)

## Features
- Terminal-based UI with ncurses
- Split-screen layout with chat messages, user list, and input areas
- Join or create chat rooms
- See active users in rooms
- Message timestamps
- System messages for connection events

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
- `/users - 
- `/help` - Show available commands
- `/users` - Show users in the current room
- `/rooms` - Show available rooms on the server
- `/exit` - Exit the chat

- `/exit` - Exit the chat application