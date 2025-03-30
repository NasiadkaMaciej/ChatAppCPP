#pragma once

#include "uiElement.h"
#include <ncurses.h>
#include <string>
#include <vector>

class UserListElement : public UIElement {
  public:
    UserListElement(int height, int width, int startY, int startX);
    ~UserListElement();

    void draw() override;
    void refresh() override;
    
    void updateUsers(const std::vector<std::string>& newUsers);
    
  private:
    std::vector<std::string> users;
};