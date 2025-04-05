#include "client.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;

Client::Client(const std::string& url)
  : url(url)
  , connected(false)
  , ui(std::make_unique<UI>()) {}

Client::~Client() {
	webSocket.stop();
}

bool Client::connect() {
	ui->showStatus("Connecting to server...");

	webSocket.setUrl(url);

	// Set up the message handler
	webSocket.setOnMessageCallback(
	  [this](const ix::WebSocketMessagePtr& msg) { handleWebSocketMessage(msg); });

	webSocket.start();

	// Wait for connection
	for (int i = 0; i < 50; i++) {
		if (webSocket.getReadyState() == ix::ReadyState::Open) {
			connected = true;
			return true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return false;
}

// Add a new method to handle WebSocket messages
void Client::handleWebSocketMessage(const ix::WebSocketMessagePtr& msg) {
	if (msg->type == ix::WebSocketMessageType::Message) {
		try {
			json received = json::parse(msg->str);

			if (received.contains("type")) {
				std::string type = received["type"];

				if (type == "roomList") {
					// Handle room list update
					std::vector<std::string> rooms;
					for (auto& room : received["data"])
						rooms.push_back(room);

					// Display available rooms
					std::string roomsStr = "Available rooms: ";
					if (rooms.empty()) {
						roomsStr += "none (create a new one)";
					} else {
						for (size_t i = 0; i < rooms.size(); ++i) {
							if (i > 0) roomsStr += ", ";
							roomsStr += rooms[i];
						}
					}
					ui->addSystemMessage(roomsStr);
				} else if (type == "message") {
					// Handle chat message
					std::string messageText = received["data"];

					// Check if it's a user message (username: message format)
					size_t colonPos = messageText.find(": ");
					if (colonPos != std::string::npos) {
						std::string username = messageText.substr(0, colonPos);
						std::string content = messageText.substr(colonPos + 2);
						ui->addMessage(username, content);
					} else {
						ui->addSystemMessage(messageText);
					}
				} else if (type == "userList") {
					// Handle user list update
					std::vector<std::string> users;
					for (auto& user : received["data"])
						users.push_back(user);
					ui->updateUsers(users);
				}
			}
		} catch (const std::exception& e) {
			ui->addSystemMessage("Error parsing message: " + std::string(e.what()));
		}
	} else if (msg->type == ix::WebSocketMessageType::Open) {
		ui->addSystemMessage("Connected to server");
	} else if (msg->type == ix::WebSocketMessageType::Error) {
		ui->addSystemMessage("Connection error: " + msg->errorInfo.reason);
		connected = false;
	} else if (msg->type == ix::WebSocketMessageType::Close) {
		ui->addSystemMessage("Connection closed");
		connected = false;
	}
}

bool Client::sendMessage(const std::string& message) {
	if (!connected) return false;

	json msgJson;

	// Check if it's a command
	if (message[0] == '/') {
		handleCommand(message);
		return true;
	}

	// Regular message
	msgJson["type"] = "message";
	msgJson["room"] = currentRoom;
	msgJson["username"] = username;
	msgJson["text"] = message;

	webSocket.send(msgJson.dump());
	return true;
}

void Client::joinRoom(const std::string& roomName, const std::string& username) {
	if (!connected) {
		ui->addSystemMessage("Not connected to server");
		return;
	}

	this->username = username;
	currentRoom = roomName;

	// Send join room message
	json joinMsg = { { "type", "joinRoom" },
					 { "data", { { "username", username }, { "room", roomName } } } };

	webSocket.send(joinMsg.dump());
	ui->showStatus("Joining room: " + roomName + " as " + username);
}

void Client::requestRooms() {
    if (!connected) return;

    json roomsMsg;
    roomsMsg["type"] = "getRoomList";
    
    webSocket.send(roomsMsg.dump());
}

void Client::run() {
	// Initialize UI
	ui->init();

	// Connect to server
	ui->showStatus("Connecting to " + url + "...");
	if (!connect()) {
		ui->addSystemMessage("Failed to connect to server");
		ui->showStatus("Failed to connect");
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}

	ui->showStatus("Connected! Please enter your username and room: /join <room> "
				   "<username>");

	// Main UI loop
	ui->run([this](const std::string& input) { handleMessage(input); });

	// Stop the WebSocket before destroying the UI
	webSocket.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Client::handleMessage(const std::string& message) {
	// Check if this is a command
	if (!message.empty() && message[0] == '/') {
		handleCommand(message);
		return;
	} else {
		// Regular message - send to current room
		if (!currentRoom.empty() && !username.empty()) {
			json msgJson = { { "type", "sendMessage" }, { "data", message } };
			webSocket.send(msgJson.dump());
		} else {
			ui->addSystemMessage("You must join a room first: /join <room> <username>");
		}
	}
}

void Client::handleCommand(const std::string& command) {
	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	if (cmd == "/join") {
		std::string room, username;
		iss >> room >> username;

		if (room.empty() || username.empty()) {
			ui->addSystemMessage("Usage: /join <room> <username>");
			return;
		}

		joinRoom(room, username);
		ui->addSystemMessage("Joining room " + room + " as " + username);
	} else if (cmd == "/rooms") {
        requestRooms();
	} else if (cmd == "/help") {
		ui->addSystemMessage("Available commands:");
		ui->addSystemMessage("/join <room> <username> - Join a room");
		ui->addSystemMessage("/rooms - Show available rooms on the server");
		ui->addSystemMessage("/exit - Exit the application");
		ui->addSystemMessage("/help - Show this help");
	} else if (cmd == "/exit") {
		// This will be handled in the UI's run method
	} else {
		ui->addSystemMessage("Unknown command: " + cmd);
	}
}