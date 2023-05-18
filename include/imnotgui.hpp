#ifndef IMNOTGUI_HPP
#define IMNOTGUI_HPP

#include <map>
#include <string>
#include <vector>
#include "raylib/raylib-cpp.hpp"

namespace imnotgui {
extern int iui_hotItem;
extern int iui_activeItem;
extern int iui_kbFocusItem;
extern int iui_idx;
extern std::map<std::string, int> iui_idMap;
extern int iui_textboxShowPos;
extern int iui_textboxCursorPos;
#define TEXTBOX_COOLDOWN 40
#define TEXTBOX_DELAY 1
extern int iui_textboxCooldownTimer;
extern int iui_textboxDelayTimer;

extern int iui_animTimer;

namespace draw {
    void iui_rect(int x, int y, int w, int h, raylib::Color color);
    void iui_rect_pos(int x1, int y1, int x2, int y2, raylib::Color color);
    void iui_rect_rot(int x, int y, int w, int h, raylib::Color color, float angle);
    void iui_rect_rot_center(int x, int y, int w, int h, raylib::Color color, float angle);
    void iui_rect_rot_origin(int x, int y, int w, int h, raylib::Color color, float angle, int ox, int oy);
    void iui_line(int x, int y, int length, float angle, float thick, raylib::Color color);
    void iui_label(int x, int y, std::string text, raylib::Color color);
    void iui_label_transform(int x, int y, std::string text, float fontsize, float angle, raylib::Color color);
    void iui_label_shadow(int x, int y, std::string text, raylib::Color color, int sx, int sy, raylib::Color scolor);
    void iui_label_underline_expensive(int x, int y, std::string text, raylib::Color color, float thick, int offsetY, raylib::Color bgColor);
    void iui_label_underline(int x, int y, std::string text, raylib::Color color, float thick, int offsetY);
    void iui_label_ext(int x, int y, std::string text, raylib::Color color, int sep, int width);
}

namespace element {
    bool iui_button(int x, int y, int w, int h, std::string text);
    bool iui_button_nodraw(int x, int y, int w, int h, std::string text);
    int iui_tab(int x, int y, int w, int h, std::vector<std::string> textVec, int &tabIdx, int trimMode);
    bool iui_textbox(int x, int y, int w, int h, std::string &text, const std::string ID);
    bool iui_intbox(int x, int y, int w, int h, std::string &text, int &ret, const std::string ID);
    bool iui_floatbox(int x, int y, int w, int h, std::string &text, float &ret, const std::string ID);
    void iui_slider_h(int x, int y, int &value, int width, int min, int max, const std::string ID);
    void iui_slider_v(int x, int y, int &value, int height, int min, int max, const std::string ID);
    bool iui_checkbox(int x, int y, int w, int h, bool checked, const std::string ID);
}

std::string iui_trim_label(std::string text);  // Cuts and returns the label part of the input string
std::string iui_trim_id(std::string text);  // Cuts and returns the ID part of the input string

void iui_get_all(std::string text, int &id, std::string &label);  // Gets LABEL and ID from string
int iui_get_id(std::string text);  // Gets ID from string
std::string iui_get_label(std::string text);  // Gets LABEL from string. Same as iui_trim_label

void iui_setAlignment(int halign, int valign);  // Sets the alignment
void iui_setAlignment(int halign, int valign, int &hprev, int &yprev);  // Sets the alignment and returns the previous alignment
void iui_getAlignment(int &hprev, int &yprev);  // Returns the current alignment
int iui_setFontSize(int size);  // Sets the font size and returns the previous font size
int iui_getFontSize();  // Returns the current font size

void iui_begin();  // Begin IMNOTGUI
void iui_end();  // End IMNOTGUI

std::string iui_strTrim(std::string text, int width);
std::string iui_strTrimNodots(std::string text, int width);

raylib::Color iui_colLighter(raylib::Color color, int amount);
raylib::Color iui_colLighter_adv(raylib::Color color, int amount, float rmod, float gmod, float bmod);

int iui_measureText(std::string text);
raylib::Vector2 iui_measureTextEx(std::string text);

enum iuLabelHAlignment {
    IUI_LABEL_ALIGN_LEFT,
    IUI_LABEL_ALIGN_CENTER,
    IUI_LABEL_ALIGN_RIGHT
};
enum iuLabelVAlignment {
    IUI_LABEL_ALIGN_TOP,
    IUI_LABEL_ALIGN_MIDDLE,
    IUI_LABEL_ALIGN_BOTTOM
};
extern int iuiLabelFontsize, iuiLabelHalign, iuiLabelValign;

extern const raylib::Color iuHellaDark,
                    iuDark,
                    iuDark2,
                    iuNormal,
                    iuCream,
                    iuCreamDark,
                    iuMint,
                    iuSky,
                    iuRed,
                    iuPiss,
                    iuBrown;

extern bool iuiButtonShadow;
extern raylib::Color   iuiColButtonShadow,
                iuiColButtonBackdrop,
                iuiColButtonBackdropTop,
                iuiColButtonActiveBackdrop,
                iuiColButtonActiveBackdropTop,
                iuiColButtonActiveBackdropTop2,// when active but mouse is out of the button
                iuiColButtonHotBackdrop,
                iuiColButtonHotBackdropTop,
                iuiColButtonLabel;

extern raylib::Color   iuiColTabLabel,
                iuiColTabHot,
                iuiColTabHotAccent,
                iuiColTabCurrent,
                iuiColTabCurrentAccent;

extern int iuiColTabNum; // number of tab colours

extern raylib::Color   iuiColTab[2],
                iuiColTabAccent[2];

extern bool iuiTextBoxRainbow; // rainbow colour when active
extern raylib::Color   iuiColTextBoxFill,
                iuiColTextBoxText,
                iuiColTextBoxBorder,
                iuiColTextBoxActiveFill,
                iuiColTextBoxActiveBorder,
                iuiColTextBoxHotFill,
                iuiColTextBoxHotBorder;

// Slider

extern bool iuiSliderDisplayValue; // display min, max and value on active?

extern int  iuiSliderHWid, // horizontal
            iuiSliderHHei;

extern int  iuiSliderVWid, // vertical
            iuiSliderVHei;

extern int iuiSliderThick; // How thick the guideline(?) is

extern raylib::Color   iuiColSliderLine,
                iuiColSlider,
                iuiColSliderActive,
                iuiColSliderHot;

// Checkbox
extern raylib::Color   iuiColCheckboxBorder,
                iuiColCheckboxBG,
                iuiColCheckboxFG; // the checker colour

}

#endif // IMNOTGUI_HPP