#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "raylib-cpp.hpp"
#include "imnotgui.hpp"

namespace iui = imnotgui;
namespace iui_draw = imnotgui::draw;


int main() {
    raylib::Window window;
    window.SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    window.Init(1280, 720, "ImNotGUI Layout");
    window.SetMinSize(640, 480);
    window.SetTargetFPS(60);

    std::vector<std::string> tabText = {"imnotgui.hpp", "imnotgui_extra.hpp", "draw.cpp", "iui_multiline_textbox.cpp"};
    std::vector<std::string> fileString(4);
    std::string tb2String = "Hello, World!";
    int tabIdx = 0;

    while(!window.ShouldClose()) {
        BeginDrawing();
        ClearBackground(iui::iuDark2);
        iui::iui_begin();

        raylib::Rectangle lScreen(0, 0, window.GetWidth(), window.GetHeight());
        auto [lTop, lTerm] = iui::layout::split_v(lScreen, 0, lScreen.height - 180);
        auto [lMenu, lMain] = iui::layout::split_h(lTop, 0, 200);
        auto [lBar, lTextbox] = iui::layout::split_v(lMain, 0, 50);
        auto [lTb1, lTb2] = iui::layout::split_h(lTextbox, 0.5f, 0);
        lTb1 = iui::layout::pad(lTb1, 0, 8, 0, 16);
        lTb2 = iui::layout::pad(lTb2, 0, 16, 0, 8);

        iui_draw::iui_rect(lScreen, iui::iuDark);
        iui_draw::iui_rect(lTerm, iui::iuDark2);
        iui_draw::iui_rect(lMenu, iui::iuHellaDark);
        iui_draw::iui_rect(lBar, iui::iuDark2);

        iui_draw::iui_rect(lTb1, iui::iuNormal);
        iui_draw::iui_rect(lTb2, iui::iuNormal);

        iui::element::iui_tab_h(iui::layout::pad(lBar, 0, 0, 16, 10), 50, 9999, tabText, tabIdx);
        iui::element::iui_tab_v(iui::layout::pad(lMenu, 0, 0, 0, 8), 40, tabText, tabIdx);
        iui::element::iui_multi_textbox(lTb1, fileString[tabIdx], "tb1");
        iui::element::iui_multi_textbox(lTb2, tb2String, "tb2");

        iui::iui_end();
        EndDrawing();
    }

    window.Close();
    return 0;
}