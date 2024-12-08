#include <nlohmann/json.hpp>
#include <raylib/raylib.h>
#include <raylib/rlgl.h>
#include <raylib/raymath.h>
#include <iostream>
#include <fstream>
#include <string>

#include "utils.hpp"

using json = nlohmann::json;

int main()
{

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Super Giga Map Game");

    SetTargetFPS(60);

    Image map = LoadImage("gfx/maps/PL01.png");
    Texture2D mapTexture = LoadTextureFromImage(map);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    int zoomMode = 0;

    std::ifstream f("gfx/maps/PL01.json");
    json colorCodes = json::parse(f);


    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_ONE)) zoomMode = 0;
        else if (IsKeyPressed(KEY_TWO)) zoomMode = 1;
        
        // Translate based on mouse right click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        if (zoomMode == 0)
        {
            // Zoom based on mouse wheel
            float wheel = GetMouseWheelMove();
            if (wheel != 0)
            {
                // Get the world point that is under the mouse
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                // Set the offset to where the mouse is
                camera.offset = GetMousePosition();

                // Set the target to match, so that the camera maps the world space point 
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;

                // Zoom increment
                float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
                if (wheel < 0) scaleFactor = 1.0f/scaleFactor;
                camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
            }
        }
        else
        {
            // Zoom based on mouse right click
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                // Get the world point that is under the mouse
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                // Set the offset to where the mouse is
                camera.offset = GetMousePosition();

                // Set the target to match, so that the camera maps the world space point 
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                // Zoom increment
                float deltaX = GetMouseDelta().x;
                float scaleFactor = 1.0f + (0.01f*fabsf(deltaX));
                if (deltaX < 0) scaleFactor = 1.0f/scaleFactor;
                camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
            }
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
           
            Vector2 mousePos = GetMousePosition(); //? Gets mouse position
            Vector2 worldPos = GetScreenToWorld2D(mousePos, camera); //? Converts screen position to world position

            Color color = GetImageColor(map, worldPos.x - screenWidth / 4 , worldPos.y - screenHeight / 4); //? Gets color of the pressed pixel

            std::string colorStr = ColorToHexString(color);

            std::cout << colorCodes[colorStr]<< std::endl;
        }   

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

            rlPushMatrix();
                rlTranslatef(0, 25*50, 0);
                rlRotatef(90, 1, 0, 0);
                DrawGrid(100, 50);
            rlPopMatrix();

            DrawTexture(mapTexture, screenWidth / 4, screenHeight / 4, WHITE);
            DrawRectangle(0, 0, 400, 400, GetColor(0xFF0055));
            EndMode2D();


        EndDrawing();
    }

    UnloadImage(map);
    CloseWindow();

    return 0;
}