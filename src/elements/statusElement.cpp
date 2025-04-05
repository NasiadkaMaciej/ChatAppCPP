#include "statusElement.h"

StatusElement::StatusElement(int height, int width, int startY, int startX)
  : UIElement(height, width, startY, startX) {

	win = newwin(height, width, startY, startX);
	draw();
}

void StatusElement::draw() {
	if (!win) return;

	werase(win);
	// Add padding to prevent text from touching the edge
	mvwprintw(win, 0, 1, "%.*s", width - 2, statusMessage.c_str());
	needRedraw = false;
}

void StatusElement::refresh() {
	wrefresh(win);
}

void StatusElement::setStatus(const std::string& message) {
	statusMessage = message;
	needRedraw = true;
}