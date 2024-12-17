#include <nlohmann/json.hpp>
#include <raylib/raylib.h>
#include <raylib/rlgl.h>
#include <raylib/raymath.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "utils.hpp"
#include "UIElement.hpp"

using json = nlohmann::json;

Color getScreenColor(int x, int y){
    Image screen = LoadImageFromScreen();

    Color color = GetImageColor(screen, x, y);

    UnloadImage(screen);

    return color;
}

void renderCountries(json &colorCodes, Vector2 &screenSize){

    Image screenImage = LoadImageFromScreen();
    Color *pixels = LoadImageColors(screenImage);

    auto colors = colorCodes.items();
    
    // for(auto element : colors){
    //     int colorHex = stoi(std::string(element.key()));
    //     Color color = GetColor(colorHex);

        for (int y = 0; y < screenSize.y; y++) { 
            for (int x = 0; x < screenSize.x; x++) { 
                int pixelIndex = y * screenSize.x + x; 
                if (areColorsEqual(pixels[pixelIndex], GetColor(0x00394b))) { 
                    // Found a matching pixel (do something with this information)
                    DrawRectangle(x, y, 1, 1, GREEN);
                    DrawCircle(x, y, 2, GREEN); // Highlight it on the screen
                } 
            } 
        }
    // }


    UnloadImage(screenImage);

}

void getClickedState(Camera2D& camera, Image& map, StateData &focusedState, json &statesData, json &colorCodes, Vector2 &screenSize){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 mousePos = GetMousePosition(); //? Gets mouse position
        Vector2 worldPos = GetScreenToWorld2D(mousePos, camera); //? Converts screen position to world position

        Vector2 textureSize = {screenSize.x / 4, screenSize.y / 4};

        if(!isInTextureBounds(worldPos, textureSize, map)){
            return;
        }

        Color color = GetImageColor(map, worldPos.x - textureSize.x , worldPos.y - textureSize.y); //? Gets color of the pressed pixel
        std::string colorStr = ColorToHexString(color);

        if(colorStr == "000000" || colorStr == "191919" || colorStr == "ffffff"){
            return;
        }

        const char* state = std::string(colorCodes[colorStr]).c_str();
        
        if(!statesData[state].is_null()){
            focusedState.stateID = state;
        }
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

void saveFocusedState(json &statesData, StateData &focusedState){
    statesData[focusedState.stateID]["country"] = focusedState.country;
    statesData[focusedState.stateID]["economy"] = focusedState.economy;
    statesData[focusedState.stateID]["population"] = focusedState.population;
}

void showSliderUI(std::vector<UIElement> &UIElements, Vector2 &screenSize){
    UIElement sliderBg({0, screenSize.y - 50}, {screenSize.x, 100}, GRAY, "sliderBG", 0);
    UIElement slider({0, screenSize.y - 50}, {screenSize.x / 2, 100}, BLUE, "slider", 0);

    UIElements.push_back(sliderBg);
    UIElements.push_back(slider);
}

void hideSliderUI(std::vector<UIElement> &UIElements){
    int bgIndex = -1;
    int sliderIndex = -1;

    for(int i = 0; i < UIElements.size(); i++){
        UIElement* element = &UIElements[i];

        if(element->getID() == "sliderBG"){
           bgIndex = i;
        }
        if(element->getID() == "slider"){
            sliderIndex = i;
        }
    }
    if(bgIndex != -1 && sliderIndex != -1){
        UIElements.erase(UIElements.begin() + bgIndex);
        UIElements.erase(UIElements.begin() + sliderIndex);
    }
}

void HandleKeyboardEvents(std::vector<UIElement> &UIElements, StateData &focusedState, Vector2 &screenSize){
    if(IsKeyPressed(KEY_R)){
        if(focusedState.isStateOpened()){
            showSliderUI(UIElements, screenSize);
        }else{
            std::cout << "Please click on state first" << std::endl;
        }
    }
    if(IsKeyPressed(KEY_C)){
        hideSliderUI(UIElements);
    }
}

bool HandleUIEvents(std::vector<UIElement> &UIElements, Vector2 screenSize){
    Vector2 mousePos = GetMousePosition();

    for(UIElement &element : UIElements){
        Rectangle UIHitbox;
        std::string ID = element.getID();

        switch (element.anchor)
        {
            case 1:
                UIHitbox = Rectangle({
                    element.position.x * screenSize.x,
                    element.position.y * screenSize.y,
                    element.size.x,
                    element.size.y}
                );
                break;
            case 2:
                UIHitbox = Rectangle({
                    (element.position.x * screenSize.x) - element.size.x,
                    element.position.y * screenSize.y,
                    element.size.x,
                    element.size.y}
                );
                break;
            case 3:
                UIHitbox = Rectangle({
                    element.position.x * screenSize.x,
                    (element.position.y * screenSize.y) - element.size.y,
                    element.size.x,
                    element.size.y}
                );
                break;
            case 4:
                UIHitbox = Rectangle({
                    (element.position.x * screenSize.x) - element.size.x,
                    (element.position.y * screenSize.y) - element.size.y,
                    element.size.x,
                    element.size.y}
                );
                break;
        }

        if(CheckCollisionPointRec(mousePos, UIHitbox)){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(ID == "declareWar"){
                    declareWar();
                }
            }

            if(ID == "nextTurn" && !areColorsEqual(element.color, GREEN)){
                element.color = BLUE;
            }
        
            return true;
        }else{
            if(ID == "nextTurn" && !areColorsEqual(element.color, GREEN)){
                element.color = BLACK;
            }
        }
    }

    return false;
}

constexpr unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + static_cast<unsigned int>(*str++);
    }
    return hash;
}

void generateUI(std::vector<UIElement> &elements, Vector2 &screenSize){
    //? LEFT TOP
    // elements.push_back(UIElement({0, 0}, {100, 100}, BLUE, "test", 1));
    //? RIGHT TOP
    // elements.push_back(UIElement({1, 0}, {100, 100}, RED, "test", 2));
    //? LEFT BOT
    // elements.push_back(UIElement({0, 1}, {100, 100}, YELLOW, "test", 3));
    //? RIGHT BOT
    // elements.push_back(UIElement({1, 1}, {100, 100}, BLACK, "test", 4));

    elements.push_back(UIElement({1, 1}, {100, 100}, BLACK, "declareWar", 4));
    elements.push_back(UIElement({0, screenSize.y - 125}, {350, 250}, GetColor(0x050505AA), "infoPanel", 4));
}

void handleInputs(std::vector<UIElement> &elements, Camera2D &camera, Image &mapImage, StateData &focusedState, json &statesData, json &colorCodes, Vector2 &screenSize){
    bool uiEventHappened = false;
    uiEventHappened = HandleUIEvents(elements, screenSize);

    if(uiEventHappened){
        return;
    }
    
    getClickedState(camera, mapImage, focusedState, statesData, colorCodes, screenSize); 

    HandleKeyboardEvents(elements, focusedState, screenSize);
}

int main()
{
    int screenWidth = 1400;
    int screenHeight = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 

    // int monitor = GetCurrentMonitor();
    // SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    // ToggleFullscreen();

    Vector2 screenSize = {screenWidth, screenHeight};

    InitWindow(screenWidth, screenHeight, "Super Giga Map Game");

    SetTargetFPS(60);

    Image map = LoadImage("gfx/maps/PL01OUTLINED.png");
    Texture2D mapTexture = LoadTextureFromImage(map);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    int zoomMode = 0;


    json colorCodes;
    json statesData;

    StateData focusedState;

    std::ifstream colorCodesStream("gfx/maps/PL01.json");
    colorCodes = json::parse(colorCodesStream);

    std::ifstream stateDataStream("data/PL01.json");
    statesData = json::parse(stateDataStream);

    std::vector<UIElement> UIElements;

    generateUI(UIElements, screenSize);

    
    while (!WindowShouldClose()) 
    {

        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        screenSize = {float(screenWidth), float(screenHeight)};

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

        handleInputs(UIElements, camera, map, focusedState, statesData, colorCodes, screenSize);

        // bool uiEventHappened = HandleUIEvents(UIElements);

        // if(uiEventHappened){
        //     continue;
        // }
        
        // getClickedState(camera, map); 
    
        // HandleKeyboardEvents(UIElements);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

            rlPushMatrix();
                rlTranslatef(0, 25*50, 0);
                rlRotatef(90, 1, 0, 0);
                DrawGrid(100, 50);
            rlPopMatrix();

            DrawTexture(mapTexture, screenWidth / 4, screenHeight / 4, WHITE);
            // DrawRectangle(0, 0, 400, 400, GetColor(0xFF0055));

            
            EndMode2D();

            Image screenImage = LoadImageFromScreen();
            Color *pixels = LoadImageColors(screenImage);


            auto colors = colorCodes.items();
            
            for(auto element : colors){
                std::string colorStr = std::string(element.key());
                std::string colorStrAlpha = colorStr + "FF";
                Color color;

                try {
                    unsigned int colorHex = std::stoul(colorStrAlpha, nullptr, 16); // Correct hex conversion
                    color = GetColor(colorHex); // Use the same `color` variable
                } catch (const std::exception &e) {
                    std::cerr << "Failed to parse colorStr as hex: " << colorStrAlpha << " (" << e.what() << ")" << std::endl;
                    continue;
                }

                std::string stateName = colorCodes[colorStr];

                if(statesData[stateName]["country"].is_null()){
                    continue;
                }
                std::string country(statesData[stateName]["country"]);


                for (int y = 0; y < screenHeight; y++) { 
                    for (int x = 0; x < screenWidth; x++) { 
                        int pixelIndex = y * screenWidth + x; 
                        if (areColorsEqual(pixels[pixelIndex], color)) { 
                            // Found a matching pixel (do something with this information)
                            switch(hash(country.c_str())){
                                case hash("POL"):
                                    DrawRectangle(x, y, 1, 1, RED);
                                    break;
                                case hash("SKIBADI"):
                                    DrawRectangle(x, y, 1, 1, BLUE);
                                    break;
                                default:
                                    DrawRectangle(x, y, 1, 1, GREEN);
                                    break;
                            }
                        } 
                    } 
                }
            }


            UnloadImage(screenImage);

            //? UI

            for(UIElement element : UIElements){
                element.drawElement(screenSize);
            }

            
            const char* country = ("Owner: " + focusedState.country).c_str();
            const char* population = ("Population: " + std::to_string(focusedState.population)).c_str();
            const char* economy = ("Economy: " + std::to_string(focusedState.economy)).c_str();

            DrawText(country, 10, 10, 16, BLACK);
            DrawText(population, 10, 30, 16, BLACK);
            DrawText(economy, 10, 50, 16, BLACK);


        EndDrawing();
    }

    UnloadImage(map);
    CloseWindow();

    return 0;
}