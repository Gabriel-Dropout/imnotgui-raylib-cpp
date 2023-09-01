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

    /// make button hoverable, activable, and clickable
    bool isHot = makeHoverable(ID, x, y, w, h);
    bool isActive = makeActivable(ID);
    isClicky = makeClickable(ID);
    
    /// Button Draw
    if(style.isButtonShadowEnabled) {
        draw::iui_rect(x + style.buttonShadowOffsetX, y + style.buttonShadowOffsetY, w, h, style.colButtonShadow);
    }

    // Hovering
    if(isHot) {
        if(isActive) {
            draw::iui_rect(x, y, w, h, style.colButtonActiveBackdrop);  // backdrop
            draw::iui_rect(x, y, w, style.buttonActiveAccentHei, style.colButtonActiveBackdropTop); // top line / box for style?
        } else {
            draw::iui_rect(x, y, w, h, style.colButtonHotBackdrop);
            draw::iui_rect(x, y, w, style.buttonActiveAccentHei, style.colButtonHotBackdropTop);
        }
    } else {  // nope
        if(isActive) {
            draw::iui_rect(x, y, w, h, style.colButtonActiveBackdrop);
            draw::iui_rect(x, y, w, style.buttonActiveAccentHei, style.colButtonActiveBackdropTop2);
        } else {
            draw::iui_rect(x, y, w, h, style.colButtonBackdrop);
            draw::iui_rect(x, y, w, style.buttonAccentHei, style.colButtonBackdropTop);
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

    /// make button hoverable, activable, and clickable
    makeHoverable(ID, x, y, w, h);
    makeActivable(ID);
    isClicky = makeClickable(ID);
    return isClicky;
}
} // namespace element
} // namespace imnotgui