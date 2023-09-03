#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

#include "imnotgui.hpp"

namespace imnotgui {

std::string iui_trim_label(std::string text) {
    int CUTPOS = text.find_first_of("##");
    if(CUTPOS == std::string::npos) return text;
    return text.substr(0, CUTPOS);
}
std::string iui_trim_idstr(std::string text) {
    int CUTPOS = text.find_first_of("##");
    if(CUTPOS == std::string::npos) return text;
    return text.substr(CUTPOS + 2, text.size() - CUTPOS - 2);
}

void iui_get_all(std::string text, int& ID, std::string& label) {
    std::string _TMPID = iui_trim_idstr(text);
    int _MAPID = iui_idMap[_TMPID];

    if(_MAPID == 0) {
        iui_idMap[_TMPID] = iui_idx;
        _MAPID = iui_idx;
        iui_idx++;
    }

    ID = _MAPID;
    label = iui_trim_label(text);
    return;
}
int iui_get_id(std::string text) {
    std::string _TMPID = iui_trim_idstr(text);
    int _MAPID = iui_idMap[_TMPID];

    if(_MAPID == 0) {
        iui_idMap[_TMPID] = iui_idx;
        _MAPID = iui_idx;
        iui_idx++;
    }

    return _MAPID;
}

// Begin IMNOTGUI
void iui_begin() {
    iui_hotItemBack = -1;
    iui_animTimer++;
    iui_cursorTimer++;
}
// End IMNOTGUI
void iui_end() {
    // swap hotItem buffers
    iui_hotItem = iui_hotItemBack;

    // reset activeItem
    // WHY not in iui_begin? to check item click, we need to maintain iui_activeItem for 1 frame after mouse release.
    if(!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        iui_activeItem = -1;
    }
}

// Label State
void iui_setAlignment(int halign, int valign) {
    IuiStyle &style = iuiGlobalStyle;
    style.labelHalign = halign;
    style.labelValign = valign;
}
void iui_setAlignment(int halign, int valign, int &hprev, int &yprev) {
    IuiStyle &style = iuiGlobalStyle;
    hprev = style.labelHalign;
    yprev = style.labelValign;
    style.labelHalign = halign;
    style.labelValign = valign;
}
void iui_getAlignment(int &hprev, int &yprev) {
    IuiStyle &style = iuiGlobalStyle;
    hprev = style.labelHalign;
    yprev = style.labelValign;
}
int iui_setFontSize(int size) {
    IuiStyle &style = iuiGlobalStyle;
    int prev = style.labelFontsize;
    style.labelFontsize = size;
    return prev;
}
int iui_getFontSize() {
    IuiStyle &style = iuiGlobalStyle;
    return style.labelFontsize;
}
Font* iui_setFont(Font* font) {
    IuiStyle &style = iuiGlobalStyle;
    Font* prev = style.font;
    style.font = font;
    return prev;
}
Font* iui_getFont() {
    IuiStyle &style = iuiGlobalStyle;
    return style.font;
}

std::string iui_strTrimDots(std::string text, int width) {
    if(iui_measureText(text) <= width) return text;

    std::string curText = text + "...";
    while(iui_measureText(curText) > width && curText.size() >= 4) {
        curText.erase(curText.size() - 4, 1);
    }
    return curText;
}
std::string iui_strTrim(std::string text, int width) {
    if(iui_measureText(text) <= width) return text;

    std::string curText = text;
    while(iui_measureText(curText) > width && curText.size() >= 1) {
        curText.erase(curText.size() - 1, 1);
    }
    return curText;
}

Color iui_colLighter(Color color, int amount) {
    return Color{
        (unsigned char) std::clamp(color.r + (int)amount, 0, 255),
        (unsigned char) std::clamp(color.g + (int)(amount*1.3), 0, 255),
        (unsigned char) std::clamp(color.b + (int)(amount*1.5), 0, 255),
        255
    };
}
Color iui_colLighter_adv(Color color, int amount, float rmod, float gmod, float bmod) {
    return Color{
        (unsigned char) std::clamp(color.r + (int)(amount*rmod), 0, 255),
        (unsigned char) std::clamp(color.g + (int)(amount*gmod), 0, 255),
        (unsigned char) std::clamp(color.b + (int)(amount*bmod), 0, 255),
        255
    };
}

int iui_measureText(std::string text) {
    IuiStyle &style = iuiGlobalStyle;
    Font curFont = style.font == nullptr ? GetFontDefault() : *style.font;
    return MeasureTextEx(curFont, text.c_str(), style.labelFontsize, std::max(style.labelFontsize/curFont.baseSize, 1)).x;
}
Vector2 iui_measureTextEx(std::string text) {
    IuiStyle &style = iuiGlobalStyle;
    Font curFont = style.font == nullptr ? GetFontDefault() : *style.font;
    return MeasureTextEx(curFont, text.c_str(), style.labelFontsize, std::max(style.labelFontsize/curFont.baseSize, 1));
}

} // namespace imnotgui