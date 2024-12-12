#pragma once

#include <raylib/raylib.h>
#include <string>

void printColor(Color &color);
std::string ColorToHexString(Color color);
bool isInTextureBounds(Vector2 worldPos, Vector2 texturePosition, Texture2D &texture);
bool isInTextureBounds(Vector2 worldPos, Vector2 texturePosition, Image &textureImage);
bool areColorsEqual(Color color1, Color color2);

struct StateData{
    int population;
    int economy;
    std::string stateID;
    std::string country;

    void Clear();
    bool isStateOpened();
};