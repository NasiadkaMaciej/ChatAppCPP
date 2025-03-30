#include "uiElement.h"

UIElement::UIElement(int height, int width, int startY, int startX)
    : height(height)
    , width(width)
    , startY(startY)
    , startX(startX)
    , needRedraw(true) {
}

UIElement::~UIElement() {
}

void UIElement::setNeedRedraw(bool value) {
    needRedraw = value;
}

bool UIElement::getNeedRedraw() const {
    return needRedraw;
}