#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <iostream>
#include <string>
#include <vector>
#include <ixwebsocket/IXWebSocket.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// UI Functions
void printRooms(const std::vector<std::string>& rooms);
void printHelp();

// Chat Client Class
class ChatClient {
public:
    ChatClient(const std::string& url);
    
    bool connect();
    void disconnect();
    bool joinRoom(const std::string& username, const std::string& roomName);
    void run();

private:
    ix::WebSocket webSocket;
    std::string username;
    std::string currentRoom;
    std::vector<std::string> availableRooms;
    std::vector<std::string> usersInRoom;
    bool connected;

    void setupCallbacks();
    bool processCommand(const std::string& command);
};

#endif // CHAT_CLIENT_H