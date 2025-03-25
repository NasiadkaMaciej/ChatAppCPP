#include "chatui.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

ChatUI::ChatUI()
  : chatWin(nullptr)
  , inputWin(nullptr)
  , userListWin(nullptr)
  , statusWin(nullptr) {}

ChatUI::~ChatUI() {
	cleanup();
}

void ChatUI::init() {
	// Initialize ncurses
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	start_color();
	use_default_colors();
	curs_set(1); // Show cursor
	timeout(100);

	// Create windows
	initWindows();
}

void ChatUI::initWindows() {
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);

	// Calculate dimensions
	userListWidth = std::max(20, maxX / 5);
	chatWidth = maxX - userListWidth;
	userListHeight = maxY - 3; // Room for status and input
	chatHeight = maxY - 3;
	inputHeight = 1;
	inputWidth = maxX;
	statusHeight = 1;

	// Create windows
	chatWin = newwin(chatHeight, chatWidth, 0, 0);
	userListWin = newwin(userListHeight, userListWidth, 0, chatWidth);
	inputWin = newwin(inputHeight, inputWidth, maxY - 2, 0);
	statusWin = newwin(statusHeight, maxX, maxY - 1, 0);

	drawAll();
}

void ChatUI::drawAll() {
	drawChat();
	drawUserList();
	drawInput();
	drawStatus();

	// Move cursor to input window
	wmove(inputWin, 0, inputBuffer.length() + 2);
	wrefresh(inputWin);
}

void ChatUI::drawChat() {
	wclear(chatWin);

	// Draw border and title
	box(chatWin, 0, 0);
	mvwprintw(chatWin, 0, 2, " Chat ");

	// Calculate visible lines (accounting for border)
	int visibleLines = chatHeight - 2;

	// Display messages
	size_t startIdx =
	  (messages.size() <= static_cast<size_t>(visibleLines)) ? 0 : messages.size() - visibleLines;

	for (size_t i = 0; i < std::min(messages.size(), static_cast<size_t>(visibleLines)); ++i)
		mvwprintw(chatWin, i + 1, 1, "%s", messages[startIdx + i].c_str());

	wrefresh(chatWin);
}

void ChatUI::drawUserList() {
	wclear(userListWin);

	// Draw border and title
	box(userListWin, 0, 0);
	mvwprintw(userListWin, 0, 2, " Users ");

	// Display users
	int visibleLines = userListHeight - 2;
	for (size_t i = 0; i < std::min(users.size(), static_cast<size_t>(visibleLines)); ++i)
		mvwprintw(userListWin, i + 1, 1, "%s", users[i].c_str());

	wrefresh(userListWin);
}

void ChatUI::drawInput() {
	wclear(inputWin);
	mvwprintw(inputWin, 0, 0, "> %s", inputBuffer.c_str());
	wrefresh(inputWin);
}

void ChatUI::drawStatus() {
	wclear(statusWin);
	mvwprintw(statusWin, 0, 0, "%s", statusMessage.c_str());
	wrefresh(statusWin);
}

std::string ChatUI::handleInput() {
	int ch = getch();
	std::string result;

	if (ch == KEY_ENTER || ch == '\n' || ch == '\r') {
		// Submit current input
		result = inputBuffer;
		inputBuffer.clear();
	} else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
		// Delete last character
		if (!inputBuffer.empty()) inputBuffer.pop_back();
	} else if (ch == KEY_RESIZE) {
		// Handle terminal resize
		handleResize();
	} else if (ch >= 32 && ch <= 126) {
		// Add printable characters to input buffer
		inputBuffer += static_cast<char>(ch);
	}

	drawInput();
	return result;
}

void ChatUI::handleResize() {
	endwin();
	refresh();
	clear();

	// Re-initialize windows
	initWindows();
}

void ChatUI::run(std::function<void(const std::string&)> messageHandler) {
	bool running = true;

	while (running) {
		try {
			std::string input = handleInput();

			if (!input.empty()) {
				// Check for exit commands directly here as a safety net
				// ToDo: Change that
				if (input == "/exit") {
					running = false;
					continue;
				}

				// Process the input via the callback
				messageHandler(input);
			}

			// Make sure the display is updated regularly
			// ToDo: This should be done in a separate thread
			// Maybe check if there's a change before redrawing?
			drawAll();

			// Short delay to prevent high CPU usage
			napms(100);
		} catch (const std::exception& e) {
			// Show exception in the status bar
			showStatus("Error: " + std::string(e.what()));
			// Also add it as a system message
			addSystemMessage("Error occurred: " + std::string(e.what()));
			// Continue the loop despite the error
		}
	}
}

void ChatUI::addMessage(const std::string& username, const std::string& message) {
	// Get current time
	// ToDo: Make this a separate method and use it in addSystemMessage
	auto now = std::time(nullptr);
	auto tm = *std::localtime(&now);
	std::ostringstream oss;
	oss << std::put_time(&tm, "[%H:%M:%S] ");

	std::string formattedMessage = oss.str() + username + ": " + message;
	messages.push_back(formattedMessage);

	drawChat();
}

void ChatUI::addSystemMessage(const std::string& message) {
	// Get current time
	auto now = std::time(nullptr);
	auto tm = *std::localtime(&now);
	std::ostringstream oss;
	oss << std::put_time(&tm, "[%H:%M:%S] ");

	std::string formattedMessage = oss.str() + "* " + message;
	messages.push_back(formattedMessage);

	drawChat();
}

void ChatUI::updateUsers(const std::vector<std::string>& users) {
	this->users = users;
	drawUserList();
}

void ChatUI::showStatus(const std::string& status) {
	statusMessage = status;
	drawStatus();
}

void ChatUI::cleanup() {
	if (chatWin) delwin(chatWin);
	if (inputWin) delwin(inputWin);
	if (userListWin) delwin(userListWin);
	if (statusWin) delwin(statusWin);
	endwin();
}