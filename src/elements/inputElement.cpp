#include "inputElement.h"
#include <algorithm>

InputElement::InputElement(int height, int width, int startY, int startX)
  : UIElement(height, width, startY, startX)
  , cursorPos(0) {

	win = newwin(height, width, startY, startX);
	keypad(win, TRUE);
	wtimeout(win, 50);
	draw();
}

void InputElement::draw() {
	werase(win);
	mvwprintw(win, 0, 0, "> %s", inputBuffer.c_str());
	needRedraw = false;
}

void InputElement::refresh() {
	if (!win) return;
	wmove(win, 0, cursorPos + 2); // +2 for "> " prompt
	wrefresh(win);
}

void InputElement::handleInput(int ch) {
	bool changed = false;

	if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
		if (cursorPos > 0) {
			inputBuffer.erase(cursorPos - 1, 1);
			cursorPos--;
			changed = true;
		}
	} else if (ch == KEY_DC) { // Delete key
		if (cursorPos < inputBuffer.length()) {
			inputBuffer.erase(cursorPos, 1);
			changed = true;
		}
	} else if (ch == KEY_LEFT) { // Left Arrow
		if (cursorPos > 0) {
			cursorPos--;
			changed = true;
		}
	} else if (ch == KEY_RIGHT) { // Right Arrow
		if (cursorPos < inputBuffer.length()) {
			cursorPos++;
			changed = true;
		}
	} else if (ch == KEY_HOME) { // Home key
		cursorPos = 0;
		changed = true;
	} else if (ch == KEY_END) { // End key
		cursorPos = inputBuffer.length();
		changed = true;
	} else if (ch >= 32 && ch <= 126) {
		// Insert character at cursor position
		inputBuffer.insert(cursorPos, 1, static_cast<char>(ch));
		cursorPos++;
		changed = true;
	}

	if (changed) needRedraw = true;
}

std::string InputElement::getInput() const {
	return inputBuffer;
}

void InputElement::clearInput() {
	inputBuffer.clear();
	cursorPos = 0;
	needRedraw = true;
}