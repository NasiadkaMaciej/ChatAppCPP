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
	void handleInput(wint_t ch, bool isSpecialKey);

	std::string getInput() const;

	void clearInput();

  private:
	std::wstring inputBuffer;
	size_t cursorPos;
};