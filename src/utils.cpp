#include <raylib/raylib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include "utils.hpp"

void printColor(Color &color){
    std::cout
            << "R: " << (int)color.r << " "
            << "G: " << (int)color.g << " "
            << "B: " << (int)color.b << " "
            << "A: " << (int)color.a << std::endl;
}

std::string ColorToHexString(Color color) {
    std::ostringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    hexStream << std::setw(2) << (int)color.r;
    hexStream << std::setw(2) << (int)color.g;
    hexStream << std::setw(2) << (int)color.b;
    // hexStream << std::setw(2) << (int)color.a; // Optional: Include alpha (RGBA)
    return hexStream.str();
}

bool isInTextureBounds(Vector2 worldPos, Vector2 texturePosition, Texture2D &texture){
    bool inBounds = (worldPos.x >= texturePosition.x && 
        worldPos.x <= texturePosition.x + texture.width &&
        worldPos.y >= texturePosition.y &&
        worldPos.y <= texturePosition.y + texture.height);

    return inBounds;
}

bool isInTextureBounds(Vector2 worldPos, Vector2 texturePosition, Image &textureImage){

    Texture2D texture = LoadTextureFromImage(textureImage);

    bool inBounds = (worldPos.x >= texturePosition.x && 
        worldPos.x <= texturePosition.x + texture.width &&
        worldPos.y >= texturePosition.y &&
        worldPos.y <= texturePosition.y + texture.height);

    UnloadTexture(texture);

    return inBounds;
}

bool areColorsEqual(Color color1, Color color2) {
    return (color1.r == color2.r &&
            color1.g == color2.g &&
            color1.b == color2.b &&
            color1.a == color2.a);
}

void StateData::Clear(){
    population = 0;
    economy = 0;
    country = "";
}

bool StateData::isStateOpened(){
    if(population != 0 && country != ""){
        return true;
    }
    return false;
}