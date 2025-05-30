#pragma once

#include "uiElement.h"
#include <ncurses.h>
#include <string>
#include <vector>

class InputElement : public UIElement {
  public:
	InputElement(int height, int width, int startY, int startX);

	void draw() override;
	void refresh() override;
	void handleInput(int ch);

	std::string getInput() const;

	void clearInput();

  private:
	std::string inputBuffer;
	size_t cursorPos;
};