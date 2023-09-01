#include <iostream>
#include <string>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "imnotgui.hpp"
#include "imnotgui_extra.hpp"

#define RTEXLOADER_IMPLEMENTATION
#include "include/rtexloader.hpp"

using namespace imnotgui;
using namespace imnotgui::draw;
using namespace imnotgui::element;

// Draw a sprite from atlas
void DrawSprite(std::string sprite, int x, int y, Color color) {
    DrawSpriteAtlas(atlas, getSprite(sprite), x, y, 0, color);
}
void DrawSprite(std::string sprite, int x, int y, float angle, Color color) {
    DrawSpriteAtlas(atlas, getSprite(sprite), x, y, angle, color);
}
void DrawSprite(std::string sprite, int x, int y, float xscale, float yscale, float angle, Color color) {
    DrawSpriteAtlas(atlas, getSprite(sprite), x, y, xscale, yscale, angle, color);
}
void DrawSprite(std::string sprite, Rectangle slice, int x, int y, float xscale, float yscale, float angle, Color color) {
    DrawSpriteAtlas(atlas, getSprite(sprite), slice, x, y, xscale, yscale, angle, color);
}

float colBrightness(raylib::Color color) {
    return (float)sqrt(color.r*color.r*0.241 + color.g*color.g*0.691 + color.b*color.b*0.068);
}
void draw_colorblock(raylib::Color color, std::string text1, std::string text2, int x, int y, int w, int h) {
    raylib::Color stringColor = iuCream;
    if(colBrightness(color) > 130) {
        stringColor = iuHellaDark;
    }
    {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE);
        iui_rect(x, y, w, h - 40, color);
        iui_rect(x, y + h - 40, w, 40, iui_colLighter(color, -20));
        iui_label(x + w/2, y + 24, text1, stringColor);
        iui_label(x + w/2, y + h - 20, text2, stringColor);
    }
}

int main() {
    std::vector<std::string> mainTabVec = {"IMnotGUI", "elements", "ID and LABEL system", "how?", "contact", "MISC.", "Language Hacker"};
    int mainTabIdx = 0;
    std::vector<std::string> demoTabVec = {"Button", "Textbox", "Slider", "Tab", "Misc.", "Extra"};
    int demoTabIdx = 0;

    bool ui_check_test = false;
    bool ui_check_test2 = false;

    std::string ui_textbox_testarea_str = "";
    std::string ui_textbox_number_str = "";
    int ui_textbox_number = 0;
    std::vector<std::string> ui_textbox_str_vec = {""};

    int ui_slider_test_h = 0;
    int ui_slider_test_v = 0;
    int ui_slider_test_minmax = 0;

    std::vector<std::string> lossTabVec = {"ATATTTATATATATA", "OMAEWA MOU", "SHINDEIRU", "NANI"};
    int lossTabIdx = 0;
    

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "ImNotGUI Demo");
    SetTargetFPS(60);

    initAtlas("resources/atlas.png");    

    iuiGlobalStyle.isButtonShadowEnabled = true;

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

        switch(imnotgui::element::iui_tab(TAB_X, 30, 140, 50, mainTabVec, mainTabIdx, IUI_TAB_FLEX)) {
            case 0: {
                /// Intro
                DrawSprite("spr_biglogo", SCREEN_CENTER_X, SCREEN_CENTER_Y/2, WHITE);
                iui_label(SCREEN_CENTER_X - 257, SCREEN_CENTER_Y/4 + 12, "Sincerely,", iuCream);

                {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE);
                    iui_label(SCREEN_CENTER_X, SCREEN_CENTER_Y/1.4, "-=[Dear ImGui bootleg(?) implementation in Raylib]=-", iuCream);
                }

                // sprite
                float sinned = sine01_;
                float cosBoi = cos(iui_animTimer * 0.1) * 12;
                float loopTime = 1320 - ((iui_animTimer*2)%1680);
                DrawSprite("spr_chip64", loopTime, (SCREEN_CENTER_Y*2) - 70 - abs(cosBoi), sinned * 12, WHITE);
                iui_label_shadow(loopTime + 40, (SCREEN_CENTER_Y * 2) - 90 - abs(cosBoi), "<- A random bloke,\n   plz ignore.", iuPiss, 2, 2, iuHellaDark);

                iui_rect(SCREEN_CENTER_X - 300, SCREEN_CENTER_Y / 1.2, 600, 300, iuHellaDark);
                iui_rect(SCREEN_CENTER_X - 300 + 4, SCREEN_CENTER_Y / 1.2 + 4, 600 - 8, 300 - 8, iui_colLighter_adv(iuDark2, -30, 1, 1.32, 1.45));
                iui_rect(SCREEN_CENTER_X - 300 + 4, SCREEN_CENTER_Y / 1.2 + 42, 600 - 8, 42, iuHellaDark);
                iui_rect(SCREEN_CENTER_X - 300 + 4, SCREEN_CENTER_Y / 1.2 +126, 600 - 8, 42, iuHellaDark);
                iui_rect(SCREEN_CENTER_X - 300 + 4, SCREEN_CENTER_Y / 1.2 +210, 600 - 8, 42, iuHellaDark);
                break;
            }
            case 1:{
                /// Elements
                int tabW = (int)(1196/6);
                iui_rect(42, 85, 1196, 60, iuHellaDark);
                switch(iui_tab(42, 90, tabW, 50, demoTabVec, demoTabIdx, IUI_TAB_TRIM)) {
                    case 0:{
                        /// Button
                        {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE);
                            {   ScopedFontSizeSetter _(42);
                                iui_label_shadow(SCREEN_CENTER_X, 180, "=[ Button ]=", iuCream, 5, 5, iuHellaDark);
                            }

                            int strWid = MeasureText("is_clicked = iui_button(x, y, width, height, ID and label);", iui_getFontSize()) + 10;

                            iui_rect(SCREEN_CENTER_X - strWid/2 - 2, 228, strWid+4, 54, iuMint);
                            iui_rect(SCREEN_CENTER_X - strWid/2, 230, strWid, 50, iuHellaDark);
                            iui_label(SCREEN_CENTER_X, 255, "is_clicked = iui_button(x, y, width, height, ID and label);", iuCream);
                        }

                        // normal buttons
                        iuiGlobalStyle.isButtonShadowEnabled = true;
                        iui_button(SCREEN_CENTER_X - 500, 300, 420, 84, "A Button.");
                        iui_button(SCREEN_CENTER_X - 500, 400, 200, 84, "Yet\nanother button###ANALS");
                        iui_button(SCREEN_CENTER_X - 280, 400, 200, 84, "And button\nwith same ID###ANALS");

                        // checker
                        {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE);
                            iui_label(SCREEN_CENTER_X - 400, 580, "I love dogs", iuCream);
                            iui_label(SCREEN_CENTER_X - 180, 580, "I love cats", iuCream);      
                            iui_label(SCREEN_CENTER_X - 290, 620, "THANOS CAR", iuCream);
                            ui_check_test = iui_checkbox(SCREEN_CENTER_X - 400, 600, 20, 20, ui_check_test, "check_test");
                            ui_check_test2 = iui_checkbox(SCREEN_CENTER_X - 180, 600, 20, 20, ui_check_test2, "check_test_2");
                            iui_checkbox(SCREEN_CENTER_X - 290, 640, 40, 40, true, "check_wew");
                        }
                        
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
                        
                        iuiGlobalStyle.isButtonShadowEnabled = false;
                        break;
                    }
                    case 1:{
                        /// Textbox
                        int hprev, vprev;
                        {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE);
                            //title
                            {   ScopedFontSizeSetter _(42);
                                iui_label_shadow(SCREEN_CENTER_X, 180, "=[ Textbox ]=", iuCream, 5, 5, iuHellaDark);
                            }

                            // codebox
                            int strWid = MeasureText("text_var = iui_textbox_numberonly(x, y, width, height, text_var, ID);", iui_getFontSize()) + 10;
                            iui_rect(SCREEN_CENTER_X - (strWid / 2) - 2, 228, (strWid + 4), 104, iuMint);
                            iui_rect(SCREEN_CENTER_X - (strWid / 2), 230, strWid, 100, iuHellaDark);
                            iui_label(SCREEN_CENTER_X, 260, "text_var = iui_textbox(x, y, width, height, text_var, ID);", iuCream);
                            iui_label(SCREEN_CENTER_X, 300, "text_var = iui_textbox_numberonly(x, y, width, height, text_var, ID);", iuCream);
                        }

                        // textboxes
                        int textboxY = 370;
                        iui_textbox(SCREEN_CENTER_X - 420, textboxY, 200, 50, ui_textbox_testarea_str, "7-grand-dad");
                        iui_intbox(SCREEN_CENTER_X - 420, textboxY + 160, 200, 50, ui_textbox_number_str, ui_textbox_number, "number-plz");
                        iui_multi_textbox(SCREEN_CENTER_X + 20, textboxY, 400, 210, ui_textbox_str_vec, "extra-thicc");

                        iui_setAlignment(IUI_LABEL_ALIGN_RIGHT, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                        iui_label(SCREEN_CENTER_X - 430, textboxY + 25, "STRING", iuCream);
                        iui_label(SCREEN_CENTER_X - 430, textboxY + 185, "NUMBER ONLY", iuCream);
                        iui_setAlignment(hprev, vprev);
                        
                        // value
                        std::string bubbleLabel = iui_strTrimDots(ui_textbox_testarea_str, 400);
                        int bubbleWid = iui_measureText(bubbleLabel) + 20;
                        draw_textbubble_top(SCREEN_CENTER_X - 420, (textboxY + 70) + abs(sine01_ * 5), std::max(200, bubbleWid), 50, bubbleLabel, iuCream, iuHellaDark, 32, 20);
                        
                        bubbleLabel = "Speed : " + ui_textbox_number_str;
                        bubbleWid = iui_measureText(bubbleLabel) + 20;
                        draw_textbubble_top(SCREEN_CENTER_X - 200, (textboxY + 230) + abs(sine01_ * 5), std::max(150, bubbleWid), 50, bubbleLabel, iuCream, iuHellaDark, 32, 20);
                        
                        // value test
                        int chipY = abs(std::sin(iui_animTimer * 0.05 * ui_textbox_number)) * 21;
                        DrawSprite("spr_chip64", SCREEN_CENTER_X - 136, (textboxY + 185) - chipY, ui_textbox_number * iui_animTimer, WHITE);
                        break;
                    }
                    case 2:{
                        /// Sliders
                        int hprev, vprev, sprev;
                        iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                        
                        //title
                        {   ScopedFontSizeSetter _(42);
                            iui_label_shadow(SCREEN_CENTER_X, 180, "=[ Slider ]=", iuCream, 5, 5, iuHellaDark);
                        }

                        // codebox
                        int strWid = iui_measureText("value_var = iui_slider_v(x, y, value_var, height, minval, maxval, ID);") + 10;
                        iui_rect(SCREEN_CENTER_X - (strWid / 2) - 2, 228, (strWid + 4), 104, iuMint);
                        iui_rect(SCREEN_CENTER_X - (strWid / 2), 230, strWid, 100, iuHellaDark);
                        iui_label(SCREEN_CENTER_X, 260, "value_var = iui_slider_h(x, y, value_var, width, minval, maxval, ID);", iuCream);
                        iui_label(SCREEN_CENTER_X, 300, "value_var = iui_slider_v(x, y, value_var, height, minval, maxval, ID);", iuCream);
                        iui_setAlignment(hprev, vprev);
                        
                        // sliders
                        iui_slider_h(SCREEN_CENTER_X - 110, 400, ui_slider_test_h, 420, 0, 100, "slider-test-h");
                        iui_slider_v(SCREEN_CENTER_X - 200, 400 - (iuiGlobalStyle.sliderHHei / 2), ui_slider_test_v, 160 + iuiGlobalStyle.sliderHHei, 0, 100, "slider-test-v");
                        iui_slider_h(SCREEN_CENTER_X - 110, 560, ui_slider_test_minmax, 420, 20, 42, "slider-test-minmax");
                        
                        draw_textbubble_right(SCREEN_CENTER_X - 460, 442, 256, 60, "Vertical#(iui_slider_v)", iuCream, iuHellaDark, 30, 20);
                        draw_textbubble_topbottom(SCREEN_CENTER_X, 450, 256, 60, "Horizontal#(iui_slider_h)", iuCream, iuHellaDark, 100, 20 + (sine01_ * 4));
                        break;
                    }
                    case 3:{
                        /// Tabs
                        int hprev, vprev;
                        iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                        
                        //title
                        {   ScopedFontSizeSetter _(42);
                            iui_label_shadow(SCREEN_CENTER_X, 180, "=[ Tabs ]=", iuCream, 5, 5, iuHellaDark);
                        }
                        // codebox
                        int strWid = iui_measureText("tab_index = iui_tab(x, y, width, height, strings, tab_index, trim mode);") + 10;
                        iui_rect(SCREEN_CENTER_X - (strWid / 2) - 2, 228, (strWid + 4), 54, iuMint);
                        iui_rect(SCREEN_CENTER_X - (strWid / 2), 230, strWid, 50, iuHellaDark);
                        iui_label(SCREEN_CENTER_X, 255, "tab_index = iui_tab(x, y, width, height, strings, tab_index, trim mode);", iuCream);
                        iui_setAlignment(hprev, vprev);
                        
                        iui_tab(SCREEN_CENTER_X - 500, 300, 250, 64, lossTabVec, lossTabIdx, IUI_TAB_TRIM);
                        iui_tab_v(SCREEN_CENTER_X - 500, 300 + 96, 200, 64, lossTabVec, lossTabIdx, IUI_TAB_TRIM);

                        {   ScopedFontSizeSetter _(42);
                            DrawSprite(TextFormat("spr_loss_%d", lossTabIdx), SCREEN_CENTER_X, 550, 0.5f, 0.5f, 0, WHITE);
                            iui_label(SCREEN_CENTER_X - 180, 510, TextFormat("%d]", lossTabIdx), iuDark2);
                        }
                        break;
                    }
                    case 4:{
                        /// Misc.
                        int hprev, vprev;
                        iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                        
                        //title
                        {   ScopedFontSizeSetter _(42);
                            iui_label_shadow(SCREEN_CENTER_X, 180, "=[ Misc. ]=", iuCream, 5, 5, iuHellaDark);
                        }
                        
                        // rect
                        raylib::Color iuRainbow = raylib::Color::FromHSV((int)(iui_animTimer*0.1f)%360, 1.0f, 1.0f);
                        iui_rect(SCREEN_CENTER_X - 560, 250, 200, 80, iuRainbow);
                        draw_textbubble_top(SCREEN_CENTER_X - 560, 350, 480, 60, "iui_rect(x, y, width, height, colour)", iuCream, iuHellaDark, 42, 20 + (sine01_ * 4));
                        
                        iui_rect_rot(SCREEN_CENTER_X + 32, 290, 80, 80, iuRainbow, iui_animTimer);
                        draw_textbubble_top(SCREEN_CENTER_X - 10, 350, 600, 60, "iui_rect_rot(x, y, width, height, colour, angle)", iuCream, iuHellaDark, 42, 20 + (sine01_ * 4));
                        
                        iui_rect_rot_center(SCREEN_CENTER_X - 520, 500, 80, 80, iuRainbow, iui_animTimer);
                        draw_textbubble_top(SCREEN_CENTER_X - 560, 600, 520, 60, "iui_rect_rot_center([same as iui_rect_rot]", iuCream, iuHellaDark, 42, 20 + (sine01_ * 4));
                        
                        iui_line(SCREEN_CENTER_X + 80, 500, 120, iui_animTimer, 8, iuRainbow);
                        draw_textbubble_top(SCREEN_CENTER_X - 10, 600, 600, 60, "iui_line(x, y, length, angle, thickness, colour)", iuCream, iuHellaDark, 42, 20 + (sine01_ * 4)); 
                        //iui_rect(SCREEN_CENTER_X - 420, 230, strWid, 100, iuRainbow);
                        
                        iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE);
                        iui_label(SCREEN_CENTER_X - 240, 500 + sine01_ * 10, "Oh, Also this text thing uses#iui_label(x, y, string, colour)...", iuPiss);

                        iui_setAlignment(hprev, vprev);
                        break;
                    }
                    case 5:{
                        /// Extra.
                        int hprev, vprev;
                        iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                        
                        //title
                        {   ScopedFontSizeSetter _(42);
                            iui_label_shadow(SCREEN_CENTER_X, 180, "=[ Extra ]=", iuCream, 5, 5, iuHellaDark);
                        }
                        
                        // groupbox
                        iui_groupbox(SCREEN_CENTER_X - 100, 300, 200, 200, "Groupbox");

                        iui_setAlignment(hprev, vprev);
                        break;
                    }
                }
                break;
            }
            case 2:{
                /// ID demo
                
                // text
                int hprev, vprev;
                iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                {   ScopedFontSizeSetter _(42);
                    iui_label_shadow(SCREEN_CENTER_X, SCREEN_CENTER_Y / 3, "=[ The ID and LABEL system ]=", iuCream, 0, 10, iuHellaDark);
                }

                
                raylib::Color AMERIKACOLOR = iuPiss;
                if ((iui_animTimer >> 5) % 1)
                    AMERIKACOLOR = iuSky;
                    
                iui_label_shadow(SCREEN_CENTER_X, (SCREEN_CENTER_Y / 3) + 50, "(LABEL : The one that's displayed)", AMERIKACOLOR, 0, 5, iuHellaDark);
                
                int labelSpacing = 390;
                int labelYMargin = 80;
                int infoWid = 320;
                int infoHei = 100;
                int infoX = SCREEN_CENTER_X - labelSpacing - (infoWid >> 1);
                int infoY = SCREEN_CENTER_Y - labelYMargin;
                
                // demo 1... Normal ID
                std::string idStr = "HAHA YES";
                
                iui_rect(infoX, infoY - infoHei/2, infoWid, infoHei, iuHellaDark);
                iui_label(SCREEN_CENTER_X - labelSpacing, infoY, "\" " + idStr + " \"", iuCream);
                
                iui_label(infoX + infoWid/2, infoY + infoHei + infoHei, "LABEL : " + iui_trim_label(idStr), iuRed);
                draw_textbubble_top(infoX, infoY + infoHei/2, infoWid, infoHei, "ID : " + iui_trim_idstr(idStr), iuCream, iuHellaDark, infoWid/2, 30);
                iui_rect(infoX, infoY + infoHei + infoHei/2, infoWid, infoHei, iuDark2);
                iui_label(infoX + infoWid/2, infoY + infoHei + infoHei, "LABEL : " + iui_trim_label(idStr), iuCream);
                
                // demo 2... ###
                infoX = SCREEN_CENTER_X - infoWid/2;
                idStr = "ASDF###SECRET";
                
                iui_rect(infoX, infoY - infoHei/2, infoWid, infoHei, iuHellaDark);
                iui_label(SCREEN_CENTER_X, infoY, "\" " + idStr + " \"", iuCream);
                
                draw_textbubble_top(infoX, infoY + infoHei/2, infoWid, infoHei, "ID : " + iui_trim_idstr(idStr), iuCream, iuHellaDark, infoWid/2, 30);
                iui_rect(infoX, infoY + infoHei + infoHei/2, infoWid, infoHei, iuBrown);
                iui_label(infoX + infoWid/2, infoY + infoHei + infoHei, "LABEL : " + iui_trim_label(idStr), iuCream);
                
                // demo 3... ##
                infoX = SCREEN_CENTER_X + labelSpacing - infoWid/2;
                idStr = "NOTHING##HIDDEN";
                
                iui_rect(infoX, infoY - infoHei/2, infoWid, infoHei, iuHellaDark);
                iui_label(SCREEN_CENTER_X + labelSpacing, infoY, "\" " + idStr + " \"", iuCream);
                
                draw_textbubble_top(infoX, infoY + infoHei/2, infoWid, infoHei, "ID : " + iui_trim_idstr(idStr), iuCream, iuHellaDark, infoWid/2, 30);
                iui_rect(infoX, infoY + infoHei + infoHei/2, infoWid, infoHei, iuNormal);
                iui_label(infoX + infoWid/2, infoY + infoHei + infoHei, "LABEL : " + iui_trim_label(idStr), iuCream);
                
                iui_setAlignment(hprev, vprev);
                break;
            }
            case 3:{
                /// KAK????
                
                // How
                
                int hprev, vprev;
                {   ScopedFontSizeSetter _(42);
                    iui_label(120, (SCREEN_CENTER_Y / 3) + 10, "How?", iuHellaDark);
                    iui_label(120, SCREEN_CENTER_Y / 3, "How?", iuCream);
                    iui_label(120, (SCREEN_CENTER_Y + 10), "WHY NOT VERTEX BUFFER?", iuHellaDark);
                    iui_label(120, SCREEN_CENTER_Y, "WHY NOT VERTEX BUFFER?", iuCream);

                    
                    // list o' stuff
                    int listX = 130;
                    int listY = (SCREEN_CENTER_Y / 3) + 100;
                    
                    iui_setFontSize(24);
                    
                    iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                    
                    DrawSprite("spr_diamond", listX, listY + 5, iuHellaDark);
                    DrawSprite("spr_diamond", listX, listY, iuMint);
                    iui_label_shadow(listX + 30, listY, "ABUSES GAMEMAKER'S SPRITE-BATCH SYSTEM", iuCream, 0, 5, iuHellaDark);
                    
                    listY += 35;
                    DrawSprite("spr_diamond", listX, listY + 5, iuHellaDark);
                    DrawSprite("spr_diamond", listX, listY, iuMint);
                    iui_label_shadow(listX + 30, listY, "...WHICH MAKES THE MOST OUT OF SINGLE DRAW CALL. (= REALLY  FAST!)", iuCream, 0, 5, iuHellaDark);
                    
                    listY += 35;
                    DrawSprite("spr_diamond", listX, listY + 5, iuHellaDark);
                    DrawSprite("spr_diamond", listX, listY, iuMint);
                    iui_label_shadow(listX + 30, listY, "HEAVILY INSPIRED FROM DEAR IMGUI'S VERTEX BUFFER SYSTEM", iuCream, 0, 5, iuHellaDark);
                    
                    listY = SCREEN_CENTER_Y + 110;
                    DrawSprite("spr_diamond", listX, listY + 5, iuHellaDark);
                    DrawSprite("spr_diamond", listX, listY, iuMint);
                    iui_label_shadow(listX + 30, listY, "VERTEX BUFFER IS NOT GOOD FOR MOVING THINGS LIKE GUI D: (NOT FUNNY)", iuCream, 0, 5, iuHellaDark);
                    
                    listY += 35;
                    DrawSprite("spr_diamond", listX, listY + 5, iuHellaDark);
                    DrawSprite("spr_diamond", listX, listY, iuMint);
                    iui_label_shadow(listX + 30, listY + 18, "SHOVING VERTICES INTO VERTEX BUFFER IS\nREALLY EXPENSIVE FOR CPU (NOT FUNNY [2])", iuCream, 0, 5, iuHellaDark);
                    
                    iui_setAlignment(hprev, vprev);
                }
                break;
            }
            case 4:{
                int hprev, vprev;
                {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE);
                    /// DEV INFO
                    int pfpX = SCREEN_CENTER_X / 2.5;
                    int pfpY = SCREEN_CENTER_Y / 1.5;
                    int marginWid = (pfpX - 42) * 2;
                    int marginX   = 42 + marginWid;
                    
                    // backdrop
                    iui_rect(42, 85, marginWid, 595, iuHellaDark);
                    iui_rect(42, 85, marginWid, 300, iuMint);
                    iui_rect(marginX, 85, 10, 595, iuCream);
                    
                    // iui_rect(marginX + 10, 85, 1186 - marginWid, 595, iuDark2);
                    
                    // pfp
                    DrawSprite("spr_pfp_0", pfpX, pfpY, WHITE);
                    
                    // namecard
                    int nameX = pfpX;
                    int nameY = pfpY + 110;

                    {   ScopedFontSizeSetter _(42);
                        // Big name
                        int nameW = iui_measureText("MILKFORDUNK") + 16;
                        int nameH = 52 + 12;
                        iui_rect(42, nameY - nameH/2, marginWid, nameH + 20, iuDark);
                        iui_rect(nameX - nameW/2, nameY - nameH/2, nameW, nameH + 20, iuBrown);
                        iui_label_shadow(nameX, nameY, "MILKFORDUNK", iuCream, 0, 9, iuHellaDark);
                    }
                    
                    // AKA
                    iui_label(nameX, nameY + 38, "(AKA ZIKBAKGURI)", iuPiss);     
                    
                    {   ScopedFontSizeSetter _(24);
                        // Smol desc.
                        nameY += 100;
                        iui_label_shadow(nameX, nameY, "- IS 18 Y/O GAME DEV BOYO", iuCream, 0, 4, iuHellaDark);
                        iui_label_shadow(nameX, nameY + 50, "- IS TOTAL DINK", iuCream, 0, 4, iuHellaDark);
                        iui_label_shadow(nameX, nameY + 100, "(IS ALSO ARTIST)", iuCream, 0, 4, iuHellaDark);
                    }
                    
                    // click-able smol link
                    int linkX    = marginX + 60;
                    int linkY    = pfpY - 79;
                    int iconSize = 21; // half size of the icon
                    raylib::Color linkColour = iuSky;
                    std::string linkStr    = "@ZIKBAKGURI";
                    raylib::Vector2 linkSize;
                    int linkOffX, linkW, linkH;

                    {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE);
                        /// Twitter
                        // we can't use shadow variant here; We want fully solid shadow, Not tinted shadow.
                        // Thankfully the icon is rectangle... That means we can draw Rect for shadow!
                        iui_rect(linkX - iconSize, linkY - iconSize + 10, 42, 42, iuHellaDark);
                        DrawSprite("spr_icons42_0", linkX, linkY, WHITE);
                        iui_label(linkX + 31, linkY, "Twitter : ", iuCream);
                        
                        // clickable stuff
                        linkSize = iui_measureTextEx(linkStr);
                        linkW = linkSize.x;
                        linkH = linkSize.y;
                        linkOffX = (linkX + 31 + iui_measureText("Twitter : "));
                        
                        // check click
                        if (iui_button_nodraw(linkOffX - 2, linkY - linkH/2 - 2, linkW + 4, linkH + 4, "btn-twitter")){}
                            //url_open("https://twitter.com/ZIKBAKGURI");
                            
                        // check hover
                        if (iui_hotItem == iui_get_id("btn-twitter"))
                        {
                            linkColour = iuRed;
                            iui_rect(linkOffX - 2, linkY - 10, linkW, linkH, iuDark2);
                        }
                        
                        iui_label(linkOffX, linkY, linkStr, linkColour);
                        
                        // prepare for another line
                        linkColour = iuSky;
                        linkY += 62;
                        
                        
                        /// Youtube
                        iui_rect(linkX - iconSize, linkY - iconSize + 10, 42, 42, iuHellaDark);
                        DrawSprite("spr_icons42_1", linkX, linkY, WHITE);
                        iui_label(linkX + 31, linkY, "YT : ", iuCream);
                        
                        linkStr = ":D MilkForDunk";
                        linkSize = iui_measureTextEx(linkStr);
                        linkW = linkSize.x;
                        linkH = linkSize.y;
                        linkOffX = (linkX + 31 + iui_measureText("YT : "));

                        if (iui_button_nodraw(linkOffX - 2, linkY - linkH/2 - 2, linkW + 4, linkH + 4, "btn-yt")){}
                            // url_open("https://www.youtube.com/user/XDJcreepXfail");
                        if (iui_hotItem == iui_get_id("btn-yt"))
                        {
                            linkColour = iuRed;
                            iui_rect(linkOffX - 2, linkY - 10, linkW, linkH, iuDark2);
                        }
                        
                        iui_label_underline(linkOffX, linkY, linkStr, linkColour, 2, -2);
                        
                        linkColour = iuSky;
                        linkY += 62;
                        
                        
                        /// Naver
                        iui_rect(linkX - iconSize, linkY - iconSize + 10, 42, 42, iuHellaDark);
                        DrawSprite("spr_icons42_2", linkX, linkY, WHITE);
                        iui_label(linkX + 31, linkY, "NAVER blog : ", iuCream);
                        
                        linkStr = "rjansrhals";
                        linkSize = iui_measureTextEx(linkStr);
                        linkW = linkSize.x;
                        linkH = linkSize.y;
                        linkOffX = (linkX + 31 + iui_measureText("NAVER blog : "));

                        if (iui_button_nodraw(linkOffX - 2, linkY - linkH/2 - 2, linkW + 4, linkH + 4, "btn-naver")){}
                            // url_open("http://blog.naver.com/rjansrhals");
                        if (iui_hotItem == iui_get_id("btn-naver"))
                        {
                            linkColour = iuRed;
                            iui_rect(linkOffX - 2, linkY - 10, linkW, linkH, iuDark2);
                        }
                        
                        iui_label_underline(linkOffX, linkY, linkStr, linkColour, 2, -2);
                        
                        linkColour = iuSky;
                        linkY += 62;
                        
                        
                        /// GitHub
                        iui_rect(linkX - iconSize, linkY - iconSize + 10, 42, 42, iuHellaDark);
                        DrawSprite("spr_icons42_3", linkX, linkY, WHITE);
                        iui_label(linkX + 31, linkY, "GitHub : ", iuCream);
                        
                        linkStr = "TandyRum1024";
                        linkSize = iui_measureTextEx(linkStr);
                        linkW = linkSize.x;
                        linkH = linkSize.y;
                        linkOffX = (linkX + 31 + iui_measureText("GitHub : "));

                        if (iui_button_nodraw(linkOffX - 2, linkY - linkH/2 - 2, linkW + 4, linkH + 4, "btn-gh")){}
                            // url_open("https://github.com/TandyRum1024");
                        if (iui_hotItem == iui_get_id("btn-gh"))
                        {
                            linkColour = iuRed;
                            iui_rect(linkOffX - 2, linkY - 10, linkW, linkH, iuDark2);
                        }
                        
                        iui_label_underline(linkOffX, linkY, linkStr, linkColour, 2, -2);
                        
                        linkY += 62;
                        
                        iui_label(linkX - 21, linkY, "Contact me via e-mail @ yubin2892@gmail.com !", iuMint);
                    }
                }
                break;
            }
            case 5:{
                /// MISC
                // palette
                int paletteY = 200; 
                int paletteW = 180;
                int paletteH = 128;
                int paletteHalfW = paletteW / 2;
                int paletteOffX = 0;
                
                draw_colorblock(iuHellaDark, "Überdark", "#191817",  SCREEN_CENTER_X - (paletteW * 2), paletteY, paletteW, paletteH);
                draw_colorblock(iuDark,      "Dark A", "#313435",    SCREEN_CENTER_X - paletteW, paletteY, paletteW, paletteH);
                draw_colorblock(iuDark2,     "Dark B", "#3F494F",    SCREEN_CENTER_X, paletteY, paletteW, paletteH);
                draw_colorblock(iuNormal,    "Neutral", "#637674",   SCREEN_CENTER_X + paletteW, paletteY, paletteW, paletteH);
                
                paletteY   += paletteH;
                paletteW    =  (paletteW * 4) / 5;
                paletteOffX = -(paletteW * 5) / 2;
                
                draw_colorblock(iuMint,  "Mint", "#25CDA3",        SCREEN_CENTER_X + paletteOffX, paletteY, paletteW, paletteH);
                paletteOffX += paletteW;
                draw_colorblock(iuSky,   "Bland blue", "#68B9C8",  SCREEN_CENTER_X + paletteOffX, paletteY, paletteW, paletteH);
                paletteOffX += paletteW;
                draw_colorblock(iuRed,   "HyperRed", "#ED3255",    SCREEN_CENTER_X + paletteOffX, paletteY, paletteW, paletteH);
                paletteOffX += paletteW;
                draw_colorblock(iuPiss,  "PISS", "#EABF11",        SCREEN_CENTER_X + paletteOffX, paletteY, paletteW, paletteH);
                paletteOffX += paletteW;
                draw_colorblock(iuBrown, "THICC Brown", "#5A4D48", SCREEN_CENTER_X + paletteOffX, paletteY, paletteW, paletteH);
                
                paletteY += paletteH;
                paletteW  = 360;
                paletteH  = 90;
                
                draw_colorblock(iuCream,     "Cream A", "#EFE8C4", SCREEN_CENTER_X - paletteW, paletteY, paletteW, paletteH);
                draw_colorblock(iuCreamDark, "Cream B", "#E0D3A7", SCREEN_CENTER_X, paletteY, paletteW, paletteH);
                
                // "Palette"
                int hprev, vprev, sprev;
                sprev = iui_setFontSize(42);

                iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                
                iui_label(SCREEN_CENTER_X, SCREEN_CENTER_Y / 2.5 + 10, "-= Palette =-", iuHellaDark);
                iui_label(SCREEN_CENTER_X, SCREEN_CENTER_Y / 2.5     , "-= Palette =-", iuCream);
                
                iui_setAlignment(hprev, vprev);
                iui_setFontSize(sprev);
                break;
            }
            case 6:{
                int hprev, vprev, sprev;

                /// DEV INFO
                int pfpX = SCREEN_CENTER_X / 2.5;
                int pfpY = SCREEN_CENTER_Y / 1.5;
                int marginWid = (pfpX - 42) * 2;
                int marginX   = 42 + marginWid;
                
                // backdrop
                iui_rect(42, 85, marginWid, 595, iuHellaDark);
                iui_rect(42, 85, marginWid, 300, iuMint);
                iui_rect(marginX, 85, 10, 595, iuCream);
                
                // iui_rect(marginX + 10, 85, 1186 - marginWid, 595, iuDark2);
                
                // pfp
                DrawSprite("spr_pfp_1", pfpX, pfpY, WHITE);
                
                // namecard
                sprev = iui_setFontSize(42);
                
                raylib::Vector2 nameSize = iui_measureTextEx("Gabriel-Dropout");
                int nameW = nameSize.x + 16;
                int nameH = 52 + 12;
                int nameX = pfpX;
                int nameY = pfpY + 110;
                
                iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_MIDDLE, hprev, vprev);
                
                // Big name
                iui_rect(42, nameY - nameH/2, marginWid, nameH + 20, iuDark);
                iui_rect(nameX - nameW/2, nameY - nameH/2, nameW, nameH + 20, iuBrown);
                iui_label_shadow(nameX, nameY, "Gabriel-Dropout", iuCream, 0, 9, iuHellaDark);
                
                iui_setFontSize(20);
                
                // AKA
                iui_label(nameX, nameY + 38, "(AKA winterday)", iuPiss);     
                
                iui_setFontSize(24);
                
                // Smol desc.
                nameY += 100;
                iui_label_shadow(nameX, nameY, "- Ported this library to raylib", iuCream, 0, 4, iuHellaDark);
                iui_label_shadow(nameX, nameY + 50, "- Raylib NOOB", iuCream, 0, 4, iuHellaDark);
                iui_label_shadow(nameX, nameY + 100, "- Buried under    \n    a mountain of homework", iuCream, 0, 4, iuHellaDark);
                
                iui_setFontSize(20);
                
                // click-able smol link
                int linkX    = marginX + 60;
                int linkY    = pfpY - 79;
                int iconSize = 21; // half size of the icon
                raylib::Color linkColour = iuSky;
                std::string linkStr    = "Gabriel's Blog";
                raylib::Vector2 linkSize;
                int linkOffX, linkW, linkH;

                iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE);
                
                /// Twitter
                // we can't use shadow variant here; We want fully solid shadow, Not tinted shadow.
                // Thankfully the icon is rectangle... That means we can draw Rect for shadow!
                iui_rect(linkX - iconSize, linkY - iconSize + 10, 42, 42, iuHellaDark);
                DrawSprite("spr_icons42_0", linkX, linkY, WHITE);
                iui_label(linkX + 31, linkY, "Github Blog : ", iuCream);
                
                // clickable stuff
                linkSize = iui_measureTextEx(linkStr);
                linkW = linkSize.x;
                linkH = linkSize.y;
                linkOffX = (linkX + 31 + iui_measureText("Github Blog : "));
                
                // check click
                if (iui_button_nodraw(linkOffX - 2, linkY - linkH/2 - 2, linkW + 4, linkH + 4, "btn-github")){}
                    //url_open("https://gabriel-dropout.github.io/");
                    
                // check hover
                if (iui_hotItem == iui_get_id("btn-github"))
                {
                    linkColour = iuRed;
                    iui_rect(linkOffX - 2, linkY - 10, linkW, linkH, iuDark2);
                }
                
                iui_label(linkOffX, linkY, linkStr, linkColour);
                
                // prepare for another line
                linkColour = iuSky;
                linkY += 62;
                
                
                /// Youtube
                iui_rect(linkX - iconSize, linkY - iconSize + 10, 42, 42, iuHellaDark);
                DrawSprite("spr_icons42_1", linkX, linkY, WHITE);
                iui_label(linkX + 31, linkY, "NAVER blog : ", iuCream);
                
                linkStr = "Trash Can";
                linkSize = iui_measureTextEx(linkStr);
                linkW = linkSize.x;
                linkH = linkSize.y;
                linkOffX = (linkX + 31 + iui_measureText("NAVER blog : "));

                if (iui_button_nodraw(linkOffX - 2, linkY - linkH/2 - 2, linkW + 4, linkH + 4, "btn-naver")){}
                    // url_open("https://blog.naver.com/smalleyemin");
                if (iui_hotItem == iui_get_id("btn-naver"))
                {
                    linkColour = iuRed;
                    iui_rect(linkOffX - 2, linkY - 10, linkW, linkH, iuDark2);
                }
                
                iui_label_underline(linkOffX, linkY, linkStr, linkColour, 2, -2);
                
                linkColour = iuSky;
                linkY += 62;
                
                
                /// Naver
                iui_rect(linkX - iconSize, linkY - iconSize + 10, 42, 42, iuHellaDark);
                DrawSprite("spr_icons42_2", linkX, linkY, WHITE);
                iui_label(linkX + 31, linkY, "NAVER blog2 : ", iuCream);
                
                linkStr = "DEV NOTE";
                linkSize = iui_measureTextEx(linkStr);
                linkW = linkSize.x;
                linkH = linkSize.y;
                linkOffX = (linkX + 31 + iui_measureText("NAVER blog2 : "));

                if (iui_button_nodraw(linkOffX - 2, linkY - linkH/2 - 2, linkW + 4, linkH + 4, "btn-naver2")){}
                    // url_open("https://blog.naver.com/min0210210");
                if (iui_hotItem == iui_get_id("btn-naver2"))
                {
                    linkColour = iuRed;
                    iui_rect(linkOffX - 2, linkY - 10, linkW, linkH, iuDark2);
                }
                
                iui_label_underline(linkOffX, linkY, linkStr, linkColour, 2, -2);
                
                linkColour = iuSky;
                linkY += 62;
                
                
                /// GitHub
                iui_rect(linkX - iconSize, linkY - iconSize + 10, 42, 42, iuHellaDark);
                DrawSprite("spr_icons42_3", linkX, linkY, WHITE);
                iui_label(linkX + 31, linkY, "GitHub : ", iuCream);
                
                linkStr = "TandyRum1024";
                linkSize = iui_measureTextEx(linkStr);
                linkW = linkSize.x;
                linkH = linkSize.y;
                linkOffX = (linkX + 31 + iui_measureText("GitHub : "));

                if (iui_button_nodraw(linkOffX - 2, linkY - linkH/2 - 2, linkW + 4, linkH + 4, "btn-gh")){}
                    // url_open("https://github.com/Gabriel-Dropout");
                if (iui_hotItem == iui_get_id("btn-gh"))
                {
                    linkColour = iuRed;
                    iui_rect(linkOffX - 2, linkY - 10, linkW, linkH, iuDark2);
                }
                
                iui_label_underline(linkOffX, linkY, linkStr, linkColour, 2, -2);
                
                iui_setAlignment(hprev, vprev);
            }
        }
        
        imnotgui::iui_end();
        EndDrawing();
    }

    unloadAtlas();
    CloseWindow();
    return 0;
}