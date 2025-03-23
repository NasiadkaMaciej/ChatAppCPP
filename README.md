# NewChat Client

A simple WebSocket-based chat client in C++. Compatible with [JS ChatApp](https://github.com/NasiadkaMaciej/ChatApp).

## Features
- Join or create chat rooms
- See active users in rooms
- Simple command interface

## Building

### Dependencies
- IXWebSocket library
- nlohmann/json library
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

### Run the Client
```bash
./chat
```

## Commands
- `/help` - Show available commands
- `/users` - Show users in the current room
- `/exit` - Exit the chat

## Todo
- Add ncurses interface
- Improve layout
- Add server component