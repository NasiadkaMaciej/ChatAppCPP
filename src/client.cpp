#include "client.h"
#include <sstream>

Client::Client(const std::string& url)
  : ui(std::make_unique<UI>())
  , commandProcessor(std::make_unique<CommandProcessor>())
  , webSocketManager(std::make_unique<WebSocketManager>(url)) {

	// Initialize command handlers
	initCommandHandlers();

	// Set up WebSocket callbacks
	webSocketManager->setMessageCallback([this](const json& msg) { processMessage(msg); });
	webSocketManager->setStatusCallback([this](const std::string& status) { handleSystemEvent(status); });
}

Client::~Client() = default;

void Client::initCommandHandlers() {
	commandProcessor->registerCommand("/join", [this](const std::string& args) {
		std::istringstream iss(args);
		std::string room, username;
		iss >> room >> username;

		if (room.empty() || username.empty()) {
			ui->addSystemMessage("Usage: /join <room> <username>");
			return;
		}

		joinRoom(room, username);
	});

	commandProcessor->registerCommand("/rooms", [this](const std::string&) { requestRooms(); });

	commandProcessor->registerCommand("/help", [this](const std::string&) {
		ui->addSystemMessage("Available commands:");
		ui->addSystemMessage("/join <room> <username> - Join a room");
		ui->addSystemMessage("/rooms - Show available rooms on the server");
		ui->addSystemMessage("/exit - Exit the application");
		ui->addSystemMessage("/help - Show this help");
	});
}

void Client::run() {
	// Initialize UI
	ui->init();

	// Connect to server
	ui->showStatus("Connecting to server...");
	if (!webSocketManager->connect()) {
		ui->showStatus("Failed to connect");
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}

	ui->showStatus("Connected! Please enter your username and room: /join <room> <username>");

	// Main UI loop
	ui->run([this](const std::string& input) { handleUserInput(input); });
}

void Client::handleUserInput(const std::string& input) {
	// Check if this is a command
	if (!input.empty() && input[0] == '/') {
		handleCommand(input);
		return;
	}

	// Regular message - send to current room
	if (!currentRoom.empty() && !username.empty()) {
		json msgJson;
		msgJson["type"] = "sendMessage";
		msgJson["data"] = input;
		webSocketManager->sendMessage(msgJson);
	} else {
		ui->addSystemMessage("You must join a room first: /join <room> <username>");
	}
}

void Client::handleCommand(const std::string& command) {
	if (command == "/exit")
		// This will be handled in the UI's run method
		return;

	// Extract command and arguments
	std::string cmd = command;
	std::string args;

	size_t spacePos = command.find(' ');
	if (spacePos != std::string::npos) {
		cmd = command.substr(0, spacePos);
		args = command.substr(spacePos + 1);
	}

	// Process via command processor
	if (!commandProcessor->processCommand(cmd, args)) ui->addSystemMessage("Unknown command: " + cmd);
}

void Client::processMessage(const json& message) {
	if (!message.contains("type")) return;

	std::string type = message["type"];

	if (type == "roomList") {
		std::vector<std::string> rooms;
		for (auto& room : message["data"])
			rooms.push_back(room);
		handleRoomListUpdate(rooms);
	} else if (type == "message") {
		std::string messageText = message["data"];

		// Check if it's a user message (username: message format)
		size_t colonPos = messageText.find(": ");
		if (colonPos != std::string::npos) {
			std::string username = messageText.substr(0, colonPos);
			std::string content = messageText.substr(colonPos + 2);
			handleChatMessage(username, content);
		} else {
			handleSystemEvent(messageText);
		}
	} else if (type == "userList") {
		std::vector<std::string> users;
		for (auto& user : message["data"])
			users.push_back(user);
		handleUserListUpdate(users);
	}
}

void Client::joinRoom(const std::string& roomName, const std::string& username) {
	if (!webSocketManager->isConnected()) {
		ui->addSystemMessage("Not connected to server");
		return;
	}

	this->username = username;
	currentRoom = roomName;
	ui->updateRoomName(roomName);

	// Send join room message
	json joinMsg = { { "type", "joinRoom" }, { "data", { { "username", username }, { "room", roomName } } } };

	webSocketManager->sendMessage(joinMsg);
	ui->showStatus("Joining room: " + roomName + " as " + username);
}

void Client::requestRooms() {
	if (!webSocketManager->isConnected()) return;

	json roomsMsg;
	roomsMsg["type"] = "getRoomList";
	webSocketManager->sendMessage(roomsMsg);
}

void Client::handleChatMessage(const std::string& username, const std::string& message) {
	ui->addMessage(username, message);
}

void Client::handleSystemEvent(const std::string& event) {
	ui->addSystemMessage(event);
}

void Client::handleUserListUpdate(const std::vector<std::string>& users) {
	ui->updateUsers(users);
}

void Client::handleRoomListUpdate(const std::vector<std::string>& rooms) {
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
}