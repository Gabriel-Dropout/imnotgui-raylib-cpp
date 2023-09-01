#include <string>
#include <iostream>

#include "imnotgui.hpp"

namespace imnotgui {
namespace element {
bool iui_checkbox(int x, int y, int w, int h, bool checked, const std::string ID) {
    IuiStyle &style = iuiGlobalStyle;

    int _ID;
    std::string LABEL;
    iui_get_all(ID, _ID, LABEL);

    // is hover
    makeHoverable(_ID, x, y, w, h);
    makeActivable(_ID);
    if(makeClickable(_ID)) {
        checked = !checked;
    }

    /// draw
    int thickness = style.checkboxBorderThickness;
    draw::iui_rect(x-thickness, y-thickness, w+thickness, h+thickness, style.colCheckboxBorder);
    draw::iui_rect(x, y, w, h, style.colCheckboxBG);

    if(checked) {
        int hprev, yprev, sprev;
        sprev = iui_setFontSize((int)(w*0.8));
        iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
        draw::iui_label(x + w/2, y + h/2, "V", style.colCheckboxFG);
        iui_setAlignment(hprev, yprev);
        iui_setFontSize(sprev);
    }
    return checked;
}

} // namespace element
} // namespace imnotgui