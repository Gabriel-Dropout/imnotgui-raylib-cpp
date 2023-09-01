#include <map>
#include <algorithm>

#include "imnotgui.hpp"

namespace imnotgui {

// Global variables
int iui_hotItemBack = -1;
int iui_hotItem = -1;
int iui_activeItem = -1;
int iui_kbFocusItem = -1;
int iui_idx = 0;
std::map<std::string, int> iui_idMap;

int iui_textboxShowPos = 0;
int iui_textboxCursorPos = 0;
int iui_textboxCooldownTimer = 0;
int iui_textboxDelayTimer = 0;

int iui_animTimer = 0;


// Style
const Color
    iuHellaDark = {0x19, 0x18, 0x17, 0xff},
    iuDark = {0x31, 0x34, 0x35, 0xff},
    iuDark2 = {0x3f, 0x49, 0x4f, 0xff},
    iuNormal = {0x63, 0x76, 0x74, 0xff},
    iuCream = {0xef, 0xe8, 0xc4, 0xff},
    iuCreamDark = {0xe0, 0xd3, 0xa7, 0xff},
    iuMint = {0x25, 0xcd, 0xa3, 0xff},
    iuSky = {0x68, 0xb9, 0xc8, 0xff},
    iuRed = {0xed, 0x32, 0x55, 0xff},
    iuPiss = {0xea, 0xbf, 0x11, 0xff},
    iuBrown = {0x5a, 0x4d, 0x48, 0xff};


// set variables to default. Refer to the above initialization.
IuiStyle::IuiStyle() {
    // Label
    labelFontsize=20;
    labelHalign=IUI_LABEL_ALIGN_LEFT;
    labelValign=IUI_LABEL_ALIGN_TOP;
    font = nullptr;

    // Button
    isButtonShadowEnabled = false;
    buttonShadowOffsetX = 8;
    buttonShadowOffsetY = 8;
    buttonAccentHei = 5;
    buttonActiveAccentHei = 9;

    colButtonShadow = iuHellaDark;
    colButtonBackdrop = iuDark2;
    colButtonBackdropTop = iuMint;
    colButtonActiveBackdrop = iuHellaDark;
    colButtonActiveBackdropTop = iuMint;
    colButtonActiveBackdropTop2 = iuPiss; // when active but mouse is out of the button
    colButtonHotBackdrop = iuNormal;
    colButtonHotBackdropTop = iuMint;
    colButtonLabel = iuCreamDark;

    // Tab
    tabAccentHei = 5;
    colTabLabel = iuCream;
    colTabHot = iuNormal;
    colTabHotAccent = iuPiss;
    colTabCurrent = iuHellaDark;
    colTabCurrentAccent = iuMint;


    colTab[0] = iuDark;
    colTab[1] = iui_colLighter(iuDark, -5);
    colTabAccent[0] = iuNormal;
    colTabAccent[1] = iui_colLighter(iuNormal, -5);

    isTextBoxRainbowEnabled   = true; // rainbow colour when active
    textboxBorderThickness = 2;
    textboxTextPadding = 8;
    textboxCursorThickness = 2;
    textboxCursorPadding = 10;

    colTextBoxFill   = iui_colLighter(iuHellaDark, 5);
    colTextBoxText   = iuCream;
    colTextBoxBorder = iuSky;
    colTextBoxActiveFill   = iuHellaDark;
    colTextBoxActiveBorder = iuHellaDark;
    colTextBoxHotFill   = iui_colLighter(iuHellaDark, 7);
    colTextBoxHotBorder = iuMint;

    // Slider
    isSliderValueEnabled = true; // display min, max and value on active?

    sliderHWid = 20; // horizontal
    sliderHHei = 42;

    sliderVWid = 42; // vertical
    sliderVHei = 20;

    sliderLineThickness = 8; // How thick the guideline(?) is

    sliderEndValueOffset = 32;
    sliderCurValueOffset = 10;

    colSliderLine   = iuHellaDark,
    colSlider       = iuNormal,
    colSliderActive = iuDark2,
    colSliderHot    = iui_colLighter(iuNormal, 10);

    // Checkbox
    int checkboxBorderThickness = 2;
    colCheckboxBorder = iuCream,
    colCheckboxBG = iuHellaDark,
    colCheckboxFG = iuMint; // the checker colour
}

IuiStyle iuiGlobalStyle;

} // namespace imnotgui