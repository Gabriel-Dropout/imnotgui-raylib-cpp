#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "imnotgui.hpp"

namespace imnotgui {
namespace element {

bool iui_textbox_base(int x, int y, int w, int h, std::string &text, bool (*validChecker)(std::string, int, char), const std::string ID) {
    IuiStyle &style = iuiGlobalStyle;

    /// Setup
    bool retValue = false;
    int insideWid = w - 4; // used both for drawing and trimming string.
    int maxTextWid = insideWid - 16;

    /// Get ID & vars
    int _ID = iui_get_id(ID);

    /// Textbox logic
    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)w, (float)h})) {
        iui_hotItem = _ID;

        // ... and is clicked
        // TODO : Somehow implement mouse navigating (well idk what it means. Isn't it already implemented?)
        if(iui_activeItem == -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = _ID;
            iui_kbFocusItem = _ID;

            // Cursor pointing logic
            iui_textboxCursorPos = text.size();  // default value
            int _relX = GetMouseX() - x - 2 - 8;
            for(int i = 0; i < text.size() - iui_textboxShowPos; i++) {
                int _tmpWid1 = MeasureText(text.substr(iui_textboxShowPos, i).c_str(), style.labelFontsize);
                int _tmpWid2 = MeasureText(text.substr(iui_textboxShowPos, i+1).c_str(), style.labelFontsize);
                int _averWid = (_tmpWid1 + _tmpWid2) / 2;
                if(_relX < _averWid) {
                    iui_textboxCursorPos = iui_textboxShowPos + i;
                    break;
                }
            }
        }
    }
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && iui_kbFocusItem == _ID) {
        iui_kbFocusItem = -1;
        iui_textboxCursorPos = 0;
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
            if(validChecker(text, iui_textboxCursorPos, (char) codepoint)) {
                text.insert(iui_textboxCursorPos, 1, (char)codepoint);
                iui_textboxCursorPos++;
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_BACKSPACE) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                if(iui_textboxCursorPos > 0) {
                    text.erase(iui_textboxCursorPos - 1, 1);
                    iui_textboxCursorPos--;
                }
            }
        }
        if(IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_RIGHT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxCursorPos = std::min(iui_textboxCursorPos + 1, (int)text.size());
            }
        }
        if(IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_LEFT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxCursorPos = std::max(iui_textboxCursorPos - 1, 0);
            }
        }
        if(IsKeyPressed(KEY_ENTER)) {
            iui_kbFocusItem = -1;
            iui_textboxCursorPos = 0;
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
        iui_textboxShowPos = std::clamp(iui_textboxShowPos, 0, iui_textboxCursorPos);  // invariant 1 and 3
        while(iui_textboxShowPos>0 && MeasureText(text.substr(iui_textboxShowPos-1, std::string::npos).c_str(), style.labelFontsize) <= maxTextWid)  // invariant 2
            iui_textboxShowPos--;
        
        while(MeasureText(text.substr(iui_textboxShowPos, iui_textboxCursorPos-iui_textboxShowPos).c_str(), style.labelFontsize) > maxTextWid)  // invariant 4
            iui_textboxShowPos++;
    }

    int currentShowPos = isFocus ? iui_textboxShowPos : 0;
    // text trimming
    int trimLen;
    std::string trimText;
    bool mustTrim;
    for(trimLen = 1; currentShowPos + trimLen <= text.size(); trimLen++) {
        int _charWid = MeasureText(text.substr(currentShowPos, trimLen).c_str(), style.labelFontsize);
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
            borderColor = ColorFromHSV(iui_animTimer % 360, 222.0f/255.0f, 1.0f);  // TODO : make it rainbow
        } else {
            borderColor = style.colTextBoxActiveBorder;
        }
    } else if(isHot) {
        fillColor = style.colTextBoxHotFill;
        borderColor = style.colTextBoxHotBorder;
    }

    draw::iui_rect(x, y, w, h, borderColor);
    draw::iui_rect(x + 2, y + 2, w - 4, h - 4, fillColor);

    // text
    int hprev, yprev;
    iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
    if(!mustTrim)
        draw::iui_label(x + 10, y + h/2, trimText, style.colTextBoxText);
    else {
        std::string trimBase = trimText.substr(0, trimText.size()-3);
        int textOffX;
        int spacing = std::max(style.labelFontsize/10, 1);
        std::string trimChar;

        draw::iui_label(x + 10, y + h/2, trimText.substr(0, trimText.size()-3), style.colTextBoxText);

        textOffX = MeasureText(trimBase.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-3)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f)*2, trimChar, Fade(style.colTextBoxText, 0.75f));
        textOffX += MeasureText(trimChar.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-2)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f + 42)*4, trimChar, Fade(style.colTextBoxText, 0.5f));
        textOffX += MeasureText(trimChar.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-1)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f + 84)*6, trimChar, Fade(style.colTextBoxText, 0.25f));

    }
    iui_setAlignment(hprev, yprev);

    // cursor
    if(isFocus) {
        int cursorX = x + 10 + MeasureText(trimText.substr(0, iui_textboxCursorPos - iui_textboxShowPos).c_str(), style.labelFontsize) - 2;
        draw::iui_rect(cursorX, y + 10, 4, h - 20, Fade(style.colButtonLabel, sin(iui_animTimer*0.1f)));
    }

    return retValue;
}

bool stringChecker(std::string text, int cursorPos, char codepoint) {
    return true;
}
bool intChecker(std::string text, int cursorPos, char codepoint) {
    bool isNegative = text.size() > 0 && text.at(0) == '-';
    int numChar = text.size() - isNegative;
    if(((numChar < 9) && ('0' <= codepoint && codepoint <= '9')) ||
       (codepoint == '-' && iui_textboxCursorPos == 0 && !isNegative)) {
        return true;
    }
    return false;
}
bool floatChecker(std::string text, int cursorPos, char codepoint) {
    bool isNegative = text.size()>0 && text.at(0) == '-';
    bool hasDot = text.find('.') != std::string::npos;
    int numChar = text.size() - isNegative - hasDot;
    if(((numChar <10) && ('0' <= codepoint && codepoint <= '9')) ||
        (codepoint == '-' && iui_textboxCursorPos == 0 && !isNegative) ||
        (codepoint == '.' && !hasDot)) {
        return true;
    }
    return false;
}

bool iui_textbox(int x, int y, int w, int h, std::string &text, const std::string ID) {
    bool retValue = iui_textbox_base(x, y, w, h, text, stringChecker, ID);
    return retValue;
}
bool iui_intbox(int x, int y, int w, int h, std::string &text, int &ret, const std::string ID) {
    // Sync ret -> text
    //if(text.size() > 0 && text != "-") {
    //    if(ret != std::stoi(text))  // if text is valid number but ret != text, sync it
    //        text = std::to_string(ret);
    //} else {
        if(iui_kbFocusItem != iui_get_id(ID))  // if text is invalid number and textbox is not focused, sync it
            text = std::to_string(ret);
    //}

    bool retValue = iui_textbox_base(x, y, w, h, text, intChecker, ID);

    // Sync text -> ret
    if(text.size() > 0 && text != "-")
        ret = std::stoi(text);
    return retValue;
}
bool iui_floatbox(int x, int y, int w, int h, std::string &text, float &ret, const std::string ID) {
    // Sync ret -> text
    //if (text.size() > 0 && text != "-" && text != "." && text != "-.") {
    //    if(ret != std::stoi(text))  // if text is valid number but ret != text, sync it
    //        text = std::to_string(ret);
    //} else {
        if(iui_kbFocusItem != iui_get_id(ID))  // if text is invalid number and textbox is not focused, sync it
            text = std::to_string(ret);
    //}

    bool retValue = iui_textbox_base(x, y, w, h, text, floatChecker, ID);
    if (text.size() > 0 && text != "-" && text != "." && text != "-.")
        ret = std::stof(text);
    return retValue;
}
#ifdef FALSE
bool iui_textbox(int x, int y, int w, int h, std::string &text, const std::string ID) {
    IuiStyle &style = iuiGlobalStyle;

    /// Setup
    bool retValue = false;
    int insideWid = w - 4; // used both for drawing and trimming string.
    int maxTextWid = insideWid - 16;

    /// Get ID & vars
    int _ID = iui_get_id(ID);

    /// Textbox logic
    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)w, (float)h})) {
        iui_hotItem = _ID;

        // ... and is clicked
        // TODO : Somehow implement mouse navigating (well idk what it means. Isn't it already implemented?)
        if(iui_activeItem == -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = _ID;
            iui_kbFocusItem = _ID;

            // Cursor pointing logic
            iui_textboxCursorPos = text.size();  // default value
            int _relX = GetMouseX() - x - 2 - 8;
            for(int i = 0; i < text.size() - iui_textboxShowPos; i++) {
                int _tmpWid1 = MeasureText(text.substr(iui_textboxShowPos, i).c_str(), style.labelFontsize);
                int _tmpWid2 = MeasureText(text.substr(iui_textboxShowPos, i+1).c_str(), style.labelFontsize);
                int _averWid = (_tmpWid1 + _tmpWid2) / 2;
                if(_relX < _averWid) {
                    iui_textboxCursorPos = iui_textboxShowPos + i;
                    break;
                }
            }
        }
    }
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && iui_kbFocusItem == _ID) {
        iui_kbFocusItem = -1;
        iui_textboxCursorPos = 0;
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
            text.insert(iui_textboxCursorPos, 1, (char)codepoint);
            iui_textboxCursorPos++;
        }
        if(IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_BACKSPACE) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                if(iui_textboxCursorPos > 0) {
                    text.erase(iui_textboxCursorPos - 1, 1);
                    iui_textboxCursorPos--;
                }
            }
        }
        if(IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_RIGHT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxCursorPos = std::min(iui_textboxCursorPos + 1, (int)text.size());
            }
        }
        if(IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_LEFT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxCursorPos = std::max(iui_textboxCursorPos - 1, 0);
            }
        }
        if(IsKeyPressed(KEY_ENTER)) {
            iui_kbFocusItem = -1;
            iui_textboxCursorPos = 0;
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
        iui_textboxShowPos = std::clamp(iui_textboxShowPos, 0, iui_textboxCursorPos);  // invariant 1 and 3
        while(iui_textboxShowPos>0 && MeasureText(text.substr(iui_textboxShowPos-1, std::string::npos).c_str(), style.labelFontsize) <= maxTextWid)  // invariant 2
            iui_textboxShowPos--;
        
        while(MeasureText(text.substr(iui_textboxShowPos, iui_textboxCursorPos-iui_textboxShowPos).c_str(), style.labelFontsize) > maxTextWid)  // invariant 4
            iui_textboxShowPos++;
    }

    int currentShowPos = isFocus ? iui_textboxShowPos : 0;
    // text trimming
    int trimLen;
    std::string trimText;
    bool mustTrim;
    for(trimLen = 1; currentShowPos + trimLen <= text.size(); trimLen++) {
        int _charWid = MeasureText(text.substr(currentShowPos, trimLen).c_str(), style.labelFontsize);
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
            borderColor = ColorFromHSV(iui_animTimer % 360, 222.0f/255.0f, 1.0f);  // TODO : make it rainbow
        } else {
            borderColor = style.colTextBoxActiveBorder;
        }
    } else if(isHot) {
        fillColor = style.colTextBoxHotFill;
        borderColor = style.colTextBoxHotBorder;
    }

    draw::iui_rect(x, y, w, h, borderColor);
    draw::iui_rect(x + 2, y + 2, w - 4, h - 4, fillColor);

    // text
    int hprev, yprev;
    iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
    if(!mustTrim)
        draw::iui_label(x + 10, y + h/2, trimText, style.colTextBoxText);
    else {
        std::string trimBase = trimText.substr(0, trimText.size()-3);
        int textOffX;
        int spacing = std::max(style.labelFontsize/10, 1);
        std::string trimChar;

        draw::iui_label(x + 10, y + h/2, trimText.substr(0, trimText.size()-3), style.colTextBoxText);

        textOffX = MeasureText(trimBase.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-3)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f)*2, trimChar, Fade(style.colTextBoxText, 0.75f));
        textOffX += MeasureText(trimChar.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-2)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f + 42)*4, trimChar, Fade(style.colTextBoxText, 0.5f));
        textOffX += MeasureText(trimChar.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-1)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f + 84)*6, trimChar, Fade(style.colTextBoxText, 0.25f));

    }
    iui_setAlignment(hprev, yprev);

    // cursor
    if(isFocus) {
        int cursorX = x + 10 + MeasureText(trimText.substr(0, iui_textboxCursorPos - iui_textboxShowPos).c_str(), style.labelFontsize) - 2;
        draw::iui_rect(cursorX, y + 10, 4, h - 20, Fade(style.colButtonLabel, sin(iui_animTimer*0.1f)));
    }

    return retValue;
}

bool iui_intbox(int x, int y, int w, int h, std::string &text, int &ret, const std::string ID) {
    IuiStyle &style = iuiGlobalStyle;

    /// Setup
    bool retValue = false;
    int insideWid = w - 4; // used both for drawing and trimming string.
    int maxTextWid = insideWid - 16;

    /// Get ID & vars
    int _ID = iui_get_id(ID);

    /// Textbox logic
    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)w, (float)h})) {
        iui_hotItem = _ID;

        // ... and is clicked
        // TODO : Somehow implement mouse navigating (well idk what it means. Isn't it already implemented?)
        if(iui_activeItem == -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = _ID;
            iui_kbFocusItem = _ID;

            // Cursor pointing logic
            iui_textboxCursorPos = text.size();  // default value
            int _relX = GetMouseX() - x - 2 - 8;
            for(int i = 0; i < text.size() - iui_textboxShowPos; i++) {
                int _tmpWid1 = MeasureText(text.substr(iui_textboxShowPos, i).c_str(), style.labelFontsize);
                int _tmpWid2 = MeasureText(text.substr(iui_textboxShowPos, i+1).c_str(), style.labelFontsize);
                int _averWid = (_tmpWid1 + _tmpWid2) / 2;
                if(_relX < _averWid) {
                    iui_textboxCursorPos = iui_textboxShowPos + i;
                    break;
                }
            }
        }
    }
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && iui_kbFocusItem == _ID) {
        iui_kbFocusItem = -1;
        iui_textboxCursorPos = 0;
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
            // measure number of characters except first '-'
            bool isNegative = text.size()>0 && text.at(0) == '-';
            int numChar = text.size() - isNegative;
            if(((numChar < 9) && ('0' <= codepoint && codepoint <= '9')) ||
               (codepoint == '-' && iui_textboxCursorPos == 0 && !isNegative)) {
                text.insert(iui_textboxCursorPos, 1, (char)codepoint);
                iui_textboxCursorPos++;
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_BACKSPACE) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                if(iui_textboxCursorPos > 0) {
                    text.erase(iui_textboxCursorPos - 1, 1);
                    iui_textboxCursorPos--;
                }
            }
        }
        if(IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_RIGHT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxCursorPos = std::min(iui_textboxCursorPos + 1, (int)text.size());
            }
        }
        if(IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_LEFT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxCursorPos = std::max(iui_textboxCursorPos - 1, 0);
            }
        }
        if(IsKeyPressed(KEY_ENTER)) {
            iui_kbFocusItem = -1;
            iui_textboxCursorPos = 0;
            iui_textboxShowPos = 0;
            retValue = true;
        }
        
        // if valid, update ret
        if(text.size() > 0 && text != "-")
            ret = std::stoi(text);

        /* showPos correction : based on the for invariant *\
            1. showPos >= 0
            2. width(showPos - 1 ~ end) > maxTextWid
            3. showPos <= cursorPos
            4. width(showPos ~ cursorPos - 1) <= maxTextWid

            important part: invariant 1 dominates 2.
        \*                                                 */
        iui_textboxShowPos = std::clamp(iui_textboxShowPos, 0, iui_textboxCursorPos);  // invariant 1 and 3
        while(iui_textboxShowPos>0 && MeasureText(text.substr(iui_textboxShowPos-1, std::string::npos).c_str(), style.labelFontsize) <= maxTextWid)  // invariant 2
            iui_textboxShowPos--;
        
        while(MeasureText(text.substr(iui_textboxShowPos, iui_textboxCursorPos-iui_textboxShowPos).c_str(), style.labelFontsize) > maxTextWid)  // invariant 4
            iui_textboxShowPos++;
    }

    int currentShowPos = isFocus ? iui_textboxShowPos : 0;
    // text trimming
    int trimLen;
    std::string trimText;
    bool mustTrim;
    for(trimLen = 1; currentShowPos + trimLen <= text.size(); trimLen++) {
        int _charWid = MeasureText(text.substr(currentShowPos, trimLen).c_str(), style.labelFontsize);
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
            borderColor = ColorFromHSV(iui_animTimer % 360, 222.0f/255.0f, 1.0f);  // TODO : make it rainbow
        } else {
            borderColor = style.colTextBoxActiveBorder;
        }
    } else if(isHot) {
        fillColor = style.colTextBoxHotFill;
        borderColor = style.colTextBoxHotBorder;
    }

    draw::iui_rect(x, y, w, h, borderColor);
    draw::iui_rect(x + 2, y + 2, w - 4, h - 4, fillColor);

    // text
    int hprev, yprev;
    iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
    if(!mustTrim)
        draw::iui_label(x + 10, y + h/2, trimText, style.colTextBoxText);
    else {
        std::string trimBase = trimText.substr(0, trimText.size()-3);
        int textOffX;
        int spacing = std::max(style.labelFontsize/10, 1);
        std::string trimChar;

        draw::iui_label(x + 10, y + h/2, trimText.substr(0, trimText.size()-3), style.colTextBoxText);

        textOffX = MeasureText(trimBase.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-3)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f)*2, trimChar, Fade(style.colTextBoxText, 0.75f));
        textOffX += MeasureText(trimChar.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-2)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f + 42)*4, trimChar, Fade(style.colTextBoxText, 0.5f));
        textOffX += MeasureText(trimChar.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-1)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f + 84)*6, trimChar, Fade(style.colTextBoxText, 0.25f));

    }
    iui_setAlignment(hprev, yprev);

    // cursor
    if(isFocus) {
        int cursorX = x + 10 + MeasureText(trimText.substr(0, iui_textboxCursorPos - iui_textboxShowPos).c_str(), style.labelFontsize) - 2;
        draw::iui_rect(cursorX, y + 10, 4, h - 20, Fade(style.colButtonLabel, sin(iui_animTimer*0.1f)));
    }

    return retValue;
}

bool iui_floatbox(int x, int y, int w, int h, std::string &text, float &ret, const std::string ID) {
    IuiStyle &style = iuiGlobalStyle;

    /// Setup
    bool retValue = false;
    int insideWid = w - 4; // used both for drawing and trimming string.
    int maxTextWid = insideWid - 16;

    /// Get ID & vars
    int _ID = iui_get_id(ID);

    /// Textbox logic
    // is hover
    if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)w, (float)h})) {
        iui_hotItem = _ID;

        // ... and is clicked
        // TODO : Somehow implement mouse navigating (well idk what it means. Isn't it already implemented?)
        if(iui_activeItem == -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            iui_activeItem = _ID;
            iui_kbFocusItem = _ID;

            // Cursor pointing logic
            iui_textboxCursorPos = text.size();  // default value
            int _relX = GetMouseX() - x - 2 - 8;
            for(int i = 0; i < text.size() - iui_textboxShowPos; i++) {
                int _tmpWid1 = MeasureText(text.substr(iui_textboxShowPos, i).c_str(), style.labelFontsize);
                int _tmpWid2 = MeasureText(text.substr(iui_textboxShowPos, i+1).c_str(), style.labelFontsize);
                int _averWid = (_tmpWid1 + _tmpWid2) / 2;
                if(_relX < _averWid) {
                    iui_textboxCursorPos = iui_textboxShowPos + i;
                    break;
                }
            }
        }
    }
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && iui_kbFocusItem == _ID) {
        iui_kbFocusItem = -1;
        iui_textboxCursorPos = 0;
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
            bool isNegative = text.size()>0 && text.at(0) == '-';
            bool hasDot = text.find('.') != std::string::npos;
            int numChar = text.size() - isNegative - hasDot;
            if(((numChar <10) && ('0' <= codepoint && codepoint <= '9')) ||
                (codepoint == '-' && iui_textboxCursorPos == 0 && !isNegative) ||
                (codepoint == '.' && !hasDot)) {
                text.insert(iui_textboxCursorPos, 1, (char)codepoint);
                iui_textboxCursorPos++;
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_BACKSPACE) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                if(iui_textboxCursorPos > 0) {
                    text.erase(iui_textboxCursorPos - 1, 1);
                    iui_textboxCursorPos--;
                }
            }
        }
        if(IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_RIGHT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxCursorPos = std::min(iui_textboxCursorPos + 1, (int)text.size());
            }
        }
        if(IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && iui_textboxCooldownTimer > TEXTBOX_COOLDOWN)) {
            iui_textboxDelayTimer++;
            if(IsKeyPressed(KEY_LEFT) || iui_textboxDelayTimer % TEXTBOX_DELAY == 0) {
                iui_textboxCursorPos = std::max(iui_textboxCursorPos - 1, 0);
            }
        }
        if(IsKeyPressed(KEY_ENTER)) {
            iui_kbFocusItem = -1;
            iui_textboxCursorPos = 0;
            iui_textboxShowPos = 0;
            retValue = true;
        }
        
        // if valid, update ret. consider dot(.) dot dot dot hey copilot consider dot
        if (text.size() > 0 && text != "-" && text != "." && text != "-.")
            ret = std::stof(text);  // update ret

        /* showPos correction : based on the for invariant *\
            1. showPos >= 0
            2. width(showPos - 1 ~ end) > maxTextWid
            3. showPos <= cursorPos
            4. width(showPos ~ cursorPos - 1) <= maxTextWid

            important part: invariant 1 dominates 2.
        \*                                                 */
        iui_textboxShowPos = std::clamp(iui_textboxShowPos, 0, iui_textboxCursorPos);  // invariant 1 and 3
        while(iui_textboxShowPos>0 && MeasureText(text.substr(iui_textboxShowPos-1, std::string::npos).c_str(), style.labelFontsize) <= maxTextWid)  // invariant 2
            iui_textboxShowPos--;
        
        while(MeasureText(text.substr(iui_textboxShowPos, iui_textboxCursorPos-iui_textboxShowPos).c_str(), style.labelFontsize) > maxTextWid)  // invariant 4
            iui_textboxShowPos++;
    }

    int currentShowPos = isFocus ? iui_textboxShowPos : 0;
    // text trimming
    int trimLen;
    std::string trimText;
    bool mustTrim;
    for(trimLen = 1; currentShowPos + trimLen <= text.size(); trimLen++) {
        int _charWid = MeasureText(text.substr(currentShowPos, trimLen).c_str(), style.labelFontsize);
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
            borderColor = ColorFromHSV(iui_animTimer % 360, 222.0f/255.0f, 1.0f);  // TODO : make it rainbow
        } else {
            borderColor = style.colTextBoxActiveBorder;
        }
    } else if(isHot) {
        fillColor = style.colTextBoxHotFill;
        borderColor = style.colTextBoxHotBorder;
    }

    draw::iui_rect(x, y, w, h, borderColor);
    draw::iui_rect(x + 2, y + 2, w - 4, h - 4, fillColor);

    // text
    int hprev, yprev;
    iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
    if(!mustTrim)
        draw::iui_label(x + 10, y + h/2, trimText, style.colTextBoxText);
    else {
        std::string trimBase = trimText.substr(0, trimText.size()-3);
        int textOffX;
        int spacing = std::max(style.labelFontsize/10, 1);
        std::string trimChar;

        draw::iui_label(x + 10, y + h/2, trimText.substr(0, trimText.size()-3), style.colTextBoxText);

        textOffX = MeasureText(trimBase.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-3)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f)*2, trimChar, Fade(style.colTextBoxText, 0.75f));
        textOffX += MeasureText(trimChar.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-2)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f + 42)*4, trimChar, Fade(style.colTextBoxText, 0.5f));
        textOffX += MeasureText(trimChar.c_str(), style.labelFontsize) + spacing;
        trimChar = {trimText.at(trimText.size()-1)};
        draw::iui_label(x + 10 + textOffX, y + h/2 + sin(iui_animTimer * 0.1f + 84)*6, trimChar, Fade(style.colTextBoxText, 0.25f));

    }
    iui_setAlignment(hprev, yprev);

    // cursor
    if(isFocus) {
        int cursorX = x + 10 + MeasureText(trimText.substr(0, iui_textboxCursorPos - iui_textboxShowPos).c_str(), style.labelFontsize) - 2;
        draw::iui_rect(cursorX, y + 10, 4, h - 20, Fade(style.colButtonLabel, sin(iui_animTimer*0.1f)));
    }

    return retValue;
}
#endif
} // namespace element
} // namespace imnotgui