#include "uiElement.h"
#include <ncurses.h>

UIElement::UIElement(int height, int width, int startY, int startX)
  : height(height)
  , width(width)
  , startY(startY)
  , startX(startX)
  , needRedraw(true) {
	win = newwin(height, width, startY, startX);
}

void UIElement::setNeedRedraw(bool value) {
	needRedraw = value;
}

void UIElement::resize(int newHeight, int newWidth, int newStartY, int newStartX) {
	// Update dimensions and position
	height = newHeight;
	width = newWidth;
	startY = newStartY;
	startX = newStartX;

	// Delete and recreate the window
	if (win) delwin(win);
	win = newwin(height, width, startY, startX);
	needRedraw = true;
}

bool UIElement::getNeedRedraw() const {
	return needRedraw;
}