#include <string>
#include <cmath>

#include "imnotgui.hpp"

namespace imnotgui {

namespace element {
bool iui_button(int x, int y, int w, int h, std::string text) {
    IuiStyle &style = iuiGlobalStyle;

    /// Get label and ID.
    int ID;
    std::string LABEL;
    iui_get_all(text, ID, LABEL);

    /// Button logic
    bool isClicky = false;

    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)w, (float)h})) {
        iui_hotItem = ID;
        if(iui_activeItem == -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = ID;
        }
    }
    if(iui_hotItem == ID && iui_activeItem == ID && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        isClicky = true;
    
    /// Button Draw
    bool isHot = iui_hotItem == ID, isActive = iui_activeItem == ID;

    if(style.isButtonShadowEnabled) {
        draw::iui_rect(x + 8, y + 8, w, h, style.colButtonShadow);
    }

    // Hovering
    if(isHot) {
        if(isActive) {
            draw::iui_rect(x, y, w, h, style.colButtonActiveBackdrop);  // backdrop
            draw::iui_rect(x, y, w, 9, style.colButtonActiveBackdropTop); // top line / box for style?
        } else {
            draw::iui_rect(x, y, w, h, style.colButtonHotBackdrop);
            draw::iui_rect(x, y, w, 9, style.colButtonHotBackdropTop);
        }
    } else {  // nope
        if(isActive) {
            draw::iui_rect(x, y, w, h, style.colButtonActiveBackdrop);
            draw::iui_rect(x, y, w, 9, style.colButtonActiveBackdropTop2);
        } else {
            draw::iui_rect(x, y, w, h, style.colButtonBackdrop);
            draw::iui_rect(x, y, w, 5, style.colButtonBackdropTop);
        }
    }

    // label
    int hprev, yprev;
    iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
    draw::iui_label(x+ w/2, y + h/2, LABEL, style.colButtonLabel);
    iui_setAlignment(hprev, yprev);
    
    return isClicky;
}
bool iui_button_nodraw(int x, int y, int w, int h, std::string text) {
    /// Get label and ID.
    int ID;
    std::string LABEL;
    iui_get_all(text, ID, LABEL);

    /// Button logic
    bool isClicky = false;

    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)w, (float)h})) {
        iui_hotItem = ID;
        if(iui_activeItem == -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = ID;
        }
    }
    if(iui_hotItem == ID && iui_activeItem == ID && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        isClicky = true;
    
    return isClicky;
}
} // namespace element
} // namespace imnotgui