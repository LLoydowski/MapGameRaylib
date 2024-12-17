#pragma once
#include <raylib/raylib.h>
#include <string>

class UIElement{
    private:
        std::string ID;
    public:
        UIElement(Vector2 pos, Vector2 size, Color color, std::string ID, int anchor);
        void drawElement(Vector2 screenSize);
        std::string getID();
        Vector2 position;
        Vector2 size;
        Color color;
        int anchor;

    
};

void declareWar();