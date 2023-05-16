#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

#include <raylib/raylib-cpp.hpp>
#include "imnotgui.hpp"

namespace imnotgui {
namespace element {

} // namespace element


std::string iui_trim_label(std::string text) {
    int CUTPOS = text.find_first_of("##");
    if(CUTPOS == std::string::npos) return text;
    return text.substr(0, CUTPOS);
}
std::string iui_trim_id(std::string text) {
    int CUTPOS = text.find_first_of("##");
    if(CUTPOS == std::string::npos) return text;
    return text.substr(CUTPOS + 2, text.size() - CUTPOS - 2);
}

void iui_get_all(std::string text, int& ID, std::string& label) {
    std::string _TMPID = iui_trim_id(text);
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
    std::string _TMPID = iui_trim_id(text);
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
    iui_hotItem = -1;
    iui_animTimer++;
}
// End IMNOTGUI
void iui_end() {
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if(iui_activeItem == -1) {
            iui_activeItem = -0xDEADBEEF;
        }
    } else {
        iui_activeItem = -1;
    }
}

// Label State
void iui_setAlignment(int halign, int valign) {
    iuiLabelHalign = halign;
    iuiLabelValign = valign;
}
void iui_setAlignment(int halign, int valign, int &hprev, int &yprev) {
    hprev = iuiLabelHalign;
    yprev = iuiLabelValign;
    iuiLabelHalign = halign;
    iuiLabelValign = valign;
}
void iui_getAlignment(int &hprev, int &yprev) {
    hprev = iuiLabelHalign;
    yprev = iuiLabelValign;
}
int iui_setFontSize(int size) {
    int prev = iuiLabelFontsize;
    iuiLabelFontsize = size;
    return prev;
}
int iui_getFontSize() {
    return iuiLabelFontsize;
}

std::string iui_strTrim(std::string text, int width) {
    int em_ = MeasureText("M", 20);
    int strLen = text.size();
    
    if(strLen * em_ > width) {
        text = text.substr(0, std::max(width/em_ - 3, 0)) + "...";
    }
    return text;
}
std::string iui_strTrimNodots(std::string text, int width) {
    int em_ = MeasureText("M", 20);
    int strLen = text.size();
    
    if(strLen * em_ > width) {
        text = text.substr(0, std::max(width/em_ - 1, 0));  // different from original code
    }
    return text;
}

raylib::Color iui_colLighter(raylib::Color color, int amount) {
    return raylib::Color(std::clamp(color.r + (int)amount, 0, 255), std::clamp(color.g + (int)(amount*1.3), 0, 255), std::clamp(color.b + (int)(amount*1.5), 0, 255), 255);
}
} // namespace imnotgui