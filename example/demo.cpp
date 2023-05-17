#include <iostream>
#include <string>
#include <raylib/raylib-cpp.hpp>
#include "imnotgui.hpp"
#include "resources/atlas.h"

using namespace imnotgui;
using namespace imnotgui::draw;
using namespace imnotgui::element;

// Get one sprite index position from the array by nameId
static int GetSpriteIndex(rtpAtlasSprite *spriteDesc, int spriteCount, const char *nameId);

int main() {
    std::vector<std::string> mainTabVec = {"IMnotGUI", "elements", "ID and LABEL system", "how?", "contact", "MISC.", "asdf"};
    int mainTabIdx = 0;
    std::vector<std::string> demoTabVec = {"Button", "Textbox", "Slider", "Tab", "Misc."};
    int demoTabIdx = 0;

    bool ui_check_test = false;
    bool ui_check_test2 = false;
    

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "NagaGUI Test");
    SetTargetFPS(60);

    raylib::Texture2D atlas("resources/atlas.png");
    
    // Get sprite from the array given a specific index
    
    rtpAtlasSprite spr_uimouse_0 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_uimouse_0")];
    rtpAtlasSprite spr_uimouse_1 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_uimouse_1")];
    rtpAtlasSprite spr_uimouse_2 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_uimouse_2")];
    rtpAtlasSprite spr_loss_0 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_loss_0")];
    rtpAtlasSprite spr_loss_1 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_loss_1")];
    rtpAtlasSprite spr_loss_2 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_loss_2")];
    rtpAtlasSprite spr_loss_3 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_loss_3")];
    rtpAtlasSprite spr_logo_0 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_logo_0")];
    rtpAtlasSprite spr_logo_1 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_logo_1")];
    rtpAtlasSprite spr_biglogo = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_biglogo")];
    rtpAtlasSprite spr_here = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_here")];
    rtpAtlasSprite spr_diamond = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_diamond")];
    rtpAtlasSprite spr_icon42_0 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_icon42_0")];
    rtpAtlasSprite spr_icon42_1 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_icon42_1")];
    rtpAtlasSprite spr_icon42_2 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_icon42_2")];
    rtpAtlasSprite spr_icon42_3 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_icon42_3")];
    rtpAtlasSprite spr_chip64 = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_chip64")];
    rtpAtlasSprite spr_pfp = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_pfp")];
    rtpAtlasSprite spr_uicheck = rtpDescAtlas[GetSpriteIndex(rtpDescAtlas, ATLAS_ATLAS_SPRITE_COUNT, "spr_uicheck")];

    iuiButtonShadow = true;

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(imnotgui::iuDark2);
        iui_begin();

        int SCREEN_CENTER_X = GetScreenWidth()/2;
        int SCREEN_CENTER_Y = GetScreenHeight()/2;

        // backdrop
        iui_rect(42, 0, 1196, 85, imnotgui::iuHellaDark);
        iui_rect(42, 85, 1196, 600, imnotgui::iuDark);
        iui_rect(42, 680, 1196, 40, imnotgui::iuHellaDark);

        float sine01_ = sin(imnotgui::iui_animTimer * 0.1f);

        int TAB_X = 42;

        switch(imnotgui::element::iui_tab(TAB_X, 30, 140, 50, mainTabVec, mainTabIdx, 2)) {
            case 0:{
                /// Intro
                iui_sprite_from_atlas(atlas, spr_biglogo, SCREEN_CENTER_X, SCREEN_CENTER_Y/2, WHITE);
                iui_label(SCREEN_CENTER_X - 257, SCREEN_CENTER_Y/4 + 12, "Sincerely,", iuCream);
                int hprev, vprev;
                iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                iui_label(SCREEN_CENTER_X, SCREEN_CENTER_Y/1.4, "-=[Dear ImGui bootleg(?) implementation in Raylib]=-", iuCream);
                iui_setAlignment(hprev, vprev);

                // sprite
                float sinned = sine01_;
                float cosBoi = cos(iui_animTimer * 0.1) * 12;
                float loopTime = 1320 - ((iui_animTimer*2)%1680);
                iui_sprite_from_atlas(atlas, spr_chip64, loopTime, (SCREEN_CENTER_Y*2) - 70 - abs(cosBoi), sinned * 12, WHITE);
                break;
            }
            case 1:
                /// Elements
                int tabW = (int)(1196/5);
                iui_rect(42, 85, 1196, 60, iuHellaDark);
                switch(iui_tab(42, 90, tabW, 50, demoTabVec, demoTabIdx, 1)) {
                    case 0:{
                        /// Button
                        int hprev, vprev, sprev;
                        iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                        sprev = iui_setFontSize(42);
                        iui_label(SCREEN_CENTER_X, 180, "=[ Button ]=", iuCream);
                        iui_setFontSize(sprev);

                        int strWid = MeasureText("is_clicked = iui_button(x, y, width, height, ID and label);", iui_getFontSize()) + 10;

                        iui_rect(SCREEN_CENTER_X - strWid/2 - 2, 228, strWid+4, 54, iuMint);
                        iui_rect(SCREEN_CENTER_X - strWid/2, 230, strWid, 50, iuHellaDark);
                        iui_label(SCREEN_CENTER_X, 255, "is_clicked = iui_button(x, y, width, height, ID and label);", iuCream);
                        iui_setAlignment(hprev, vprev);

                        // normal buttons
                        iuiButtonShadow = true;
                        iui_button(SCREEN_CENTER_X - 500, 300, 420, 84, "A Button.");
                        iui_button(SCREEN_CENTER_X - 500, 400, 200, 84, "Yet\nanother button###ANALS");
                        iui_button(SCREEN_CENTER_X - 280, 400, 200, 84, "And button\nwith same ID###ANALS");

                        // checker
                        iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                        iui_label(SCREEN_CENTER_X - 400, 580, "I love dogs", iuCream);
                        iui_label(SCREEN_CENTER_X - 180, 580, "I love cats", iuCream);      
                        iui_label(SCREEN_CENTER_X - 290, 620, "THANOS CAR", iuCream);
                        ui_check_test = iui_checkbox(SCREEN_CENTER_X - 400, 600, 20, 20, ui_check_test, "check_test");
                        ui_check_test2 = iui_checkbox(SCREEN_CENTER_X - 180, 600, 20, 20, ui_check_test2, "check_test_2");
                        iui_checkbox(SCREEN_CENTER_X - 290, 640, 40, 40, true, "check_wew");
                        iui_setAlignment(hprev, vprev);
                        
                        // button with different ID than string
                        iui_button(SCREEN_CENTER_X + 40, 300, 200, 84, "Button with\nDifferent ID###SPECIAL_ID"); // ID Key = Hash of "SPECIAL_ID"
                        
                        // Draw white speech bubble
                        std::string demoStr    = "string = Button with#Different ID###SPECIAL_ID";
                        raylib::Vector2 demoStrSize = MeasureTextEx(GetFontDefault(), demoStr.c_str(), iui_getFontSize(), std::max(iui_getFontSize()/10, 1));
                        int underlineWid = MeasureText("###SPECIAL_ID", iui_getFontSize());
                        
                        iui_rect_rot_center(SCREEN_CENTER_X + 60, 405, 20, 20, iuCream, 45);
                        iui_rect(SCREEN_CENTER_X + 20, 405, demoStrSize.x + 20, demoStrSize.y + 20, iuCream);
                        iui_rect(SCREEN_CENTER_X + 30 + (demoStrSize.x - underlineWid), 415 + demoStrSize.y, underlineWid, 4, iuRed); // underline
                        iui_label(SCREEN_CENTER_X + 30, 415, demoStr, iuHellaDark);
                        
                        // button with hidden ID part
                        iui_button(SCREEN_CENTER_X + 40, 470, 200, 84, "Button with\nHidden ID##SECRET_ID"); // ID Key = Hash of "Button with#Hidden ID##SECRET_ID"
                        
                        demoStr    = "string = Button with#Hidden ID###SECRET_ID";
                        demoStrSize = MeasureTextEx(GetFontDefault(), demoStr.c_str(), iui_getFontSize(), std::max(iui_getFontSize()/10, 1));
                        underlineWid = MeasureText("###SECRET_ID", iui_getFontSize());

                        iui_rect_rot_center(SCREEN_CENTER_X + 60, 575, 20, 20, iuCream, 45);
                        iui_rect(SCREEN_CENTER_X + 20, 575, demoStrSize.x + 20, demoStrSize.y + 20, iuCream); 
                        iui_rect(SCREEN_CENTER_X + 30 + (demoStrSize.x - underlineWid), 585 + demoStrSize.y, underlineWid, 4, iuRed); // underline
                        iui_label(SCREEN_CENTER_X + 30, 585, demoStr, iuHellaDark);
                        
                        
                        iuiButtonShadow = false;
                        break;
                    }
                    case 1:{
                        /// Textbox
                        int hprev, vprev, sprev;
                        iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                        sprev = iui_setFontSize(42);
                        iui_label(SCREEN_CENTER_X, 180, "=[ Textbox ]=", iuCream);
                        iui_setFontSize(sprev);
                        // codebox
                        int strWid = MeasureText("text_var = iui_textbox_numberonly(x, y, width, height, text_var, ID);", iui_getFontSize()) + 10;
                        iui_rect(SCREEN_CENTER_X - (strWid / 2) - 2, 228, (strWid + 4), 104, iuMint);
                        iui_rect(SCREEN_CENTER_X - (strWid / 2), 230, strWid, 100, iuHellaDark);
                        iui_label(SCREEN_CENTER_X, 260, "text_var = iui_textbox(x, y, width, height, text_var, ID);", iuCream);
                        iui_label(SCREEN_CENTER_X, 300, "text_var = iui_textbox_numberonly(x, y, width, height, text_var, ID);", iuCream);
                
                        iui_setAlignment(hprev, vprev);
                        break;
                    }
                }
                break;
        }
        
        imnotgui::iui_end();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Get one sprite index position from the array by nameId
static int GetSpriteIndex(rtpAtlasSprite *spriteDesc, int spriteCount, const char *nameId)
{
    int index = 0;
    
    for (int i = 0; i < spriteCount; i++)
    {
        if (TextIsEqual(nameId, spriteDesc[i].nameId))
        {
            index = i;
            break;
        }
    }
    
    return index;
}