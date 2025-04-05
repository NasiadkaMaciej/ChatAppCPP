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
    mvwprintw(win, 0, 2, " Chat ");
    
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
        mvwprintw(win, i + 1, 1, "%.*s", width-2, messages[startIdx + i].c_str());
    
    needRedraw = false;
}

void ChatElement::refresh() {
    if (win) wrefresh(win);
}

void ChatElement::handleInput(int ch) {
    if (ch == KEY_NPAGE) { // Page Down
        scrollPageDown();
    } else if (ch == KEY_PPAGE) { // Page Up
        scrollPageUp();
    }
}

void ChatElement::addMessage(const std::string& message) {
    bool wasOnBottom = isOnBottom();
    messages.push_back(message);
    
    if (wasOnBottom) 
        scrollPosition = messages.size() - (height - 2);
    
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

void ChatElement::scrollPageUp() {
    scrollPosition = std::max(0, scrollPosition - (height - 2) / 2);
    needRedraw = true;
}

void ChatElement::scrollPageDown() {
    int visibleLines = height - 2;
    int maxScroll = std::max(0, static_cast<int>(messages.size()) - visibleLines);
    scrollPosition = std::min(maxScroll, scrollPosition + (height - 2) / 2);
    needRedraw = true;
}

bool ChatElement::isOnBottom() const {
    int visibleLines = height - 2;
    return scrollPosition >= static_cast<int>(messages.size() - visibleLines);
}