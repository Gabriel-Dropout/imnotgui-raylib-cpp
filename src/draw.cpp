#include <string>
#include <cmath>

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
    raylib::Vector2 tSize = MeasureTextEx(GetFontDefault(), text.c_str(), iuiLabelFontsize, std::max(iuiLabelFontsize/10, 1));
    int offX = ((int)tSize.x * iuiLabelHalign)/2;
    int offY = ((int)tSize.y * iuiLabelValign)/2;
    raylib::DrawText(text, x - offX, y - offY, iuiLabelFontsize, color);
}
void iui_label_transform(int x, int y, std::string text, int fontsize, float angle, raylib::Color color) {
    raylib::Vector2 tSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontsize, std::max(iuiLabelFontsize/10, 1));
    int offX = ((int)tSize.x << iuiLabelHalign)/2;
    int offY = ((int)tSize.y << iuiLabelValign)/2;
    raylib::DrawText(text, x - offX, y - offY, fontsize, color);
}
void iui_label_shadow(int x, int y, std::string text, raylib::Color color, int sx, int sy, raylib::Color scolor);
void iui_label_underline_expensive(int x, int y, std::string text, raylib::Color color, float thick, int offsetY, raylib::Color bgColor);
void iui_lavel_underline(int x, int y, std::string text, raylib::Color color, float thick, int offsetY);
void iui_label_ext(int x, int y, std::string text, raylib::Color color, int sep, int width);

void iui_sprite(raylib::Texture2D texture, int x, int y, raylib::Color color) {
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
    // sprite.positionX, sprite.positionY, sprite.sourceWidth, sprite.sourceHeight, sprite.originX, sprite.originY
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Vector2{(float)sprite.originX, (float)sprite.originY},
        angle,
        color);
}

} // namespace draw
} // namespace imnotgui