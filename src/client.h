#pragma once

#include "command/commandProcessor.h"
#include "message/messageHandler.h"
#include "ui/ui.h"
#include <ixwebsocket/IXWebSocket.h>
#include <memory>
#include <string>
#include <vector>

class Client : public MessageHandler {
  public:
	Client(const std::string& url);
	~Client();

	// Connect to server
	bool connect();

	// Send message to server
	bool sendMessage(const std::string& message);

	// Run the client
	void run();

	// MessageHandler implementation
	void handleMessage(const std::string& message) override;
	void handleSystemEvent(const std::string& event) override;
	void handleUserListUpdate(const std::vector<std::string>& users) override;

  private:
	ix::WebSocket webSocket;
	std::string username;
	std::string url;
	std::string currentRoom;
	bool connected;

	std::unique_ptr<UI> ui;
	std::unique_ptr<CommandProcessor> commandProcessor;

	// WebSocket handlers
	void setupWebSocketCallbacks();
	void handleWebSocketMessage(const ix::WebSocketMessagePtr& msg);

	// Room operations
	void joinRoom(const std::string& roomName, const std::string& username);
	void requestRooms();

	// Command handling
	void handleCommand(const std::string& command);
	void initCommandHandlers();
};