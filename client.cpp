#include <iostream>
#include <ixwebsocket/IXWebSocket.h>
#include <nlohmann/json.hpp>
#include <vector>

// ix -> IXWebSocket
using json = nlohmann::json;
using namespace std;

void printRooms(const vector<string> &rooms) {
	cout << "\n=== Available Rooms ===\n";
	if (rooms.empty())
		cout << "No rooms available. Create a new one!\n";
	else
		for (size_t i = 0; i < rooms.size(); i++)
			cout << i + 1 << ". " << rooms[i] << "\n";
	cout << "=======================\n";
}

void printHelp() {
	cout << "\n====== Commands ======\n";
	cout << "/help - Show this help message\n";
	cout << "/users - Show users in the current room\n";
	cout << "/exit - Exit the chat\n";
	cout << "======================\n";
}

int main() {
	string username;
	cout << "Enter your username: ";
	getline(cin, username);

	ix::WebSocket webSocket;
	webSocket.setUrl("wss://chat.nasiadka.pl/ws");

	string currentRoom;
	vector<string> availableRooms;
	vector<string> usersInRoom;
	bool connected = false;
	
	webSocket.setOnMessageCallback([&](const ix::WebSocketMessagePtr &msg) {
		if (msg->type == ix::WebSocketMessageType::Message) {
			json received = json::parse(msg->str);

			if (received.contains("type")) {
				string type = received["type"];

				if (type == "roomList") {
					availableRooms.clear();
					for (auto &room : received["data"])
						availableRooms.push_back(room);

					if (!connected) {
						printRooms(availableRooms);

						// Prompt for room selection or creation
						if (!availableRooms.empty())
							cout << "Enter room number to join or type a new room name: ";
						else
							cout << "Enter a new room name: ";

						string roomInput;
						getline(cin, roomInput);

						// Check if input is a number (selecting existing room)
						int roomIndex;
						try {
							roomIndex = stoi(roomInput);
							if (roomIndex > 0 && roomIndex <= static_cast<int>(availableRooms.size()))
								currentRoom = availableRooms[roomIndex - 1];
							else
								currentRoom = roomInput;
						} catch (...) { // Input is not a number, treat as new room name
							currentRoom = roomInput;
						}

						// Join the room
						json joinMsg = {{"type", "joinRoom"}, {"data", {{"username", username}, {"room", currentRoom}}}};
						webSocket.send(joinMsg.dump());
						connected = true;
					}
				} else if (type == "message") {
					cout << "\033[32m" << received["data"] << "\033[0m\n";
				} else if (type == "userList") {
					usersInRoom.clear();
					for (auto &user : received["data"])
						usersInRoom.push_back(user);

					cout << "\n=== Users in " << currentRoom << " ===\n";
					for (const auto &user : usersInRoom)
						cout << "- " << user << "\n";
					cout << "==========================\n";
				}
			}
		} else if (msg->type == ix::WebSocketMessageType::Open)
			cout << "Connected to chat server.\n";
		else if (msg->type == ix::WebSocketMessageType::Error)
			cout << "Connection error: " << msg->errorInfo.reason << "\n";
		else if (msg->type == ix::WebSocketMessageType::Close)
			cout << "Connection closed.\n";
	});

	webSocket.start();

	// Wait for connection to be established
	while (!connected)
		this_thread::sleep_for(chrono::milliseconds(100));

	cout << "Joined room: " << currentRoom << "\n";
	cout << "Type your messages below (type /help for commands):\n";

	string message;
	while (true) {
		getline(cin, message);

		if (message == "/exit")
			break;
		else if (message == "/help")
			printHelp();
		else if (message == "/users") {
			json userListMsg = {{"type", "requestUserList"}};
			webSocket.send(userListMsg.dump());
		} else if (!message.empty()) {
			json data = {{"type", "sendMessage"}, {"data", message}};
			webSocket.send(data.dump());
		}
	}

	webSocket.stop();
	cout << "Disconnected.\n";
	return 0;
}