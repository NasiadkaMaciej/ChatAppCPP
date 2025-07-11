#pragma once

#include "uiElement.h"
#include <ncurses.h>
#include <string>
#include <vector>

class ChatElement : public UIElement {
  public:
	ChatElement(int height, int width, int startY, int startX);

	void draw() override;
	void refresh() override;
	void handleInput(int ch);

	void addMessage(const std::string& message);

	void scrollUp();
	void scrollDown();
	void scrollPageUp();
	void scrollPageDown();

	bool isOnBottom() const;
	void setRoomName(const std::string& name);

  private:
	std::vector<std::string> messages;
	int scrollPosition;
	std::string roomName;
};