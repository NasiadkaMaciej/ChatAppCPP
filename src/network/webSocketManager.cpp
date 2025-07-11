#include "webSocketManager.h"

WebSocketManager::WebSocketManager(const std::string& url)
  : url(url)
  , connected(false) {}

WebSocketManager::~WebSocketManager() {
	disconnect();
}

bool WebSocketManager::connect() {
	if (connected) return true;

	webSocket.setUrl(url);
	setupWebSocketCallbacks();
	webSocket.start();

	// Wait for connection
	for (int i = 0; i < 50; i++) {
		if (webSocket.getReadyState() == ix::ReadyState::Open) {
			connected = true;
			if (onConnectionStatus) onConnectionStatus(true);
			return true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	if (onStatus) onStatus("Failed to connect to server");
	return false;
}

void WebSocketManager::disconnect() {
	if (!connected) return;

	webSocket.stop();
	connected = false;
	if (onConnectionStatus) onConnectionStatus(false);
}

bool WebSocketManager::isConnected() const {
	return connected;
}

bool WebSocketManager::sendMessage(const json& message) {
	if (!connected) return false;
	return sendRawMessage(message.dump());
}

bool WebSocketManager::sendRawMessage(const std::string& message) {
	if (!connected) return false;
	webSocket.send(message);
	return true;
}

void WebSocketManager::setMessageCallback(MessageCallback callback) {
	onMessage = callback;
}

void WebSocketManager::setStatusCallback(StatusCallback callback) {
	onStatus = callback;
}

void WebSocketManager::setConnectionStatusCallback(ConnectionStatusCallback callback) {
	onConnectionStatus = callback;
}

void WebSocketManager::setupWebSocketCallbacks() {
	webSocket.setOnMessageCallback(
	  [this](const ix::WebSocketMessagePtr& msg) { handleWebSocketMessage(msg); });
}

void WebSocketManager::handleWebSocketMessage(const ix::WebSocketMessagePtr& msg) {
	if (msg->type == ix::WebSocketMessageType::Message) {
		try {
			json received = json::parse(msg->str);
			if (onMessage) onMessage(received);
		} catch (const std::exception& e) {
			if (onStatus) onStatus("Error parsing message: " + std::string(e.what()));
		}
	} else if (msg->type == ix::WebSocketMessageType::Open) {
		connected = true;
		if (onStatus) onStatus("Connected to server");
		if (onConnectionStatus) onConnectionStatus(true);
	} else if (msg->type == ix::WebSocketMessageType::Error) {
		connected = false;
		if (onStatus) onStatus("Connection error: " + msg->errorInfo.reason);
		if (onConnectionStatus) onConnectionStatus(false);
	} else if (msg->type == ix::WebSocketMessageType::Close) {
		connected = false;
		if (onStatus) onStatus("Connection closed");
		if (onConnectionStatus) onConnectionStatus(false);
	}
}