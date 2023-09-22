#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "imnotgui.hpp"

namespace imnotgui {
namespace element {
bool iui_textbox(Rectangle rect, std::string &text, const std::string ID) {return iui_textbox(rect.x, rect.y, rect.width, rect.height, text, ID);}
bool iui_intbox(Rectangle rect, std::string &text, int &ret, const std::string ID) {return iui_intbox(rect.x, rect.y, rect.width, rect.height, text, ret, ID);}
bool iui_floatbox(Rectangle rect, std::string &text, float &ret, const std::string ID) {return iui_floatbox(rect.x, rect.y, rect.width, rect.height, text, ret, ID);}

bool iui_textbox_base(int x, int y, int w, int h, std::string &text, bool (*validChecker)(std::string, int, char), const std::string ID) {
    IuiStyle &style = iuiGlobalStyle;
    // variable names are too long...
    int _bT = style.textboxBorderThickness;
    int _tP = style.textboxTextPadding;
    int _cT = style.textboxCursorThickness;
    int _cP = style.textboxCursorPadding;

    /// Setup
    bool retValue = false;
    int insideWid = w - _bT*2; // used both for drawing and trimming string.
    int maxTextWid = insideWid - _tP*2;

    /// Get ID & vars
    int _ID = iui_get_id(ID);

    /// Textbox logic
    // is hover
    makeHoverable(_ID, x, y, w, h);
    makeActivable(_ID);
    if(makePressable(_ID)) {
        iui_kbFocusItem = _ID;

        // Cursor pointing logic
        iui_textboxSelectIdx = text.size();  // default value
        int _relX = GetMouseX() - x - _bT - _tP;
        for(int i = 0; i < text.size() - iui_textboxShowPos; i++) {
            int _tmpWid1 = iui_measureText(text.substr(iui_textboxShowPos, i));
            int _tmpWid2 = iui_measureText(text.substr(iui_textboxShowPos, i+1));
            int _averWid = (_tmpWid1 + _tmpWid2) / 2;
            if(_relX < _averWid) {
                iui_textboxSelectIdx = iui_textboxShowPos + i;
                break;
            }
        }
    } else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && iui_kbFocusItem == _ID) {
        iui_kbFocusItem = -1;
        iui_textboxSelectIdx = 0;
        iui_textboxShowPos = 0;
    }

    // typing stuff
    bool isActive = iui_activeItem == _ID;
    bool isFocus = iui_kbFocusItem == _ID;
    bool isHot = iui_hotItem == _ID;

    if(isFocus) {
        // key input, cursor moving
        int codepoint = GetCharPressed();
        
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_BACKSPACE))
            iui_textboxCooldownTimer++;
        else {
            iui_textboxCooldownTimer = 0;
            iui_textboxDelayTimer = 0;
        }

        if(codepoint != 0) {
            if(validChecker(text, iui_textboxSelectIdx, (char) codepoint)) {
                text.insert(iui_textboxSelectIdx, 1, (char)codepoint);
                iui_textboxSelectIdx++;
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_BACKSPACE) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                if(iui_textboxSelectIdx > 0) {
                    text.erase(iui_textboxSelectIdx - 1, 1);
                    iui_textboxSelectIdx--;
                }
            }
        }
        if(IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_RIGHT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxSelectIdx = std::min(iui_textboxSelectIdx + 1, (int)text.size());
            }
        }
        if(IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_LEFT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxSelectIdx = std::max(iui_textboxSelectIdx - 1, 0);
            }
        }
        if(IsKeyPressed(KEY_ENTER)) {
            iui_kbFocusItem = -1;
            iui_textboxSelectIdx = 0;
            iui_textboxShowPos = 0;
            retValue = true;
        }

        /* showPos correction : based on the for invariant *\
            1. showPos >= 0
            2. width(showPos - 1 ~ end) > maxTextWid
            3. showPos <= cursorPos
            4. width(showPos ~ cursorPos - 1) <= maxTextWid

            important part: invariant 1 dominates 2.
        \*                                                 */
        iui_textboxShowPos = std::clamp(iui_textboxShowPos, 0, iui_textboxSelectIdx);  // invariant 1 and 3
        while(iui_textboxShowPos>0 && iui_measureText(text.substr(iui_textboxShowPos-1, std::string::npos)) <= maxTextWid)  // invariant 2
            iui_textboxShowPos--;
        
        while(iui_measureText(text.substr(iui_textboxShowPos, iui_textboxSelectIdx-iui_textboxShowPos)) > maxTextWid)  // invariant 4
            iui_textboxShowPos++;
    }

    int currentShowPos = isFocus ? iui_textboxShowPos : 0;
    // text trimming
    int trimLen;
    std::string trimText;
    bool mustTrim;
    for(trimLen = 1; currentShowPos + trimLen <= text.size(); trimLen++) {
        int _charWid = iui_measureText(text.substr(currentShowPos, trimLen));
        if(_charWid > maxTextWid) {
            trimLen--;
            break;
        }
    }
    trimText = text.substr(currentShowPos, trimLen);
    mustTrim = currentShowPos + trimLen < text.size();

    /// DRAW
    // box
    Color fillColor = style.colTextBoxFill;
    Color borderColor = style.colTextBoxBorder;
    if(isActive || isFocus) {
        fillColor = style.colTextBoxActiveFill;
        if(style.isTextBoxRainbowEnabled){
            borderColor = ColorFromHSV(iui_animTimer % 360, 222.0f/255.0f, 1.0f);
        } else {
            borderColor = style.colTextBoxActiveBorder;
        }
    } else if(isHot) {
        fillColor = style.colTextBoxHotFill;
        borderColor = style.colTextBoxHotBorder;
    }

    draw::iui_rect(x, y, w, h, borderColor);
    draw::iui_rect(x + _bT, y + _bT, w - _bT*2, h - _bT*2, fillColor);

    // text
    int hprev, yprev;
    iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
    int textX = x + (_bT + _tP), textY = y + h/2;
    if(!mustTrim)
        draw::iui_label(textX, textY, trimText, style.colTextBoxText);
    else {
        std::string trimBase = trimText.substr(0, trimText.size()-3), trimChar;
        int spacing = std::max(style.labelFontsize/10, 1);

        draw::iui_label(textX, textY, trimBase, style.colTextBoxText);

        textX += iui_measureText(trimBase) + spacing;
        trimChar = {trimText.at(trimText.size()-3)};
        draw::iui_label(textX, textY + sin(iui_animTimer * 0.1f)*2, trimChar, Fade(style.colTextBoxText, 0.75f));

        textX += iui_measureText(trimChar) + spacing;
        trimChar = {trimText.at(trimText.size()-2)};
        draw::iui_label(textX, textY + sin(iui_animTimer * 0.1f + 42)*4, trimChar, Fade(style.colTextBoxText, 0.5f));

        textX += iui_measureText(trimChar) + spacing;
        trimChar = {trimText.at(trimText.size()-1)};
        draw::iui_label(textX, textY + sin(iui_animTimer * 0.1f + 84)*6, trimChar, Fade(style.colTextBoxText, 0.25f));

    }
    iui_setAlignment(hprev, yprev);

    // cursor
    if(isFocus) {
        int cursorX = x + (_bT + _tP) + iui_measureText(trimText.substr(0, iui_textboxSelectIdx - iui_textboxShowPos)) - _cT/2;
        draw::iui_rect(cursorX, y + _cP, _cT, h - _cP*2, Fade(style.colButtonLabel, sin(iui_animTimer*0.1f)));
    }

    return retValue;
}

/// checkers
bool stringChecker(std::string text, int cursorPos, char codepoint) {
    return true;
}
bool intChecker(std::string text, int cursorPos, char codepoint) {
    bool isNegative = text.size() > 0 && text.at(0) == '-';
    int numChar = text.size() - isNegative;
    if(((numChar < 9) && ('0' <= codepoint && codepoint <= '9')) ||
       (codepoint == '-' && iui_textboxSelectIdx == 0 && !isNegative)) {
        return true;
    }
    return false;
}
bool floatChecker(std::string text, int cursorPos, char codepoint) {
    bool isNegative = text.size()>0 && text.at(0) == '-';
    bool hasDot = text.find('.') != std::string::npos;
    int numChar = text.size() - isNegative - hasDot;
    if(((numChar <10) && ('0' <= codepoint && codepoint <= '9')) ||
        (codepoint == '-' && iui_textboxSelectIdx == 0 && !isNegative) ||
        (codepoint == '.' && !hasDot)) {
        return true;
    }
    return false;
}

/// TextBoxes
bool iui_textbox(int x, int y, int w, int h, std::string &text, const std::string ID) {
    bool retValue = iui_textbox_base(x, y, w, h, text, stringChecker, ID);
    return retValue;
}
bool iui_intbox(int x, int y, int w, int h, std::string &text, int &ret, const std::string ID) {
    if(iui_kbFocusItem != iui_get_id(ID))  // if textbox is not focused, sync it
        text = std::to_string(ret);

    bool retValue = iui_textbox_base(x, y, w, h, text, intChecker, ID);

    // Sync text -> ret
    if(text.size() > 0 && text != "-")
        ret = std::stoi(text);
    return retValue;
}
bool iui_floatbox(int x, int y, int w, int h, std::string &text, float &ret, const std::string ID) {
    if(iui_kbFocusItem != iui_get_id(ID))  // if textbox is not focused, sync it
        text = std::to_string(ret);

    bool retValue = iui_textbox_base(x, y, w, h, text, floatChecker, ID);
    if (text.size() > 0 && text != "-" && text != "." && text != "-.")
        ret = std::stof(text);
    return retValue;
}

} // namespace element
} // namespace imnotgui