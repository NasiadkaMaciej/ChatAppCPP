#pragma once

#include <string>
#include <vector>

class MessageHandler {
  public:
	virtual ~MessageHandler() = default;

	// Handle incoming messages
	virtual void handleMessage(const std::string& message) = 0;

	// Handle system events
	virtual void handleSystemEvent(const std::string& event) = 0;

	// Handle user list updates
	virtual void handleUserListUpdate(const std::vector<std::string>& users) = 0;
};