#include <nlohmann/json.hpp>
#include <raylib/raylib.h>
#include <raylib/rlgl.h>
#include <raylib/raymath.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "utils.hpp"
#include "UIElement.hpp"

using json = nlohmann::json;


const int screenWidth = 800;
const int screenHeight = 450;

json colorCodes;
json statesData;

StateData focusedState;

void getClickedState(Camera2D& camera, Image& map){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 mousePos = GetMousePosition(); //? Gets mouse position
        Vector2 worldPos = GetScreenToWorld2D(mousePos, camera); //? Converts screen position to world position

        Vector2 textureSize = {screenWidth / 4, screenHeight / 4};

        if(!isInTextureBounds(worldPos, textureSize, map)){
            return;
        }

        Color color = GetImageColor(map, worldPos.x - textureSize.x , worldPos.y - textureSize.y); //? Gets color of the pressed pixel
        std::string colorStr = ColorToHexString(color);

        std::cout << colorStr << std::endl; 
        if(colorStr == "000000"){
            return;
        }

        const char* state = std::string(colorCodes[colorStr]).c_str();
        
        if(!statesData[state]["country"].is_null()){
            focusedState.country = statesData[state]["country"];
        }
        if(!statesData[state]["economy"].is_null()){
            focusedState.economy = statesData[state]["economy"];
        }
        if(!statesData[state]["population"].is_null()){
            focusedState.population = statesData[state]["population"];
        }
        
        
    }   
}

void HandleUIEvents(std::vector<UIElement> &UIElements){
    Vector2 mousePos = GetMousePosition();
    Rectangle cursorHitBox = {mousePos.x, mousePos.y, 1, 1};

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        
        
        for(UIElement &element : UIElements){
            Rectangle UIHitbox = {element.position.x, element.position.y, element.size.x, element.size.y};
            int ID = element.getID();

            if(CheckCollisionRecs(cursorHitBox, UIHitbox)){
                if(ID == 1){
                    // element.color = GREEN;
                }
                break;
            }
        }

    }

    for(UIElement &element : UIElements){
            Rectangle UIHitbox = {element.position.x, element.position.y, element.size.x, element.size.y};
            int ID = element.getID();

            if(CheckCollisionRecs(cursorHitBox, UIHitbox)){
                if(ID == 1 && !areColorsEqual(element.color, GREEN)){
                    element.color = BLUE;
                }
                break;
            }else{
                if(ID == 1 && !areColorsEqual(element.color, GREEN)){
                    element.color = BLACK;
                }
            }
    }
}

void generateUI(std::vector<UIElement> &elements){
    elements.push_back(UIElement({screenWidth - 200, screenHeight - 100}, {200, 100}, BLACK, 1));
    elements.push_back(UIElement({0, screenHeight - 50}, {100, 50}, BLACK, 2));
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Super Giga Map Game");

    SetTargetFPS(60);

    Image map = LoadImage("gfx/maps/PL01.png");
    Texture2D mapTexture = LoadTextureFromImage(map);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    int zoomMode = 0;

    std::ifstream colorCodesStream("gfx/maps/PL01.json");
    colorCodes = json::parse(colorCodesStream);

    std::ifstream stateDataStream("data/PL01.json");
    statesData = json::parse(stateDataStream);

    std::vector<UIElement> UIElements;

    generateUI(UIElements);

    

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

        getClickedState(camera, map);
        HandleUIEvents(UIElements);

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

            //? UI

            for(UIElement element : UIElements){
                DrawRectangle(element.position.x, element.position.y, element.size.x, element.size.y, element.color);
            }

            
            const char* country = ("Owner: " + focusedState.country).c_str();
            const char* population = ("Population: " + std::to_string(focusedState.population)).c_str();
            const char* economy = ("Economy: " + std::to_string(focusedState.economy)).c_str();

            DrawText(country, 0, screenHeight - 50, 10, WHITE);
            DrawText(population, 0, screenHeight - 30, 10, WHITE);
            DrawText(economy, 0, screenHeight - 10, 10, WHITE);


        EndDrawing();
    }

    UnloadImage(map);
    CloseWindow();

    return 0;
}