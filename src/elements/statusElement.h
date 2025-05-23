#pragma once

#include "uiElement.h"
#include <ncurses.h>
#include <string>

class StatusElement : public UIElement {
  public:
    StatusElement(int height, int width, int startY, int startX);

    void draw() override;
    void refresh() override;
    
    void setStatus(const std::string& message);
    
  private:
    std::string statusMessage;
};