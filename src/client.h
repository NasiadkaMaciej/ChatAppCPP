#pragma once

#include "chatui.h"
#include <functional>
#include <ixwebsocket/IXWebSocket.h>
#include <memory>
#include <string>
#include <vector>

class ChatClient {
  public:
	ChatClient(const std::string& url);
	~ChatClient();
	bool connect();
	bool sendMessage(const std::string& message);
	void requestUsers();
	void run();

  private:
	ix::WebSocket webSocket;
	std::string username;
	std::string url;
	std::string currentRoom;
	bool connected;
	std::unique_ptr<ChatUI> ui;
	std::vector<std::string> users;

	// Message handlers
	void handleMessage(const std::string& message);
	void handleCommand(const std::string& command);
	void handleWebSocketMessage(const ix::WebSocketMessagePtr& msg);
	void joinRoom(const std::string& roomName, const std::string& username);
};