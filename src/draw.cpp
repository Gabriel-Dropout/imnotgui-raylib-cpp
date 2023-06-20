#include <string>
#include <cmath>
#include <algorithm>

#include "raylib/raylib-cpp.hpp"
#include "imnotgui.hpp"
#include "imnotgui_extra.hpp"

namespace imnotgui {
// If you wanna use different backends, you can change this namespace fileds.
namespace draw {
void iui_rect(int x, int y, int w, int h, raylib::Color color) {
    DrawRectangle(x, y, w, h, color);
}
void iui_rect_pos(int x1, int y1, int x2, int y2, raylib::Color color) {
    DrawRectangle(x1, y1, x2 - x1, y2 - y1, color);
}
void iui_rect_rot(int x, int y, int w, int h, raylib::Color color, float angle) {
    DrawRectanglePro(Rectangle{(float)x + w/2.0f, (float)y + h/2.0f, (float)w, (float)h}, Vector2{w / 2.0f, h / 2.0f}, angle, color);
}
void iui_rect_rot_center(int x, int y, int w, int h, raylib::Color color, float angle) {
    DrawRectanglePro(Rectangle{(float)x, (float)y, (float)w, (float)h}, Vector2{w / 2.0f, h / 2.0f}, angle, color);
}
void iui_rect_rot_origin(int x, int y, int w, int h, raylib::Color color, float angle, int ox, int oy) {
    DrawRectanglePro(Rectangle{(float)(x - ox), (float)(y - oy), (float)w, (float)h}, Vector2{(float)ox, (float)oy}, angle, color);
}
void iui_line(int x, int y, int length, float angle, float thick, raylib::Color color) {
    DrawLineEx(Vector2{(float)x, (float)y}, Vector2{x + length * std::cos(angle*DEG2RAD), y + length * std::sin(angle*DEG2RAD)}, thick, color);
}
void iui_label(int x, int y, std::string text, raylib::Color color) {
    raylib::Vector2 tSize = iui_measureTextEx(text);
    int offX = ((int)tSize.x * iuiLabelHalign)/2;
    int offY = ((int)tSize.y * iuiLabelValign)/2;

    if(iuiFont != nullptr)
        raylib::DrawTextEx(*iuiFont, text, Vector2{(float)x - offX, (float)y - offY}, iuiLabelFontsize, std::max(iuiLabelFontsize/(*iuiFont).baseSize, 1), color);
    else
        raylib::DrawText(text, x - offX, y - offY, iuiLabelFontsize, color);
}
void iui_label_transform(int x, int y, std::string text, int fontsize, float angle, raylib::Color color) {
    raylib::Vector2 tSize = iui_measureTextEx(text);
    int offX = ((int)tSize.x << iuiLabelHalign)/2;
    int offY = ((int)tSize.y << iuiLabelValign)/2;
    if(iuiFont != nullptr)
        raylib::DrawTextEx(*iuiFont, text, Vector2{(float)x - offX, (float)y - offY}, iuiLabelFontsize, std::max(iuiLabelFontsize/(*iuiFont).baseSize, 1), color);
    else
        raylib::DrawText(text, x - offX, y - offY, iuiLabelFontsize, color);
}
void iui_label_shadow(int x, int y, std::string text, raylib::Color color, int sx, int sy, raylib::Color scolor) {
    raylib::Vector2 tSize = iui_measureTextEx(text);
    int offX = ((int)tSize.x * iuiLabelHalign)/2;
    int offY = ((int)tSize.y * iuiLabelValign)/2;
    
    if(iuiFont != nullptr) {
        raylib::DrawTextEx(*iuiFont, text, Vector2{(float)x - offX + sx, (float)y - offY + sy}, iuiLabelFontsize, std::max(iuiLabelFontsize/(*iuiFont).baseSize, 1), color);
        raylib::DrawTextEx(*iuiFont, text, Vector2{(float)x - offX, (float)y - offY}, iuiLabelFontsize, std::max(iuiLabelFontsize/(*iuiFont).baseSize, 1), color);
    } else {
        raylib::DrawText(text, x - offX + sx, y - offY + sy, iuiLabelFontsize, scolor);
        raylib::DrawText(text, x - offX, y - offY, iuiLabelFontsize, color);
    }
}
void iui_label_underline_expensive(int x, int y, std::string text, raylib::Color color, float thick, int offsetY, raylib::Color bgColor);
void iui_label_underline(int x, int y, std::string text, raylib::Color color, float thick, int offsetY) {
    raylib::Vector2 tSize = iui_measureTextEx(text);
    int offX = ((int)tSize.x * iuiLabelHalign)/2;
    int offY = ((int)tSize.y * iuiLabelValign)/2;
    DrawRectangle(x - offX, y - offY + tSize.y + offsetY, tSize.x, thick, iui_colLighter_adv(color, -42, 1.15, 1.3, 1.05));
    raylib::DrawText(text, x - offX, y - offY, iuiLabelFontsize, color);
    
    if(iuiFont != nullptr) {
        raylib::DrawTextEx(*iuiFont, text, Vector2{(float)x - offX, (float)y - offY}, iuiLabelFontsize, std::max(iuiLabelFontsize/(*iuiFont).baseSize, 1), color);
    } else {
        raylib::DrawText(text, x - offX, y - offY, iuiLabelFontsize, color);
    }
}
void iui_label_ext(int x, int y, std::string text, raylib::Color color, int sep, int width);

void iui_sprite(raylib::Texture2D &texture, int x, int y, raylib::Color color) {
    DrawTexture(texture, x, y, color);
}
void iui_sprite_from_atlas(raylib::Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, raylib::Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Vector2{(float)sprite.originX, (float)sprite.originY},
        0.f,
        color);
}
void iui_sprite_from_atlas(raylib::Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, float angle, raylib::Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Vector2{(float)sprite.originX, (float)sprite.originY},
        angle,
        color);
}
void iui_sprite_from_atlas(raylib::Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, float xscale, float yscale, float angle, raylib::Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth*xscale, (float)sprite.sourceHeight*yscale},
        Vector2{(float)sprite.originX*xscale, (float)sprite.originY*yscale},
        angle,
        color);
}

void draw_textbubble_top(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, w - arrowHypo);

    // raylib::Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x + arrowPos, y, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}
void draw_textbubble_bottom(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, w - arrowHypo);

    // raylib::Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x + arrowPos, y + h, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}
void draw_textbubble_topbottom(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, w - arrowHypo);

    // raylib::Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x + arrowPos, y, arrowSize, arrowSize, color, 45);
    iui_rect_rot_center(x + arrowPos, y + h, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}
void draw_textbubblel_left(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, h - arrowHypo);

    // raylib::Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x, y + arrowPos, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}
void draw_textbubble_right(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize) {
    int arrowHypo = 0.70710678118 * arrowSize;
    arrowPos = std::clamp(arrowPos, arrowHypo, h - arrowHypo);

    // raylib::Vector2 labelSize = iui_measureTextEx(text);
    int hprev, vprev;
    iui_setAlignment(IUI_LABEL_ALIGN_CENTER, IUI_LABEL_ALIGN_CENTER, hprev, vprev);
    iui_rect_rot_center(x + w, y + arrowPos, arrowSize, arrowSize, color, 45);
    iui_rect(x, y, w, h, color);
    iui_label(x + w/2, y + h/2, text, textColor);
    iui_setAlignment(hprev, vprev);
}

} // namespace draw
} // namespace imnotgui