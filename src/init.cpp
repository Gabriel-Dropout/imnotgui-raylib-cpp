#include <map>
#include <algorithm>

#include "raylib/raylib-cpp.hpp"
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

// Style
const raylib::Color iuHellaDark(0x191817ff),
                    iuDark(0x313435ff),
                    iuDark2(0x3f494fff),
                    iuNormal(0x637674ff),
                    iuCream(0xefe8c4ff),
                    iuCreamDark(0xe0d3a7ff),
                    iuMint(0x25cda3ff),
                    iuSky(0x68b9c8ff),
                    iuRed(0xed3255ff),
                    iuPiss(0xeabf11ff),
                    iuBrown(0x5A4D48ff);

bool iuiButtonShadow    = false;
raylib::Color   iuiColButtonShadow = iuHellaDark,
                iuiColButtonBackdrop    = iuDark2,
                iuiColButtonBackdropTop = iuMint,
                iuiColButtonActiveBackdrop     = iuHellaDark,
                iuiColButtonActiveBackdropTop  = iuMint,
                iuiColButtonActiveBackdropTop2 = iuPiss, // when active but mouse is out of the button
                iuiColButtonHotBackdrop    = iuNormal,
                iuiColButtonHotBackdropTop = iuMint,
                iuiColButtonLabel          = iuCreamDark;

raylib::Color   iuiColTabLabel     = iuCream,
                iuiColTabHot       = iuNormal,
                iuiColTabHotAccent = iuPiss,
                iuiColTabCurrent       = iuHellaDark,
                iuiColTabCurrentAccent = iuMint;

int iuiColTabNum    = 2; // number of tab colours

raylib::Color   iuiColTab[2]       = {iuDark, iui_colLighter(iuDark, -5)},
                iuiColTabAccent[2] = {iuNormal, iui_colLighter(iuNormal, -5)};

bool iuiTextBoxRainbow   = true; // rainbow colour when active
raylib::Color   iuiColTextBoxFill   = iui_colLighter(iuHellaDark, 5),
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

int iuiSliderThick = 8; // How thick the guideline(?) is

raylib::Color   iuiColSliderLine   = iuHellaDark,
                iuiColSlider       = iuNormal,
                iuiColSliderActive = iuDark2,
                iuiColSliderHot    = iui_colLighter(iuNormal, 10);

// Checkbox
raylib::Color   iuiColCheckboxBorder = iuCream,
                iuiColCheckboxBG = iuHellaDark,
                iuiColCheckboxFG = iuMint; // the checker colour

} // namespace imnotgui