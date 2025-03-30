#pragma once

#include "uiElement.h"
#include <ncurses.h>
#include <string>
#include <vector>

class InputElement : public UIElement {
  public:
    InputElement(int height, int width, int startY, int startX);
    ~InputElement();

    void draw() override;
    void refresh() override;
    void handleInput(int ch);
    
    std::string getInput() const;
    
    void clearInput();
    
    bool historyUp();
    bool historyDown();
    
    void addToHistory(const std::string& command);
    
  private:
    WINDOW* win;
    std::string inputBuffer;
    std::vector<std::string> history;
    size_t historyIndex;
};