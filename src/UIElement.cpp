#include <string>
#include <iostream>

#include "UIElement.hpp"


UIElement::UIElement(Vector2 pos, Vector2 size, Color color, std::string ID){
    this->position = pos;
    this->size = size;
    this->color = color;
    this->ID = ID;
}

std::string UIElement::getID(){
    return ID;
}

void declareWar(){
    std::cout << "SKIBADI WAR DECALRED" << std::endl;
}
