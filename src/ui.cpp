#include "ui.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

UI::UI()
  : statusMessage("Welcome to Chat") {}

UI::~UI() {
	cleanup();
}

void UI::init() {
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

void UI::initWindows() {
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

	// Create UI elements
	chatElement = std::make_unique<ChatElement>(chatHeight, chatWidth, 0, 0);
	userListElement =
	  std::make_unique<UserListElement>(userListHeight, userListWidth, 0, chatWidth);
	inputElement = std::make_unique<InputElement>(inputHeight, inputWidth, maxY - 2, 0);
	statusElement = std::make_unique<StatusElement>(statusHeight, maxX, maxY - 1, 0);

	chatElement->setNeedRedraw(true);
	userListElement->setNeedRedraw(true);
	inputElement->setNeedRedraw(true);
	statusElement->setNeedRedraw(true);
	
	// Populate elements list
	elements.clear();
	elements.push_back(chatElement.get());
	elements.push_back(userListElement.get());
	elements.push_back(inputElement.get());
	elements.push_back(statusElement.get());

	// Show initial status
	statusElement->setStatus(statusMessage);

	// Force initial draw of all elements
	drawAll();
}

void UI::drawAll() {
	for (auto* element : elements) {
		element->draw();
		element->refresh();
	}

	// Move cursor to input element
	inputElement->refresh();
}

std::string UI::handleInput() {
	int ch = getch();
	std::string result;


	if (ch == KEY_ENTER || ch == '\n' || ch == '\r') {
		// Submit current input
		result = inputElement->getInput();

		if (!result.empty()) {
			// Add to history
			inputElement->addToHistory(result);
		}
	} else if (ch == KEY_RESIZE) {
		// Handle terminal resize
		handleResize();
	} else {
		// Let input element handle the character
		inputElement->handleInput(ch);

		// Let chat element handle navigation keys
		chatElement->handleInput(ch);
	}

	return result;
}

void UI::handleResize() {
	endwin();
	refresh();
	clear();

	// Re-initialize windows
	initWindows();
}

void UI::run(std::function<void(const std::string&)> messageHandler) {
	bool running = true;

	while (running) {
		try {
			std::string input = handleInput();

			if (!input.empty()) {
				// Check for exit commands directly
				if (input == "/exit") {
					running = false;
					continue;
				}

				// Process the input via the callback
				messageHandler(input);
			}

			// Only redraw elements that need redrawing
			for (auto* element : elements) {
				if (element->getNeedRedraw()) {
					element->draw();
					element->refresh();
				}
			}

			inputElement->refresh();

			// Short delay to prevent high CPU usage
			napms(20);
		} catch (const std::exception& e) {
			// Show exception in the status bar
			showStatus("Error: " + std::string(e.what()));
			// Also add it as a system message
			addSystemMessage("Error occurred: " + std::string(e.what()));
		}
	}
}

void UI::addMessage(const std::string& username, const std::string& message) {
	// Get current time
	auto now = std::time(nullptr);
	auto tm = *std::localtime(&now);
	std::ostringstream oss;
	oss << std::put_time(&tm, "[%H:%M:%S] ");

	std::string formattedMessage = oss.str() + username + ": " + message;
	chatElement->addMessage(formattedMessage);
}

void UI::addSystemMessage(const std::string& message) {
	// Get current time
	auto now = std::time(nullptr);
	auto tm = *std::localtime(&now);
	std::ostringstream oss;
	oss << std::put_time(&tm, "[%H:%M:%S] ");

	std::string formattedMessage = oss.str() + "* " + message;
	chatElement->addMessage(formattedMessage);
}

void UI::updateUsers(const std::vector<std::string>& users) {
	userListElement->updateUsers(users);
}

void UI::showStatus(const std::string& status) {
	statusMessage = status;
	statusElement->setStatus(status);
	inputElement->setNeedRedraw(true); // Get input focus back
}

bool UI::isOnBottom() const {
	return chatElement->isOnBottom();
}

void UI::cleanup() {
	// Elements will clean up their windows in destructors
	elements.clear();
	chatElement.reset();
	inputElement.reset();
	userListElement.reset();
	statusElement.reset();
	endwin();
}