#include "inputElement.h"

InputElement::InputElement(int height, int width, int startY, int startX)
  : UIElement(height, width, startY, startX)
  , historyIndex(0) {

	win = newwin(height, width, startY, startX);
	wtimeout(win, 50);
	draw();
}

InputElement::~InputElement() {
	if (win) delwin(win);
}

void InputElement::draw() {
	werase(win);
	mvwprintw(win, 0, 0, "> %s", inputBuffer.c_str());
	needRedraw = false;
}

void InputElement::refresh() {
	wmove(win, 0, inputBuffer.length() + 2); // Position cursor after text
	wrefresh(win);
}

void InputElement::handleInput(int ch) {
	bool changed = false;

	if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
		// Delete last character
		if (!inputBuffer.empty()) {
			inputBuffer.pop_back();
			changed = true;
		}
	} else if (ch == KEY_UP) {
		changed = historyUp();
	} else if (ch == KEY_DOWN) {
		changed = historyDown();
	} else if (ch >= 32 && ch <= 126) {
		// Add printable characters to input buffer
		inputBuffer += static_cast<char>(ch);
		changed = true;
	}

	if (changed) needRedraw = true;
}

std::string InputElement::getInput() const {
	return inputBuffer;
}

void InputElement::clearInput() {
	inputBuffer.clear();
	needRedraw = true;
}

bool InputElement::historyUp() {
	if (!history.empty() && historyIndex > 0) {
		historyIndex--;
		inputBuffer = history[historyIndex];
		needRedraw = true;
		return true;
	}
	return false;
}

bool InputElement::historyDown() {
	if (historyIndex < history.size()) {
		historyIndex++;
		if (historyIndex == history.size())
			inputBuffer.clear();
		else
			inputBuffer = history[historyIndex];
		needRedraw = true;
		return true;
	}
	return false;
}

void InputElement::addToHistory(const std::string& command) {
	if (!command.empty() && (history.empty() || command != history.back()))
		history.push_back(command);
	historyIndex = history.size();
	clearInput();
}