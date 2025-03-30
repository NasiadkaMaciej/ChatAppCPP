#include "statusElement.h"

StatusElement::StatusElement(int height, int width, int startY, int startX)
    : UIElement(height, width, startY, startX) {
    
    win = newwin(height, width, startY, startX);
    draw();
}

StatusElement::~StatusElement() {
    if (win) delwin(win);
}

void StatusElement::draw() {
    wclear(win);
    mvwprintw(win, 0, 0, "%s", statusMessage.c_str());
    needRedraw = false;
}

void StatusElement::refresh() {
    wrefresh(win);
}

void StatusElement::setStatus(const std::string& message) {
    statusMessage = message;
    needRedraw = true;
}