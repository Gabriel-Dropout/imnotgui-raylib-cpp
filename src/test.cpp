#include <iostream>
#include <string>
#include <raylib/raylib-cpp.hpp>
#include "imnotgui.hpp"

int main() {
    std::vector<std::string> textVec = {"Tab 1", "Tab 2", "Tab 3", "Tab 4", "Tab 5", "Tab 6", "Tab 7"};
    int tabIdx = 0;
    std::string textbox = "Hello World!";
    std::string intbox;
    int intboxVal = 0;
    std::string floatbox;
    float floatboxVal = 0.0f;
    int sliderVal = 0;
    bool checkboxVal = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "NagaGUI Test");
    SetTargetFPS(60);

    imnotgui::iuiButtonShadow = true;
    while(!WindowShouldClose()) {
        BeginDrawing();
        imnotgui::iui_begin();
        ClearBackground(imnotgui::iuDark2);

        imnotgui::draw::iui_rect(42, 0, 1196, 85, imnotgui::iuHellaDark);
        imnotgui::draw::iui_rect(42, 85, 1196, 600, imnotgui::iuDark);
        imnotgui::draw::iui_rect(42, 680, 1196, 40, imnotgui::iuHellaDark);

        imnotgui::element::iui_tab(42, 30, 100, 55, textVec, tabIdx, 1);
        
        int tmpSize = imnotgui::iui_setFontSize(28);
        if(imnotgui::element::iui_button(84, 170, 200, 80, "Hello World!")) {
            std::cout << "Hello World!" << std::endl;
        }
        imnotgui::iui_setFontSize(tmpSize);

        tmpSize = imnotgui::iui_setFontSize(32);
        imnotgui::element::iui_textbox(368, 170, 400, 80, textbox, "TB1");
        imnotgui::element::iui_intbox(368, 170 + 142, 400, 80, intbox, intboxVal, "IB1");
        //imnotgui::element::iui_floatbox(368, 170 + 142*2, 400, 80, floatbox, floatboxVal, "FB1");
        imnotgui::iui_setAlignment(imnotgui::IUI_LABEL_ALIGN_LEFT, imnotgui::IUI_LABEL_ALIGN_MIDDLE);
        imnotgui::draw::iui_label(368+400+21, 170 + 142 + 40, std::to_string(intboxVal), imnotgui::iuiColButtonLabel);
        imnotgui::draw::iui_label(368+400+21, 170 + 284 + 40, std::to_string(floatboxVal), imnotgui::iuiColButtonLabel);
        imnotgui::iui_setAlignment(imnotgui::IUI_LABEL_ALIGN_LEFT, imnotgui::IUI_LABEL_ALIGN_TOP);
        imnotgui::iui_setFontSize(tmpSize);

        imnotgui::element::iui_slider_h(768+21+42, 170, sliderVal, 400, 0, 20, "SL1");
        imnotgui::element::iui_slider_v(768+21+42+200, 212, sliderVal, 400, 0, 20, "SL2");

        checkboxVal = imnotgui::element::iui_checkbox(184 - 21 - 42, 250+42, 42, 42, checkboxVal, "CB1");
        imnotgui::element::iui_checkbox(184 + 21, 250+42, 42, 42, false, "CB2");
        imnotgui::iui_end();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}