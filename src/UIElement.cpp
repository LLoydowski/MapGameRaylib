#include "UIElement.hpp"


UIElement::UIElement(Vector2 pos, Vector2 size, Color color, int ID){
    this->position = pos;
    this->size = size;
    this->color = color;
    this->ID = ID;
}

int UIElement::getID(){
    return ID;
}