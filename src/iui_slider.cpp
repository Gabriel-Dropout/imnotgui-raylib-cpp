#include <string>
#include<algorithm>

#include "imnotgui.hpp"

#include <iostream>

namespace imnotgui {
namespace element {
template <typename T>
void iui_slider_h_base(int x, int y, T &value, int width, T min, T max, const std::string ID) {
    IuiStyle &style = iuiGlobalStyle;

    int sliderX = x + std::clamp((int)(width*(value-min)/(max-min)), 0, width);
    int btnW = style.sliderHWid, btnH = style.sliderHHei;

    /// Get ID
    int _ID = iui_get_id(ID);

    /// Slider logic
    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)(sliderX - btnW/2), (float)(y - btnH/2), (float)btnW, (float)btnH})) {
        iui_hotItem = _ID;

        // ... and is clicked
        if(iui_activeItem == -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = _ID;
        }
    }

    // drag / slide
    bool isHot = iui_hotItem == _ID;
    bool isActive = iui_activeItem == _ID;
    if(isActive) {
        int _relX = GetMouseX() - x;
        value = min + (2*_relX*(max-min) + width)/(2*width);
        value = std::clamp(value, min, max);

        // update slider position
        sliderX = x + std::clamp((int)(width*(value-min)/(max-min)), 0, width);
    }

    // draw
    Color btnColor = style.colSlider;

    // slider guideline
    draw::iui_rect(x, y - style.sliderLineThickness/2, width, style.sliderLineThickness, style.colSliderLine);

    // slider button / handle
    if(isActive) {
        // change slider button color
        btnColor = style.colSliderActive;

        // slider guide text
        if(style.isSliderValueEnabled) {
            int hprev, yprev;
            iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
            draw::iui_label(x - 32, y, std::to_string(min), iuMint);
            draw::iui_label(x + width + 32, y, std::to_string(max), iuRed);
            iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_TOP);
            draw::iui_label(sliderX, y + btnH/2 + 10, std::to_string(value), iuCream);
            iui_setAlignment(hprev, yprev);
        }
    }else if(isHot) {
        btnColor = style.colSliderHot;

        // slider guide text
        if(style.isSliderValueEnabled) {
            int hprev, yprev;
            iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_TOP, hprev, yprev);
            draw::iui_label(sliderX, y + btnH/2 + 10, std::to_string(value), iuCream);
            iui_setAlignment(hprev, yprev);
        }
    }
    draw::iui_rect(sliderX - btnW/2, y - btnH/2, btnW, btnH, btnColor);
}

void iui_slider_h(int x, int y, int &value, int width, int min, int max, const std::string ID) {
    iui_slider_h_base<int>(x, y, value, width, min, max, ID);
}
void iui_slider_h(int x, int y, float &value, int width, float min, float max, const std::string ID) {
    iui_slider_h_base<float>(x, y, value, width, min, max, ID);
}

template<typename T>
void iui_slider_v_base(int x, int y, T &value, int height, T min, T max, const std::string ID) {
    IuiStyle &style = iuiGlobalStyle;

    int sliderY = y + std::clamp((int)(height*(value-min)/(max-min)), 0, height);
    int btnW = style.sliderVWid, btnH = style.sliderVHei;

    /// Get ID
    int _ID = iui_get_id(ID);

    /// Slider logic
    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)(x - btnW/2), (float)(sliderY - btnH/2), (float)btnW, (float)btnH})) {
        iui_hotItem = _ID;

        // ... and is clicked
        if(iui_activeItem == -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = _ID;
        }
    }

    // drag / slide
    bool isHot = iui_hotItem == _ID;
    bool isActive = iui_activeItem == _ID;
    if(isActive) {
        int _relY = GetMouseY() - y;
        value = min + (2*_relY*(max-min) + height)/(2*height);
        value = std::clamp(value, min, max);

        // update slider position
        sliderY = y + std::clamp((int)(height*(value-min)/(max-min)), 0, height);
    }

    // draw
    Color btnColor = style.colSlider;

    // slider guideline
    draw::iui_rect(x - style.sliderLineThickness/2, y, style.sliderLineThickness, height, style.colSliderLine);

    // slider button / handle
    if(isActive) {
        // change slider button color
        btnColor = style.colSliderActive;

        // slider guide text
        if(style.isSliderValueEnabled) {
            int hprev, yprev;
            iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
            draw::iui_label(x, y - 32, std::to_string(min), iuMint);
            draw::iui_label(x, y + height + 32, std::to_string(max), iuRed);
            iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE);
            draw::iui_label(x + btnW/2 + 10, sliderY, std::to_string(value), iuCream);
            iui_setAlignment(hprev, yprev);
        }
    }else if(isHot) {
        btnColor = style.colSliderHot;

        // slider guide text
        if(style.isSliderValueEnabled) {
            int hprev, yprev;
            iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
            draw::iui_label(x + btnW/2 + 10, sliderY, std::to_string(value), iuCream);
            iui_setAlignment(hprev, yprev);
        }
    }
    draw::iui_rect(x - btnW/2, sliderY - btnH/2, btnW, btnH, btnColor);
}

void iui_slider_v(int x, int y, int &value, int height, int min, int max, const std::string ID) {
    iui_slider_v_base<int>(x, y, value, height, min, max, ID);
}

void iui_slider_v(int x, int y, float &value, int height, float min, float max, const std::string ID) {
    iui_slider_v_base<float>(x, y, value, height, min, max, ID);
}

} // namespace element
} // namespace imnotgui