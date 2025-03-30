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

void UI::setupWindows(bool initialSetup) {
	// Get terminal dimensions
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);

	// Calculate dimensions
	userListWidth = std::max(20, maxX / 5);
	chatWidth = maxX - userListWidth;
	userListHeight = maxY - 3;
	chatHeight = maxY - 3;
	inputHeight = 1;
	inputWidth = maxX;
	statusHeight = 1;

	if (initialSetup) {
		chatElement = std::make_unique<ChatElement>(chatHeight, chatWidth, 0, 0);
		userListElement =
		  std::make_unique<UserListElement>(userListHeight, userListWidth, 0, chatWidth);
		inputElement = std::make_unique<InputElement>(inputHeight, inputWidth, maxY - 2, 0);
		statusElement = std::make_unique<StatusElement>(statusHeight, maxX, maxY - 1, 0);

		// Populate elements list
		elements.clear();
		elements.push_back(chatElement.get());
		elements.push_back(userListElement.get());
		elements.push_back(inputElement.get());
		elements.push_back(statusElement.get());

		// Set initial user list content
		std::vector<std::string> initialUserList;
		userListElement->updateUsers(initialUserList);

		// Set initial status
		statusElement->setStatus(statusMessage);
	} else {
		// Resize all UI elements
		chatElement->resize(chatHeight, chatWidth, 0, 0);
		userListElement->resize(userListHeight, userListWidth, 0, chatWidth);
		inputElement->resize(inputHeight, inputWidth, maxY - 2, 0);
		statusElement->resize(statusHeight, maxX, maxY - 1, 0);
	}

	// Redraw all elements
	for (auto* element : elements) {
		element->draw();
		element->refresh();
	}

	// Set input focus (todo: method?)
	wmove(inputElement->getWindow(), 0, inputElement->getInput().length() + 2);
	wrefresh(inputElement->getWindow());
}

// Now use this in both places:
void UI::initWindows() {
	setupWindows(true);
}

std::string UI::handleInput() {
	int ch = wgetch(inputElement->getWindow());
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
	clear();
	refresh();
	setupWindows(false);
}

void UI::run(std::function<void(const std::string&)> messageHandler) {
	bool running = true;

	while (running) {
		try {
			std::string input = handleInput();

			if (!input.empty()) {
				if (input == "/exit") {
					running = false;
					continue;
				}

				// Process input via callback
				messageHandler(input);
				inputElement->clearInput();
			}

			// Update elements that need redrawing using double-buffering
			for (auto* element : elements) {
				if (element && element->getNeedRedraw()) {
					element->draw();
					wnoutrefresh(element->getWindow());
					doupdate();
				}
			}

			// Small delay to reduce CPU usage
			napms(10);
		} catch (const std::exception& e) {
			showStatus("Error: " + std::string(e.what()));
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

	for (auto* element : elements) {
		if (element && element->getNeedRedraw()) {
			element->draw();
			wnoutrefresh(element->getWindow());
		}
	}
	doupdate();

	// Make sure cursor returns to input element after status update
	wmove(inputElement->getWindow(), 0, inputElement->getInput().length() + 2);
	wrefresh(inputElement->getWindow());
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