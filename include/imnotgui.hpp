/*
The source code author is @Gabriel-Dropout.
imnotgui-raylib-cpp is a ported version of [ZIK's ImNotGUI UI framework](https://github.com/TandyRum1024/sincerly-imnotgui-gml/tree/master)
for use with [Raylib-cpp](https://github.com/RobLoach/raylib-cpp).
The repository is also influenced by Omar Cornut's [Dear ImGUI](https://github.com/ocornut/imgui).
*/
#ifndef IMNOTGUI_HPP
#define IMNOTGUI_HPP

#include <map>
#include <string>
#include <vector>

#include "raylib.h"

namespace imnotgui {
extern int iui_hotItemBack;  // back buffer for iui_hotItem. Each element updates the back buffer when it is hovered over
extern int iui_hotItem;  // The item that is currently hovered over(actually 1 frame behind)
extern int iui_activeItem;  // The item that is currently active
extern int iui_kbFocusItem;  // The item that is currently focused by keyboard
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
    void iui_rect(int x, int y, int w, int h, Color color);
    void iui_rect_pos(int x1, int y1, int x2, int y2, Color color);
    void iui_rect_rot(int x, int y, int w, int h, Color color, float angle);
    void iui_rect_rot_center(int x, int y, int w, int h, Color color, float angle);
    void iui_rect_rot_origin(int x, int y, int w, int h, Color color, float angle, int ox, int oy);
    void iui_line(int x, int y, int length, float angle, float thick, Color color);
    void iui_label(int x, int y, std::string text, Color color);
    void iui_label_transform(int x, int y, std::string text, float fontsize, float angle, Color color);
    void iui_label_shadow(int x, int y, std::string text, Color color, int sx, int sy, Color scolor);
    void iui_label_underline_expensive(int x, int y, std::string text, Color color, float thick, int offsetY, Color bgColor);
    void iui_label_underline(int x, int y, std::string text, Color color, float thick, int offsetY);
    void iui_label_ext(int x, int y, std::string text, Color color, int sep, int width);
}

namespace element {
    bool iui_button(int x, int y, int w, int h, std::string text);
    bool iui_button_nodraw(int x, int y, int w, int h, std::string text);
    int iui_tab(int x, int y, int w, int h, const std::vector<std::string> &textVec, int &tabIdx, int trimMode);
    int iui_tab_v(int x, int y, int w, int h, const std::vector<std::string> &textVec, int &tabIdx, int trimMode);
    bool iui_textbox(int x, int y, int w, int h, std::string &text, const std::string ID);
    bool iui_intbox(int x, int y, int w, int h, std::string &text, int &ret, const std::string ID);
    bool iui_floatbox(int x, int y, int w, int h, std::string &text, float &ret, const std::string ID);
    void iui_slider_h(int x, int y, int &value, int width, int min, int max, const std::string ID);
    void iui_slider_h(int x, int y, float &value, int width, float min, float max, const std::string ID);
    void iui_slider_v(int x, int y, int &value, int height, int min, int max, const std::string ID);
    void iui_slider_v(int x, int y, float &value, int height, float min, float max, const std::string ID);
    bool iui_checkbox(int x, int y, int w, int h, bool checked, const std::string ID);
}

void iui_begin();  // Begin IMNOTGUI
void iui_end();    // End IMNOTGUI

inline bool makeHoverable(int ID, Rectangle rect) {
    if(CheckCollisionPointRec(GetMousePosition(), rect))
        iui_hotItemBack = ID;

    return iui_hotItem == ID;
}
inline bool makeHoverable(int ID, int x, int y, int w, int h) {
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)w, (float)h}))
        iui_hotItemBack = ID;

    return iui_hotItem == ID;
}
inline bool makeActivable(int ID) {  // Returns true if the item is active
    if(iui_hotItem == ID && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        iui_activeItem = ID;

    return iui_activeItem == ID;
}
inline bool makePressable(int ID) {  // Returns true if the item is pressed
    if(iui_hotItem == ID && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return true;
    
    return false;
}
inline bool makeClickable(int ID) {
    return (iui_hotItem == ID && iui_activeItem == ID && IsMouseButtonReleased(MOUSE_LEFT_BUTTON));
}

// State management functions
void iui_setAlignment(int halign, int valign);  // Sets the alignment
void iui_setAlignment(int halign, int valign, int &hprev, int &yprev);  // Sets the alignment and returns the previous alignment
void iui_getAlignment(int &hprev, int &yprev);  // Returns the current alignment
int iui_setFontSize(int size);  // Sets the font size and returns the previous font size
int iui_getFontSize();  // Returns the current font size
Font* iui_setFont(Font* font);  // Sets the font and returns the previous font
Font* iui_getFont();  // Returns the current font

// Scoped State management classes
class ScopedStateSetter {
// non-copyable
private:
    auto operator=( ScopedStateSetter const& ) -> ScopedStateSetter& = delete;
    ScopedStateSetter( ScopedStateSetter const& ) = delete;
public:
    auto operator=( ScopedStateSetter&& ) -> ScopedStateSetter& = default;
    ScopedStateSetter() = default;
    ScopedStateSetter( ScopedStateSetter&& ) = default;

// non-constructible
public:
    template<typename... Args> void* operator new(std::size_t,Args...) = delete;
};

class ScopedAlignmentSetter: ScopedStateSetter {
private:
    int hprev, vprev;
public:
    ScopedAlignmentSetter(int halign, int valign) {iui_setAlignment(halign, valign, hprev, vprev);}
    ~ScopedAlignmentSetter() {iui_setAlignment(hprev, vprev);}
};

class ScopedFontSizeSetter: ScopedStateSetter {
private:
    int prev;
public:
    ScopedFontSizeSetter(int size) {prev = iui_setFontSize(size);}
    ~ScopedFontSizeSetter() {iui_setFontSize(prev);}
};

class ScopedFontSetter: ScopedStateSetter {
private:
    Font* prev;
public:
    ScopedFontSetter(Font* font) {prev = iui_setFont(font);}
    ~ScopedFontSetter() {iui_setFont(prev);}
};

std::string iui_trim_label(std::string text);  // Cuts and returns the label part of the input string
std::string iui_trim_idstr(std::string text);  // Cuts and returns the ID part of the input string

void iui_get_all(std::string text, int &id, std::string &label);  // Gets LABEL and ID from string
int iui_get_id(std::string text);  // Gets ID from string
std::string iui_get_label(std::string text);  // Gets LABEL from string. Same as iui_trim_label

std::string iui_strTrimDots(std::string text, int width);
std::string iui_strTrim(std::string text, int width);

Color iui_colLighter(Color color, int amount);
Color iui_colLighter_adv(Color color, int amount, float rmod, float gmod, float bmod);

int iui_measureText(std::string text);
Vector2 iui_measureTextEx(std::string text);

extern const Color  iuHellaDark,
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
enum iuTabTrimMode {
    IUI_TAB_TRIM,
    IUI_TAB_FLEX,
};

typedef struct IuiStyle {
    /// Label
    int labelFontsize, labelHalign, labelValign;
    Font* font;

    /// Button
    bool isButtonShadowEnabled;
    int buttonShadowOffsetX, buttonShadowOffsetY;
    int buttonAccentHei, buttonActiveAccentHei;
    Color   colButtonLabel,
            colButtonBackdrop,
            colButtonBackdropTop,
            colButtonActiveBackdrop,
            colButtonActiveBackdropTop,
            colButtonActiveBackdropTop2,// when active but mouse is out of the button
            colButtonHotBackdrop,
            colButtonHotBackdropTop,
            colButtonShadow;

    /// Tab
    static constexpr int colTabNum = 2; // number of tab colours
    int tabAccentHei;
    Color   colTabLabel,
            colTab[2],
            colTabAccent[2],
            colTabHot,
            colTabHotAccent,
            colTabCurrent,
            colTabCurrentAccent;

    /// Textbox
    bool isTextBoxRainbowEnabled; // rainbow colour when active
    int textboxBorderThickness, textboxTextPadding, textboxCursorThickness, textboxCursorPadding;
    Color   colTextBoxText,
            colTextBoxFill,
            colTextBoxBorder,
            colTextBoxActiveFill,
            colTextBoxActiveBorder,
            colTextBoxHotFill,
            colTextBoxHotBorder;

    /// Slider
    bool isSliderValueEnabled; // display min, max and value on active?

    int sliderHWid, // horizontal
        sliderHHei;

    int sliderVWid, // vertical
        sliderVHei;

    int sliderLineThickness; // How thick the guideline(?) is

    int sliderEndValueOffset, // How far the min and max values are from the slider
        sliderCurValueOffset; // How far the current value is from the slider

    Color   colSliderLine,
            colSlider,
            colSliderActive,
            colSliderHot;

    // Checkbox
    int checkboxBorderThickness;
    Color   colCheckboxBorder,
            colCheckboxBG,
            colCheckboxFG; // the checker colour

    IuiStyle();
} IuiStyle;

extern IuiStyle iuiGlobalStyle;

}

#endif // IMNOTGUI_HPP