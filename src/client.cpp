#include "client.h"
#include <chrono>
#include <thread>

// UI Functions Implementation
void printRooms(const std::vector<std::string> &rooms) {
	std::cout << "\n=== Available Rooms ===\n";
	if (rooms.empty())
		std::cout << "No rooms available. Create a new one!\n";
	else
		for (size_t i = 0; i < rooms.size(); i++)
			std::cout << i + 1 << ". " << rooms[i] << "\n";
	std::cout << "=======================\n";
}

void printHelp() {
	std::cout << "\n====== Commands ======\n";
	std::cout << "/help - Show this help message\n";
	std::cout << "/users - Show users in the current room\n";
	std::cout << "/exit - Exit the chat\n";
	std::cout << "======================\n";
}

// ChatClient Implementation
ChatClient::ChatClient(const std::string &url) : connected(false) { webSocket.setUrl(url); }

bool ChatClient::connect() {
	webSocket.start();

	// Wait for connection to be established (timeout after 5 seconds)
	for (int i = 0; i < 50; i++) {
		if (webSocket.getReadyState() == ix::ReadyState::Open)
			return true;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return false;
}

void ChatClient::disconnect() {
	webSocket.stop();
	std::cout << "Disconnected.\n";
}

bool ChatClient::joinRoom(const std::string &username, const std::string &roomName) {
	this->username = username;
	this->currentRoom = roomName;

	json joinMsg = {{"type", "joinRoom"}, {"data", {{"username", username}, {"room", roomName}}}};

	webSocket.send(joinMsg.dump());
	connected = true;
	return true;
}

void ChatClient::setupCallbacks() {
	webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr &msg) {
		if (msg->type == ix::WebSocketMessageType::Message) {
			json received = json::parse(msg->str);

			if (received.contains("type")) {
				std::string type = received["type"];

				if (type == "roomList") {
					availableRooms.clear();
					for (auto &room : received["data"])
						availableRooms.push_back(room);

					if (!connected) {
						printRooms(availableRooms);

						// Prompt for room selection or creation
						if (!availableRooms.empty())
							std::cout << "Enter room number to join or type a new room name: ";
						else
							std::cout << "Enter a new room name: ";

						std::string roomInput;
						std::getline(std::cin, roomInput);

						// Check if input is a number (selecting existing room)
						int roomIndex;
						try {
							roomIndex = std::stoi(roomInput);
							if (roomIndex > 0 && roomIndex <= static_cast<int>(availableRooms.size()))
								currentRoom = availableRooms[roomIndex - 1];
							else
								currentRoom = roomInput;
						} catch (...) { // Input is not a number, treat as new room name
							currentRoom = roomInput;
						}

						joinRoom(username, currentRoom);
					}
				} else if (type == "message") {
					std::cout << "\033[32m" << received["data"] << "\033[0m\n";
				} else if (type == "userList") {
					usersInRoom.clear();
					for (auto &user : received["data"])
						usersInRoom.push_back(user);

					std::cout << "\n=== Users in " << currentRoom << " ===\n";
					for (const auto &user : usersInRoom)
						std::cout << "- " << user << "\n";
					std::cout << "==========================\n";
				}
			}
		} else if (msg->type == ix::WebSocketMessageType::Open) {
			std::cout << "Connected to chat server.\n";
		} else if (msg->type == ix::WebSocketMessageType::Error) {
			std::cout << "Connection error: " << msg->errorInfo.reason << "\n";
		} else if (msg->type == ix::WebSocketMessageType::Close) {
			std::cout << "Connection closed.\n";
		}
	});
}

bool ChatClient::processCommand(const std::string &command) {
	if (command == "/exit") {
		return false;
	} else if (command == "/help") {
		printHelp();
	} else if (command == "/users") {
		json userListMsg = {{"type", "requestUserList"}};
		webSocket.send(userListMsg.dump());
	} else if (!command.empty()) {
		json data = {{"type", "sendMessage"}, {"data", command}};
		webSocket.send(data.dump());
	}
	return true;
}

void ChatClient::run() {
	std::cout << "Enter your username: ";
	std::getline(std::cin, username);

	setupCallbacks();

	if (!connect()) {
		std::cerr << "Failed to connect to server.\n";
		return;
	}

	// Wait for room selection and connection
	while (!connected) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	std::cout << "Joined room: " << currentRoom << "\n";
	std::cout << "Type your messages below (type /help for commands):\n";

	std::string message;
	bool running = true;

	while (running) {
		std::getline(std::cin, message);
		running = processCommand(message);
	}

	disconnect();
}