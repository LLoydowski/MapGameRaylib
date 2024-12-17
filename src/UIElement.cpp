#include <string>
#include <iostream>

#include "UIElement.hpp"


UIElement::UIElement(Vector2 pos, Vector2 size, Color color, std::string ID, int anchor){
    this->position = pos;
    this->size = size;
    this->color = color;
    this->ID = ID;
    this->anchor = anchor;
}

std::string UIElement::getID(){
    return ID;
}

void UIElement::drawElement(Vector2 screenSize){

    switch (this->anchor)
    {
    case 1:
        DrawRectangle(
            this->position.x * screenSize.x,
            this->position.y * screenSize.y,
            this->size.x,
            this->size.y,
            this->color
        );
        break;
    case 2:
        DrawRectangle(
            (this->position.x * screenSize.x) - this->size.x,
            this->position.y * screenSize.y,
            this->size.x,
            this->size.y,
            this->color
        );
        break;
    case 3:
        DrawRectangle(
            this->position.x * screenSize.x,
            (this->position.y * screenSize.y) - this->size.y,
            this->size.x,
            this->size.y,
            this->color
        );
        break;
    case 4:
        DrawRectangle(
            (this->position.x * screenSize.x) - this->size.x,
            (this->position.y * screenSize.y) - this->size.y,
            this->size.x,
            this->size.y,
            this->color
        );
        break;
    
    default:
        break;
    }
}

void declareWar(){
    std::cout << "SKIBADI WAR DECALRED" << std::endl;
}
