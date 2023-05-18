#include <string>

#include "raylib/raylib-cpp.hpp"
#include "imnotgui.hpp"

namespace imnotgui {
namespace element {
bool iui_checkbox(int x, int y, int w, int h, bool checked, const std::string ID) {

    int _ID;
    std::string LABEL;
    iui_get_all(ID, _ID, LABEL);

    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)w, (float)h})) {
        iui_hotItem = _ID;

        // ... and is clicked
        if(iui_activeItem == -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = _ID;
        }
    }

    bool isHot = iui_hotItem == _ID, isActive = iui_activeItem == _ID;

    // is 'Pressed' (AKA The user pressed and released the button)
    if(isHot && isActive && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        checked = !checked;
    }

    /// draw

    draw::iui_rect(x-2, y-2, w+4, h+4, iuiColCheckboxBorder);
    draw::iui_rect(x, y, w, h, iuiColCheckboxBG);

    if(checked) {
        int hprev, yprev, sprev;
        sprev = iui_setFontSize((int)(w*0.8));
        iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
        draw::iui_label(x + w/2, y + h/2, "V", iuiColCheckboxFG);
        iui_setAlignment(hprev, yprev);
        iui_setFontSize(sprev);
    }
    return checked;
}

} // namespace element
} // namespace imnotgui