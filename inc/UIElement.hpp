#pragma once
#include <raylib/raylib.h>


class UIElement{
    private:
        int ID;
    public:
        UIElement(Vector2 pos, Vector2 size, Color color, int ID);
        int getID();
        Vector2 position;
        Vector2 size;
        Color color;

    
};