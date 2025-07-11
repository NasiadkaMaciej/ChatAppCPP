#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class MessageHandler {
  public:
	virtual ~MessageHandler() = default;

	// Process incoming JSON messages
	virtual void processMessage(const json& message) = 0;

	// Handle chat messages
	virtual void handleChatMessage(const std::string& username, const std::string& message) = 0;

	// Handle system events
	virtual void handleSystemEvent(const std::string& event) = 0;

	// Handle user list updates
	virtual void handleUserListUpdate(const std::vector<std::string>& users) = 0;

	// Handle room list updates
	virtual void handleRoomListUpdate(const std::vector<std::string>& rooms) = 0;
};