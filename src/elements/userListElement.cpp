#include "userListElement.h"
#include <algorithm>

UserListElement::UserListElement(int height, int width, int startY, int startX)
  : UIElement(height, width, startY, startX) {

	win = newwin(height, width, startY, startX);
	draw();
}

UserListElement::~UserListElement() {
	if (win) delwin(win);
}

void UserListElement::draw() {
	// Draw border and title
	box(win, 0, 0);
	mvwprintw(win, 0, 2, " Users ");

	// Display users
	int visibleLines = height - 2;
	if (users.empty()) {
		mvwprintw(win, 1, 1, "Not in a room");
	} else {
		for (size_t i = 0; i < std::min(users.size(), static_cast<size_t>(visibleLines)); ++i)
			mvwprintw(win, i + 1, 1, "%s", users[i].c_str());
	}

	needRedraw = false;
}

void UserListElement::refresh() {
	wrefresh(win);
}

void UserListElement::updateUsers(const std::vector<std::string>& newUsers) {
	users = newUsers;
	needRedraw = true;
}