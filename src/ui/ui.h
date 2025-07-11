#pragma once

#include "uiManager.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

class UI {
  public:
	UI();
	~UI();

	// Initialize the UI
	void init();

	// Main UI loop
	void run(std::function<void(const std::string&)> messageHandler);

	// Add a message to the chat window
	void addMessage(const std::string& username, const std::string& message);

	// Add a system message (like user joined/left)
	void addSystemMessage(const std::string& message);

	// Update the user list
	void updateUsers(const std::vector<std::string>& users);

	// Show an error or notification in the status bar
	void showStatus(const std::string& status);

	// Check if chat is scrolled to the bottom
	bool isOnBottom() const;

	// Update the room name in the chat window
	void updateRoomName(const std::string& roomName);

	// Clean up resources and exit
	void cleanup();

  private:
	std::unique_ptr<UIManager> uiManager;
	std::string statusMessage;

	// Input handling
	std::string handleInput();

	// Window management
	void handleResize();
};