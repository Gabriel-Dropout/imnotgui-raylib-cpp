#include <map>
#include <algorithm>

#include "imnotgui.hpp"

namespace imnotgui {

// Global variables
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


int iuiLabelFontsize=20, iuiLabelHalign=IUI_LABEL_ALIGN_LEFT, iuiLabelValign=IUI_LABEL_ALIGN_TOP;
Font* iuiFont = nullptr;

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


bool iuiButtonShadow    = false;
Color   iuiColButtonShadow = iuHellaDark,
                iuiColButtonBackdrop    = iuDark2,
                iuiColButtonBackdropTop = iuMint,
                iuiColButtonActiveBackdrop     = iuHellaDark,
                iuiColButtonActiveBackdropTop  = iuMint,
                iuiColButtonActiveBackdropTop2 = iuPiss, // when active but mouse is out of the button
                iuiColButtonHotBackdrop    = iuNormal,
                iuiColButtonHotBackdropTop = iuMint,
                iuiColButtonLabel          = iuCreamDark;

Color   iuiColTabLabel     = iuCream,
                iuiColTabHot       = iuNormal,
                iuiColTabHotAccent = iuPiss,
                iuiColTabCurrent       = iuHellaDark,
                iuiColTabCurrentAccent = iuMint;

int iuiColTabNum    = 2; // number of tab colours

Color   iuiColTab[2]       = {iuDark, iui_colLighter(iuDark, -5)},
                iuiColTabAccent[2] = {iuNormal, iui_colLighter(iuNormal, -5)};

bool iuiTextBoxRainbow   = true; // rainbow colour when active
Color   iuiColTextBoxFill   = iui_colLighter(iuHellaDark, 5),
                iuiColTextBoxText   = iuCream,
                iuiColTextBoxBorder = iuSky,
                iuiColTextBoxActiveFill   = iuHellaDark,
                iuiColTextBoxActiveBorder = iuHellaDark,
                iuiColTextBoxHotFill   = iui_colLighter(iuHellaDark, 7),
                iuiColTextBoxHotBorder = iuMint;

// Slider

bool iuiSliderDisplayValue = true; // display min, max and value on active?

int iuiSliderHWid = 20, // horizontal
    iuiSliderHHei = 42;

int iuiSliderVWid = 42, // vertical
    iuiSliderVHei = 20;

int iuiSliderThickness = 8; // How thick the guideline(?) is

Color   iuiColSliderLine   = iuHellaDark,
                iuiColSlider       = iuNormal,
                iuiColSliderActive = iuDark2,
                iuiColSliderHot    = iui_colLighter(iuNormal, 10);

// Checkbox
Color   iuiColCheckboxBorder = iuCream,
                iuiColCheckboxBG = iuHellaDark,
                iuiColCheckboxFG = iuMint; // the checker colour

} // namespace imnotgui