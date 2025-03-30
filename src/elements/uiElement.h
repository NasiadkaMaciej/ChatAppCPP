#pragma once

class UIElement {
  public:
    UIElement(int height, int width, int startY, int startX);
    virtual ~UIElement();

    // Draw the UI element
    virtual void draw() = 0;

    // Refresh the UI element
    virtual void refresh() = 0;

    // Set/get the need to redraw flag
    void setNeedRedraw(bool value);
    bool getNeedRedraw() const;

  protected:
    int height;
    int width;
    int startY;
    int startX;
    bool needRedraw;
};