#pragma once

#include "elements/chatElement.h"
#include "elements/inputElement.h"
#include "elements/statusElement.h"
#include "elements/uiElement.h"
#include "elements/userListElement.h"
#include <functional>
#include <list>
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

	// Clean up resources and exit
	void cleanup();

  private:
	// UI elements
	std::unique_ptr<ChatElement> chatElement;
	std::unique_ptr<InputElement> inputElement;
	std::unique_ptr<UserListElement> userListElement;
	std::unique_ptr<StatusElement> statusElement;

	// List of all elements for easier iteration
	std::list<UIElement*> elements;

	// Window dimensions and positions
	int chatHeight, chatWidth;
	int inputHeight, inputWidth;
	int userListHeight, userListWidth;
	int statusHeight;

	// Initialize windows
	void initWindows();

	// Resize all windows
	void setupWindows(bool initialSetup);

	// Handle keyboard input
	std::string handleInput();

	// Resize handler
	void handleResize();

	// Current status message
	std::string statusMessage;
};