#include "ui.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

UI::UI()
  : uiManager(std::make_unique<UIManager>())
  , statusMessage("Welcome to Chat") {}

UI::~UI() {
	cleanup();
}

void UI::init() {
	// Initialize ncurses and UI components
	uiManager->init();

	// Set initial status
	showStatus(statusMessage);
}

std::string UI::handleInput() {
	auto* inputElement = uiManager->getInputElement();
	wint_t ch;
	int result_get = wget_wch(inputElement->getWindow(), &ch);
	std::string result;

	if (result_get == ERR) return result;

	if (ch == KEY_ENTER || ch == '\n' || ch == '\r') {
		// Submit current input
		result = inputElement->getInput();
		inputElement->clearInput();
		return result;
	}

	if (result_get == KEY_CODE_YES) {
		if (ch == KEY_RESIZE) {
			// Handle terminal resize
			handleResize();
		} else if (ch == KEY_UP || ch == KEY_DOWN) {
			// Direct navigation keys to chat element for scrolling
			uiManager->getChatElement()->handleInput(ch);
		} else {
			// Let input element handle other special keys
			inputElement->processInput(ch, true); // It's a special key
		}
	} else {
		// Regular character input
		inputElement->processInput(ch, false); // It's a regular character
	}

	return result;
}

void UI::handleResize() {
	uiManager->handleResize();
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
			}

			// Update elements that need redrawing
			uiManager->refreshElements();

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
	uiManager->getChatElement()->addMessage(formattedMessage);
}

void UI::addSystemMessage(const std::string& message) {
	// Get current time
	auto now = std::time(nullptr);
	auto tm = *std::localtime(&now);
	std::ostringstream oss;
	oss << std::put_time(&tm, "[%H:%M:%S] ");

	std::string formattedMessage = oss.str() + "* " + message;
	uiManager->getChatElement()->addMessage(formattedMessage);

	// EventBus removed - direct calls should be used if notification is needed
}

void UI::updateUsers(const std::vector<std::string>& users) {
	uiManager->getUserListElement()->updateUsers(users);
}

void UI::showStatus(const std::string& status) {
	statusMessage = status;
	uiManager->getStatusElement()->setStatus(status);
	uiManager->refreshElements();
}

bool UI::isOnBottom() const {
	return uiManager->getChatElement()->isOnBottom();
}

void UI::updateRoomName(const std::string& roomName) {
	uiManager->getChatElement()->setRoomName(roomName);
}

void UI::cleanup() {
	uiManager->cleanup();
}