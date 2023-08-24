#ifndef IMNOTGUI_EXTRA_HPP
#define IMNOTGUI_EXTRA_HPP

#include <string>

#include "raylib.h"

namespace imnotgui {
namespace draw {
    void draw_textbubble_top(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);
    void draw_textbubble_bottom(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);
    void draw_textbubble_topbottom(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);
    void draw_textbubblel_left(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);
    void draw_textbubble_right(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);

    void iui_groupbox(int x, int y, int w, int h, std::string text);
}  // namespace draw
}  // namespace imnotgui

#endif // IMNOTGUI_EXTRA_HPP