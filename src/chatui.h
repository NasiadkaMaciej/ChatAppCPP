#pragma once

#include <functional>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

class ChatUI {
  public:
	ChatUI();
	~ChatUI();

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

	// Draw all windows
	void drawAll();

	// Clean up resources and exit
	void cleanup();

  private:
	// Windows
	WINDOW* chatWin;	 // Chat messages
	WINDOW* inputWin;	 // Input box
	WINDOW* userListWin; // User list
	WINDOW* statusWin;	 // Status bar

	// Window dimensions and positions
	int chatHeight, chatWidth;
	int inputHeight, inputWidth;
	int userListHeight, userListWidth;
	int statusHeight;

	// Message storage
	std::vector<std::string> messages;

	// Current input buffer
	std::string inputBuffer;

	// Current user list
	std::vector<std::string> users;

	// Initialize windows
	void initWindows();

	// Draw chat window
	void drawChat();

	// Draw input window
	void drawInput();

	// Draw user list
	void drawUserList();

	// Draw status bar
	void drawStatus();

	// Handle keyboard input
	std::string handleInput();

	// Resize handler
	void handleResize();

	// Current status message
	std::string statusMessage;
};