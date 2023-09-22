#include <string>
#include <cmath>
#include <algorithm>

#include "imnotgui.hpp"
#include "imnotgui_extra.hpp"

namespace imnotgui {
namespace draw {
void iui_rect(Rectangle rect, Color color) {
    DrawRectangleRec(rect, color);
}
void iui_rect(int x, int y, int w, int h, Color color) {
    DrawRectangle(x, y, w, h, color);
}
void iui_rect_pos(int x1, int y1, int x2, int y2, Color color) {
    DrawRectangle(x1, y1, x2 - x1, y2 - y1, color);
}
void iui_rect_rot(int x, int y, int w, int h, Color color, float angle) {
    DrawRectanglePro(Rectangle{(float)x + w/2.0f, (float)y + h/2.0f, (float)w, (float)h}, Vector2{w / 2.0f, h / 2.0f}, angle, color);
}
void iui_rect_rot_center(int x, int y, int w, int h, Color color, float angle) {
    DrawRectanglePro(Rectangle{(float)x, (float)y, (float)w, (float)h}, Vector2{w / 2.0f, h / 2.0f}, angle, color);
}
void iui_rect_rot_origin(int x, int y, int w, int h, Color color, float angle, int ox, int oy) {
    DrawRectanglePro(Rectangle{(float)(x - ox), (float)(y - oy), (float)w, (float)h}, Vector2{(float)ox, (float)oy}, angle, color);
}
void iui_line(int x, int y, int length, float angle, float thick, Color color) {
    DrawLineEx(Vector2{(float)x, (float)y}, Vector2{x + length * std::cos(angle*DEG2RAD), y + length * std::sin(angle*DEG2RAD)}, thick, color);
}
void iui_label(int x, int y, const std::string &text, Color color) {
    IuiStyle &style = iuiGlobalStyle;

    Vector2 tSize = iui_measureTextEx(text);
    int offX = ((int)tSize.x * style.labelHalign)/2;
    int offY = ((int)tSize.y * style.labelValign)/2;
    
    Font curFont = style.font == nullptr ? GetFontDefault() : *style.font;
    DrawTextEx(curFont, text.c_str(), Vector2{(float)x - offX, (float)y - offY}, style.labelFontsize, std::max(style.labelFontsize/curFont.baseSize, 1), color);
}
void iui_label_transform(int x, int y, const std::string &text, int fontsize, float angle, Color color) {
    // TODO: make angle to work
    {   ScopedFontSizeSetter _(fontsize);
        iui_label(x, y, text, color);
    }
}
void iui_label_shadow(int x, int y, const std::string &text, Color color, int sx, int sy, Color scolor) {
    iui_label(x + sx, y + sy, text, scolor);
    iui_label(x, y, text, color);
}
void iui_label_underline(int x, int y, const std::string &text, Color color, float thick, int offsetY) {
    IuiStyle &style = iuiGlobalStyle;

    Vector2 tSize = iui_measureTextEx(text);
    int offX = ((int)tSize.x * style.labelHalign)/2;
    int offY = ((int)tSize.y * style.labelValign)/2;
    DrawRectangle(x - offX, y - offY + tSize.y + offsetY, tSize.x, thick, iui_colLighter_adv(color, -42, 1.15, 1.3, 1.05));
    
    Font curFont = style.font == nullptr ? GetFontDefault() : *style.font;
    DrawTextEx(curFont, text.c_str(), Vector2{(float)x - offX, (float)y - offY}, style.labelFontsize, std::max(style.labelFontsize/curFont.baseSize, 1), color);
}
void iui_label_box(Rectangle rect, const std::string &text, Color color) {
    IuiStyle &style = iuiGlobalStyle;
    int x = rect.x + ((int)rect.width * style.labelHalign)/2;
    int y = rect.y + ((int)rect.height * style.labelValign)/2;
    iui_label(x, y, text, color);
}
void iui_label_box_shadow(Rectangle rect, const std::string &text, Color color, int sx, int sy, Color scolor) {
    IuiStyle &style = iuiGlobalStyle;
    int x = rect.x + ((int)rect.width * style.labelHalign)/2;
    int y = rect.y + ((int)rect.height * style.labelValign)/2;
    iui_label_shadow(x, y, text, color, sx, sy, scolor);
}
void iui_label_box_underline(Rectangle rect, const std::string &text, Color color, float thick, int offsetY) {
    IuiStyle &style = iuiGlobalStyle;
    int x = rect.x + ((int)rect.width * style.labelHalign)/2;
    int y = rect.y + ((int)rect.height * style.labelValign)/2;
    iui_label_underline(x, y, text, color, thick, offsetY);
}

void draw_textbubble_top(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, w - arrowHypo);

    // Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x + arrowPos, y, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}
void draw_textbubble_bottom(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, w - arrowHypo);

    // Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x + arrowPos, y + h, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}
void draw_textbubble_topbottom(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, w - arrowHypo);

    // Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x + arrowPos, y, arrowSize, arrowSize, color, 45);
    iui_rect_rot_center(x + arrowPos, y + h, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}
void draw_textbubblel_left(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, h - arrowHypo);

    // Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x, y + arrowPos, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}
void draw_textbubble_right(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, h - arrowHypo);

    // Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x + w, y + arrowPos, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}

void iui_groupbox(int x, int y, int w, int h, std::string text) {
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_CENTER, hprev, vprev);

    iui_rect(x, y, 2, h, iuMint);
    iui_rect(x + w - 2, y, 2, h, iuMint);
    iui_rect(x, y + h - 2, w, 2, iuMint);

    int textWidth = iui_measureText(text);
    iui_rect(x, y, 12 - 4, 2, iuMint);
    iui_label(x + 12, y, text, iuMint);
    iui_rect(x + 12 + textWidth + 4, y, w - 12 - textWidth - 4, 2, iuMint);
    
    iui_setAlignment(hprev, vprev);
}

} // namespace draw
} // namespace imnotgui