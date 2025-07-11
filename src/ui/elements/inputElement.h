#pragma once

#include "uiElement.h"
#include <functional>
#include <ncurses.h>
#include <string>
#include <vector>

class InputElement : public UIElement {
  public:
	using InputCallback = std::function<void(const std::string&)>;

	InputElement(int height, int width, int startY, int startX);

	void draw() override;
	void refresh() override;

	// Set callback for input submission
	void setInputCallback(InputCallback callback);

	// Handle all input processing in one place
	bool processInput(wint_t ch, bool isSpecialKey);

	std::string getInput() const;
	void clearInput();

  private:
	std::wstring inputBuffer;
	size_t cursorPos;
	InputCallback onInputSubmitted;

	// History feature
	std::vector<std::wstring> inputHistory;
	size_t historyPosition;
};