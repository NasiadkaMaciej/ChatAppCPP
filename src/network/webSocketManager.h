#pragma once

#include <functional>
#include <ixwebsocket/IXWebSocket.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class WebSocketManager {
  public:
	using MessageCallback = std::function<void(const json&)>;
	using StatusCallback = std::function<void(const std::string&)>;
	using ConnectionStatusCallback = std::function<void(bool)>;

	WebSocketManager(const std::string& url);
	~WebSocketManager();

	// Connection management
	bool connect();
	void disconnect();
	bool isConnected() const;

	// Send messages
	bool sendMessage(const json& message);
	bool sendRawMessage(const std::string& message);

	// Set callbacks
	void setMessageCallback(MessageCallback callback);
	void setStatusCallback(StatusCallback callback);
	void setConnectionStatusCallback(ConnectionStatusCallback callback);

  private:
	ix::WebSocket webSocket;
	std::string url;
	bool connected;

	MessageCallback onMessage;
	StatusCallback onStatus;
	ConnectionStatusCallback onConnectionStatus;

	void setupWebSocketCallbacks();
	void handleWebSocketMessage(const ix::WebSocketMessagePtr& msg);
};