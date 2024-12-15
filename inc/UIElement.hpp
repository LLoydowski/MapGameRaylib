#pragma once
#include <raylib/raylib.h>
#include <string>

class UIElement{
    private:
        std::string ID;
    public:
        UIElement(Vector2 pos, Vector2 size, Color color, std::string ID);
        std::string getID();
        Vector2 position;
        Vector2 size;
        Color color;

    
};

void declareWar();