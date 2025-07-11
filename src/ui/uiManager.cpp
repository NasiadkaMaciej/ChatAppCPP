#include "uiManager.h"
#include <algorithm>
#include <ncurses.h>

UIManager::UIManager() {}

UIManager::~UIManager() {
	cleanup();
}

void UIManager::init() {
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

void UIManager::initWindows() {
	setupWindows(true);
}

void UIManager::setupWindows(bool initialSetup) {
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
		// Create UI elements
		chatElement = std::make_unique<ChatElement>(chatHeight, chatWidth, 0, 0);
		userListElement = std::make_unique<UserListElement>(userListHeight, userListWidth, 0, chatWidth);
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

	// Set input focus
	wmove(inputElement->getWindow(), 0, inputElement->getInput().length() + 2);
	wrefresh(inputElement->getWindow());
}

void UIManager::handleResize() {
	clear();
	refresh();
	setupWindows(false);
}

void UIManager::refreshElements() {
	// Update elements that need redrawing using double-buffering
	for (auto* element : elements) {
		if (element && element->getNeedRedraw()) {
			element->draw();
			wnoutrefresh(element->getWindow());
		}
	}
	doupdate();

	// Make sure the input element's cursor is properly positioned
	inputElement->refresh();
}

void UIManager::cleanup() {
	// Elements will clean up their windows in destructors
	elements.clear();
	chatElement.reset();
	inputElement.reset();
	userListElement.reset();
	statusElement.reset();
	endwin();
}