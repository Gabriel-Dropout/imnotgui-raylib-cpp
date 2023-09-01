#include <iostream>
#include <string>
#include <vector>

#include "Vector2.hpp"
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "imnotgui.hpp"
#include "imnotgui_extra.hpp"

#define RTEXLOADER_IMPLEMENTATION
#include "include/rtexloader.hpp"

using namespace imnotgui;
using namespace imnotgui::draw;
using namespace imnotgui::element;

/*
<(-)\
   _/_
  |___|-.
    <|
    _|

(O)


*/

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "ImNotGUI Synchronization Example");
    SetTargetFPS(60);

    initAtlas("resources/atlas_sync.png");

    const std::vector<std::string> tabText = {"Intbox", "Floatbox"};
    int tabIdx = 0;

    std::string intboxText = "";
    int intValue = 0;

    std::string floatboxText = "";
    float floatValue = 0;

    int globalTimer = 0;
    raylib::Vector2 facePos(700, 380),
                    eye1Pos(-50, -70),
                    eye2Pos(20, -70),
                    mouthPos(-50, -10),
                    capPos(0, -120),
                    bubblePos(120, -220);

    while(!WindowShouldClose()) {
        globalTimer++;

        bool fever = (tabIdx==0 && intValue==100) || (tabIdx==1 && floatValue>99.0f);
        raylib::Vector2 _faceDelta, _eye1Delta, _eye2Delta, _mouthDelta, _capDelta, _bubbleDelta;
        if(!fever) {
            _faceDelta = raylib::Vector2{20*sinf(globalTimer*8*DEG2RAD), 30*sinf((globalTimer*6 + 60)*DEG2RAD)};
            _eye1Delta = raylib::Vector2{6*sinf((globalTimer*12 - 60)*DEG2RAD), 6*sinf((globalTimer*9 + 20)*DEG2RAD)};
            _eye2Delta = raylib::Vector2{6*sinf((globalTimer*5 - 120)*DEG2RAD), 6*sinf((globalTimer*5 + 180)*DEG2RAD)};
            _mouthDelta = raylib::Vector2{8*sinf((globalTimer*7 - 10)*DEG2RAD), 5*sinf((globalTimer*14 + 10)*DEG2RAD)};
            _capDelta = raylib::Vector2{10*sinf((globalTimer*3 + 50)*DEG2RAD), 10*sinf((globalTimer*3 + 140)*DEG2RAD)};
            _bubbleDelta = raylib::Vector2{0, 10*sinf((globalTimer*4)*DEG2RAD)};
        } else {
            _faceDelta = raylib::Vector2{25*sinf(globalTimer*16*DEG2RAD), 30*sinf((globalTimer*24 + 60)*DEG2RAD)};
            _eye1Delta = raylib::Vector2{8*sinf((globalTimer*23 - 60)*DEG2RAD), 6*sinf((globalTimer*18 + 20)*DEG2RAD)};
            _eye2Delta = raylib::Vector2{8*sinf((globalTimer*10 - 120)*DEG2RAD), 6*sinf((globalTimer*10 + 180)*DEG2RAD)};
            _mouthDelta = raylib::Vector2{12*sinf((globalTimer*14 - 10)*DEG2RAD), 5*sinf((globalTimer*28 + 10)*DEG2RAD)};
            _capDelta = raylib::Vector2{14*sinf((globalTimer*6 + 50)*DEG2RAD), 10*sinf((globalTimer*6 + 140)*DEG2RAD)};
            _bubbleDelta = raylib::Vector2{0, 15*sinf((globalTimer*8)*DEG2RAD)};
        }


        raylib::Vector2 _facePos =  facePos + _faceDelta;
        raylib::Vector2 _eye1Pos =  _facePos + eye1Pos + _eye1Delta;
        raylib::Vector2 _eye2Pos =  _facePos + eye2Pos + _eye2Delta;
        raylib::Vector2 _mouthPos =  _facePos + mouthPos + _mouthDelta;
        raylib::Vector2 _capPos =  _facePos + capPos + _capDelta;
        raylib::Vector2 _bubblePos =  facePos + bubblePos + _bubbleDelta;

        BeginDrawing();
        ClearBackground(imnotgui::iuDark2);
        iui_begin();

        // backdrop
        iui_rect(42, 0, 1196, 85, imnotgui::iuHellaDark);
        iui_rect(42, 85, 1196, 600, imnotgui::iuDark);
        iui_rect(42, 680, 1196, 40, imnotgui::iuHellaDark);
        iui_rect(62, 85, 450, 595, imnotgui::iuDark2);

        switch(iui_tab(42, 30, 200, 55, {"Intbox", "Floatbox"}, tabIdx, IUI_TAB_TRIM)) {
        case 0: {
            iui_setFontSize(25);
            iui_label_shadow(104, 120, "Enter any integer!", RAYWHITE, 5, 5, BLACK);
            iui_setFontSize(20);
            iui_intbox(104, 170, 300, 50, intboxText, intValue, "INTBOX");
            iui_slider_h(104, 280, intValue, 300, -100, 100, "INTSLIDER_H");
            iui_slider_v(464, 100, intValue, 200, -100, 100, "INTSLIDER_V");
            iui_setFontSize(25);
            iui_label_shadow(104, 450, "<(-)\\", WHITE, 5, 5, BLACK);
            iui_label_shadow(102, 480, "   _/_   <- What a nice flamingo!", WHITE, 5, 5, BLACK);
            iui_label_shadow(110, 510, "  |___|-.", WHITE, 5, 5, BLACK);
            iui_label_shadow(112, 540, "    <|", WHITE, 5, 5, BLACK);
            iui_label_shadow(108, 570, "    _|", WHITE, 5, 5, BLACK);
            iui_setFontSize(20);

            DrawSpriteAtlas(atlas, getSprite("thief"), _facePos.x, _facePos.y, fever?20*sinf(globalTimer*4*DEG2RAD):0, WHITE);
            DrawSpriteAtlas(atlas, getSprite("eye"), _eye1Pos.x, _eye1Pos.y, fever?globalTimer*2:0, WHITE);
            DrawSpriteAtlas(atlas, getSprite("eye"), _eye2Pos.x, _eye2Pos.y, fever?globalTimer*2:0, WHITE);
            DrawSpriteAtlas(atlas, getSprite("mouth"), _mouthPos.x, _mouthPos.y, fever?globalTimer*2:0, WHITE);

            iui_setFontSize(30);
            draw_textbubble_bottom(_bubblePos.x, _bubblePos.y, 360, 80, TextFormat("Give me %d flamingos!", intValue), iuDark2, iuCream, 25, 20);
            iui_setFontSize(20);
            break;
        }
        case 1: {
            iui_setFontSize(25);
            iui_label_shadow(104, 120, "Enter any float!", RAYWHITE, 5, 5, BLACK);
            iui_setFontSize(20);
            iui_floatbox(104, 170, 300, 50, floatboxText, floatValue, "FLOATBOX");
            iui_slider_h(104, 280, floatValue, 300, -100.0f, 100.0f, "FLOATSLIDER_H");
            iui_slider_v(464, 100, floatValue, 200, -100.0f, 100.0f, "FLOATSLIDER_V");
            iui_setFontSize(25);
            iui_label_shadow(104, 450, "(o) (o) (o)", WHITE, 5, 5, BLACK);
            iui_label_shadow(102, 480, " (o) (o) (o)   <- Bunch of donuts", WHITE, 5, 5, BLACK);
            iui_label_shadow(110, 510, "  (o) (o) (o)", WHITE, 5, 5, BLACK);
            iui_label_shadow(112, 540, "(o) (o) (o)", WHITE, 5, 5, BLACK);
            iui_label_shadow(108, 570, " (o) (o) (o)", WHITE, 5, 5, BLACK);
            iui_setFontSize(20);

            DrawSpriteAtlas(atlas, getSprite("police"), _facePos.x, _facePos.y, fever?20*sinf(globalTimer*4*DEG2RAD):0, WHITE);
            DrawSpriteAtlas(atlas, getSprite("eye"), _eye1Pos.x, _eye1Pos.y, fever?globalTimer*2:0, WHITE);
            DrawSpriteAtlas(atlas, getSprite("eye"), _eye2Pos.x, _eye2Pos.y, fever?globalTimer*2:0, WHITE);
            DrawSpriteAtlas(atlas, getSprite("mouth"), _mouthPos.x, _mouthPos.y, fever?globalTimer*2:0, WHITE);
            DrawSpriteAtlas(atlas, getSprite("cap"), _capPos.x, _capPos.y, fever?20*sinf(globalTimer*4*DEG2RAD):0, WHITE);

            iui_setFontSize(30);
            draw_textbubble_bottom(_bubblePos.x, _bubblePos.y, 360, 80, TextFormat("Give me %.2f donuts!", floatValue), iuDark2, iuCream, 25, 20);
            iui_setFontSize(20);
            break;
        }
        }

        int SCREEN_CENTER_X = GetScreenWidth()/2;
        int SCREEN_CENTER_Y = GetScreenHeight()/2;

        imnotgui::iui_end();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}