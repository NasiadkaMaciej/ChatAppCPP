#include "inputElement.h"

InputElement::InputElement(int height, int width, int startY, int startX)
  : UIElement(height, width, startY, startX) {

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