#include "UIElement.hpp"
#include <string>

UIElement::UIElement(Vector2 pos, Vector2 size, Color color, std::string ID){
    this->position = pos;
    this->size = size;
    this->color = color;
    this->ID = ID;
}

std::string UIElement::getID(){
    return ID;
}