#include "chatElement.h"
#include <algorithm>

ChatElement::ChatElement(int height, int width, int startY, int startX)
  : UIElement(height, width, startY, startX)
  , scrollPosition(0) {

	win = newwin(height, width, startY, startX);
	draw();
}

void ChatElement::draw() {
	if (!win) return;

	werase(win);

	// Draw border and title
	box(win, 0, 0);

	// Display room name instead of "Chat" if available
	std::string title = roomName.empty() ? " Chat " : " " + roomName + " ";
	mvwprintw(win, 0, 2, "%s", title.c_str());

	// Calculate visible lines (accounting for border)
	int visibleLines = height - 2;

	// Adjust scroll position if needed
	if (messages.size() > static_cast<size_t>(visibleLines)) {
		if (scrollPosition > static_cast<int>(messages.size() - visibleLines))
			scrollPosition = messages.size() - visibleLines;
		if (scrollPosition < 0) scrollPosition = 0;
	} else {
		scrollPosition = 0;
	}

	// Display messages with scrolling
	size_t startIdx = scrollPosition;

	for (size_t i = 0; i < std::min(messages.size() - startIdx, static_cast<size_t>(visibleLines)); ++i)
		mvwprintw(win, i + 1, 1, "%.*s", width - 2, messages[startIdx + i].c_str());

	needRedraw = false;
}

void ChatElement::refresh() {
	if (!win) return;
	wrefresh(win);
}

void ChatElement::handleInput(int ch) {
	if (ch == KEY_DOWN) // Down Arrow and mouse scroll
		scrollDown();
	else if (ch == KEY_UP) // Up Arrow and mouse scroll
		scrollUp();
	refresh();
}

void ChatElement::addMessage(const std::string& message) {
	bool wasOnBottom = isOnBottom();
	messages.push_back(message);

	if (wasOnBottom) scrollPosition = messages.size() - (height - 2);

	needRedraw = true;
}

void ChatElement::scrollUp() {
	if (scrollPosition > 0) {
		--scrollPosition;
		needRedraw = true;
	}
}

void ChatElement::scrollDown() {
	int visibleLines = height - 2;
	if (scrollPosition < static_cast<int>(messages.size() - visibleLines)) {
		++scrollPosition;
		needRedraw = true;
	}
}

bool ChatElement::isOnBottom() const {
	int visibleLines = height - 2;
	return scrollPosition >= static_cast<int>(messages.size() - visibleLines);
}

void ChatElement::setRoomName(const std::string& name) {
	roomName = name;
	needRedraw = true;
}