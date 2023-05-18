// This header is highly dependent on raylib.

#ifndef IMNOTGUI_EXTRA_HPP
#define IMNOTGUI_EXTRA_HPP

#include "raylib/raylib-cpp.hpp"

// Atlas sprite properties
typedef struct rtpAtlasSprite {
    const char *nameId;
    int originX, originY;
    int positionX, positionY;
    int sourceWidth, sourceHeight;
    int padding;
    bool trimmed;
    int trimRecX, trimRecY, trimRecWidth, trimRecHeight;
} rtpAtlasSprite;

namespace imnotgui {
namespace draw {
    void iui_sprite(raylib::Texture2D texture, int x, int y, raylib::Color color);
    void iui_sprite_from_atlas(raylib::Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, raylib::Color color);
    void iui_sprite_from_atlas(raylib::Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, float angle, raylib::Color color);

    void draw_textbubble_top(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize);
    void draw_textbubble_bottom(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize);
    void draw_textbubblel_left(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize);
    void draw_textbubble_right(int x, int y, int w, int h, std::string text, raylib::Color color, raylib::Color textColor, int arrowPos, int arrowSize);
}  // namespace draw
}  // namespace imnotgui

#endif // IMNOTGUI_EXTRA_HPP