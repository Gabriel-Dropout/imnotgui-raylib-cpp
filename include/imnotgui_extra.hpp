// This header is highly dependent on raylib.

#ifndef IMNOTGUI_EXTRA_HPP
#define IMNOTGUI_EXTRA_HPP

#include "raylib.h"

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
    void iui_sprite(Texture2D &texture, int x, int y, Color color);
    void iui_sprite_from_atlas(Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, Color color);
    void iui_sprite_from_atlas(Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, float angle, Color color);
    void iui_sprite_from_atlas(Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, float xscale, float yscale, float angle, Color color);

    void draw_textbubble_top(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);
    void draw_textbubble_bottom(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);
    void draw_textbubble_topbottom(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);
    void draw_textbubblel_left(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);
    void draw_textbubble_right(int x, int y, int w, int h, std::string text, Color color, Color textColor, int arrowPos, int arrowSize);

    void iui_groupbox(int x, int y, int w, int h, std::string text);
}  // namespace draw
}  // namespace imnotgui

#endif // IMNOTGUI_EXTRA_HPP