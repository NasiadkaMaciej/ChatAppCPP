#pragma once

#include "command/commandProcessor.h"
#include "message/messageHandler.h"
#include "network/webSocketManager.h"
#include "ui/ui.h"
#include <memory>
#include <string>
#include <vector>

class Client : public MessageHandler {
  public:
	Client(const std::string& url);
	~Client();

	// Run the client
	void run();

	// MessageHandler implementation
	void processMessage(const json& message) override;
	void handleChatMessage(const std::string& username, const std::string& message) override;
	void handleSystemEvent(const std::string& event) override;
	void handleUserListUpdate(const std::vector<std::string>& users) override;
	void handleRoomListUpdate(const std::vector<std::string>& rooms) override;

  private:
	std::string username;
	std::string currentRoom;

	std::unique_ptr<UI> ui;
	std::unique_ptr<CommandProcessor> commandProcessor;
	std::unique_ptr<WebSocketManager> webSocketManager;

	// Input handling
	void handleUserInput(const std::string& input);
	void handleCommand(const std::string& command);

	// Room operations
	void joinRoom(const std::string& roomName, const std::string& username);
	void requestRooms();

	// Initialize command handlers
	void initCommandHandlers();
};