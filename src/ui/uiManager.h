#pragma once

#include "elements/chatElement.h"
#include "elements/inputElement.h"
#include "elements/statusElement.h"
#include "elements/userListElement.h"
#include <functional>
#include <memory>
#include <vector>

class UIManager {
  public:
	UIManager();
	~UIManager();

	void init();
	void cleanup();

	// Get UI elements
	ChatElement* getChatElement() const { return chatElement.get(); }
	InputElement* getInputElement() const { return inputElement.get(); }
	UserListElement* getUserListElement() const { return userListElement.get(); }
	StatusElement* getStatusElement() const { return statusElement.get(); }

	// Resize handler
	void handleResize();

	// Refresh all elements that need redrawing
	void refreshElements();

  private:
	// UI elements
	std::unique_ptr<ChatElement> chatElement;
	std::unique_ptr<InputElement> inputElement;
	std::unique_ptr<UserListElement> userListElement;
	std::unique_ptr<StatusElement> statusElement;

	// List of all elements for easier iteration
	std::vector<UIElement*> elements;

	// Window dimensions and positions
	int chatHeight, chatWidth;
	int inputHeight, inputWidth;
	int userListHeight, userListWidth;
	int statusHeight;

	// Initialize windows
	void initWindows();
	void setupWindows(bool initialSetup);
};