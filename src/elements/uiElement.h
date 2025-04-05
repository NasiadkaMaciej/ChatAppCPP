#pragma once
#include <ncurses.h>

class UIElement {
  public:
	UIElement(int height, int width, int startY, int startX);

	// Draw the UI element
	virtual void draw() = 0;

	// Refresh the UI element
	virtual void refresh() = 0;

	// Resize the UI element
	void resize(int newHeight, int newWidth, int newStartY, int newStartX);

	// Set/get the need to redraw flag
	void setNeedRedraw(bool value);
	bool getNeedRedraw() const;

	WINDOW* getWindow() const { return win; }

  protected:
	WINDOW* win;
	int height;
	int width;
	int startY;
	int startX;
	bool needRedraw;
};