#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "imnotgui.hpp"

namespace imnotgui {
namespace element {

void iui_multi_textbox(int x, int y, int w, int h, std::vector<std::string> &textVec, const std::string ID) {
    if(textVec.empty())
        textVec.push_back("");

    IuiStyle &style = iuiGlobalStyle;
    // variable names are too long...
    int _bT = style.textboxBorderThickness;
    int _tP = style.textboxTextPadding;
    int _cT = style.textboxCursorThickness;
    int _cP = style.textboxCursorPadding;

    /// Setup
    int insideWid = w - _bT*2, insideHei = h - _bT*2; // used both for drawing and trimming string.
    int maxTextWid = insideWid - _tP*2;
    int textHei = style.labelFontsize * 1.5f;
    int numOfLines = insideHei/textHei;

    /// Get ID & vars
    int _ID = iui_get_id(ID);

    /// Textbox logic
    auto getCurText = [&textVec]() -> std::string& {
        return textVec[iui_textboxCursorLine];
    };

    // is hover
    makeHoverable(_ID, x, y, w, h);
    makeActivable(_ID);
    if(makePressable(_ID)) {
        iui_kbFocusItem = _ID;

        // Cursor pointing logic
        int _relY = GetMouseY() - (y + h/2 - textHei*numOfLines/2);
        iui_textboxCursorLine = iui_textboxShowLine + std::clamp(_relY/textHei, 0, numOfLines - 1);
        iui_textboxCursorLine = std::min(iui_textboxCursorLine, (int)textVec.size() - 1);

        iui_textboxCursorPos = getCurText().size();  // default value
        int _relX = GetMouseX() - x - _bT - _tP;
        for(int i = 0; i < getCurText().size() - iui_textboxShowPos; i++) {
            int _tmpWid1 = iui_measureText(getCurText().substr(iui_textboxShowPos, i));
            int _tmpWid2 = iui_measureText(getCurText().substr(iui_textboxShowPos, i+1));
            int _averWid = (_tmpWid1 + _tmpWid2) / 2;
            if(_relX < _averWid) {
                iui_textboxCursorPos = iui_textboxShowPos + i;
                break;
            }
        }
        iui_textboxPreferredX = -1;
    } else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && iui_kbFocusItem == _ID) {
        iui_kbFocusItem = -1;
        iui_textboxCursorPos = 0;
        iui_textboxShowPos = 0;
        iui_textboxShowLine = 0;
        iui_textboxCursorLine = 0;
        iui_textboxPreferredX = -1;
    }

    // typing stuff
    bool isActive = iui_activeItem == _ID;
    bool isFocus = iui_kbFocusItem == _ID;
    bool isHot = iui_hotItem == _ID;

    if(isFocus) {
        // key input, cursor moving
        int codepoint = GetCharPressed();
        
        auto IsKeyPressedOrRepeated = [](int key) -> bool {
            if(IsKeyPressed(key))
                return true;

            if(IsKeyReleased(key)) {
                iui_textboxCooldownTimer = 0;
                iui_textboxDelayTimer = 0;
            }

            if(IsKeyDown(key)){
                iui_textboxCooldownTimer++;
                if(iui_textboxCooldownTimer > TEXTBOX_COOLDOWN) {
                    iui_textboxDelayTimer++;
                    if(iui_textboxDelayTimer % TEXTBOX_DELAY == 0)
                        return true;
                }
            }
            return false;
        };

        if(codepoint != 0) {
            getCurText().insert(iui_textboxCursorPos, 1, (char)codepoint);
            iui_textboxCursorPos++;
            iui_textboxPreferredX = -1;
        }
        if(IsKeyPressedOrRepeated(KEY_BACKSPACE)) {
            if(iui_textboxCursorPos > 0) {  // delete one char
                getCurText().erase(iui_textboxCursorPos - 1, 1);
                iui_textboxCursorPos--;
            } else {  // merge with previous line
                if(iui_textboxCursorLine > 0) {
                    int _prevLen = textVec[iui_textboxCursorLine - 1].size();
                    textVec[iui_textboxCursorLine - 1] += getCurText();
                    textVec.erase(textVec.begin() + iui_textboxCursorLine);
                    iui_textboxCursorLine--;
                    iui_textboxCursorPos = _prevLen;
                }
            }
            iui_textboxPreferredX = -1;
        }
        if(IsKeyPressedOrRepeated(KEY_RIGHT)) {
            if(iui_textboxCursorPos < getCurText().size()) {
                iui_textboxCursorPos++;
            } else {
                if(iui_textboxCursorLine < textVec.size() - 1) {
                    iui_textboxCursorLine++;
                    iui_textboxCursorPos = 0;
                }
            }
            iui_textboxPreferredX = -1;
        }
        if(IsKeyPressedOrRepeated(KEY_LEFT)) {
            if(iui_textboxCursorPos > 0) {
                iui_textboxCursorPos--;
            } else {
                if(iui_textboxCursorLine > 0) {
                    iui_textboxCursorLine--;
                    iui_textboxCursorPos = getCurText().size();
                }
            }
            iui_textboxPreferredX = -1;
        }
        if(IsKeyPressedOrRepeated(KEY_DOWN)) {
            if(iui_textboxPreferredX == -1)
                iui_textboxPreferredX = iui_measureText(getCurText().substr(iui_textboxShowPos, iui_textboxCursorPos-iui_textboxShowPos));

            iui_textboxCursorLine = std::min(iui_textboxCursorLine + 1, (int)textVec.size() - 1);
            iui_textboxCursorPos = textVec[iui_textboxCursorLine].size();
            for(int i = 1; i < textVec[iui_textboxCursorLine].size() - iui_textboxShowPos; i++) {
                int _tmpWid = iui_measureText(textVec[iui_textboxCursorLine].substr(iui_textboxShowPos, i));
                if(iui_textboxPreferredX < _tmpWid) {
                    int _prevWid = iui_measureText(textVec[iui_textboxCursorLine].substr(iui_textboxShowPos, i - 1));
                    if(iui_textboxPreferredX - _prevWid < _tmpWid - iui_textboxPreferredX)
                        iui_textboxCursorPos = iui_textboxShowPos + i - 1;
                    else
                        iui_textboxCursorPos = iui_textboxShowPos + i;
                    break;
                }
            }
        }
        if(IsKeyPressedOrRepeated(KEY_UP)) {
            if(iui_textboxPreferredX == -1)
                iui_textboxPreferredX = iui_measureText(getCurText().substr(iui_textboxShowPos, iui_textboxCursorPos-iui_textboxShowPos));

            iui_textboxCursorLine = std::max(iui_textboxCursorLine - 1, 0);
            iui_textboxCursorPos = textVec[iui_textboxCursorLine].size();
            for(int i = 1; i < textVec[iui_textboxCursorLine].size() - iui_textboxShowPos; i++) {
                int _tmpWid = iui_measureText(textVec[iui_textboxCursorLine].substr(iui_textboxShowPos, i));
                if(iui_textboxPreferredX < _tmpWid) {
                    int _prevWid = iui_measureText(textVec[iui_textboxCursorLine].substr(iui_textboxShowPos, i - 1));
                    if(iui_textboxPreferredX - _prevWid < _tmpWid - iui_textboxPreferredX)
                        iui_textboxCursorPos = iui_textboxShowPos + i - 1;
                    else
                        iui_textboxCursorPos = iui_textboxShowPos + i;
                    break;
                }
            }
        }
        if(IsKeyPressed(KEY_ENTER)) {
            // divide text at cursorPos
            textVec.insert(textVec.begin() + iui_textboxCursorLine + 1, getCurText().substr(iui_textboxCursorPos, std::string::npos));
            getCurText().erase(iui_textboxCursorPos, std::string::npos);
            iui_textboxCursorPos = 0;
            iui_textboxCursorLine++;
            iui_textboxPreferredX = -1;
        }
        if(GetMouseWheelMove() > 0) {
            iui_textboxShowLine--;
        }
        if(GetMouseWheelMove() < 0) {
            iui_textboxShowLine++;
        }

        /* showPos correction : based on the for invariances *\
            1. showPos >= 0
            2. width(showPos - 1 ~ end) > maxTextWid
            3. showPos <= cursorPos
            4. width(showPos ~ cursorPos - 1) <= maxTextWid

            important part: invariant 1 dominates 2.
        \*                                                   */
        iui_textboxShowPos = std::clamp(iui_textboxShowPos, 0, iui_textboxCursorPos);  // invariant 1 and 3
        while(iui_textboxShowPos>0 && iui_measureText(getCurText().substr(iui_textboxShowPos-1, std::string::npos)) <= maxTextWid)  // invariant 2
            iui_textboxShowPos--;
        
        while(iui_measureText(getCurText().substr(iui_textboxShowPos, iui_textboxCursorPos-iui_textboxShowPos)) > maxTextWid)  // invariant 4
            iui_textboxShowPos++;

        /* showLine correction : based on the four invariances *\
            1. showLine >= 0
            2. showLine + numOfLines <= textVec.size()
            3. showLine <= cursorLine
            4. cursorLine < showLine + numOfLines

            important part: invariant 1 dominates 2.
            important part 2: 3 and 4 are activated only when an action is taken.
        \*                                                     */
        /*
        iui_textboxShowLine = std::clamp(iui_textboxShowLine, 0, iui_textboxCursorLine);  // invariant 1 and 3
        while(iui_textboxShowLine>0 && iui_textboxShowLine + numOfLines > textVec.size())  // invariant 2
            iui_textboxShowLine--;

        while(iui_textboxCursorLine >= iui_textboxShowLine + numOfLines)  // invariant 4
            iui_textboxShowLine++;
        */
        iui_textboxShowLine = std::max(iui_textboxShowLine, 0);  // invariant 1
        while(iui_textboxShowLine>0 && iui_textboxShowLine + numOfLines > textVec.size())  // invariant 2
            iui_textboxShowLine--;
    }

    int currentShowPos = isFocus ? iui_textboxShowPos : 0;
    // text trimming
    /*
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
    */

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

    int textX = x + (_bT + _tP);
    int textY = y + h/2 - textHei*(numOfLines - 1)/2;

    {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE);
        for(int i = iui_textboxShowLine; i < std::min(iui_textboxShowLine + numOfLines, (int)textVec.size()); i++) {
            if((i == iui_textboxShowLine && i > 0) || (i == iui_textboxShowLine + numOfLines - 1 && i < textVec.size() - 1))
                draw::iui_label(textX, textY + sin(iui_animTimer * 0.1f)*4, textVec[i], Fade(style.colTextBoxText, 0.50f));
            else {
                draw::iui_label(textX, textY, textVec[i], style.colTextBoxText);
            }
            textY += textHei;
        }
    }

    // cursor
    if(isFocus) {
        int cursorX = x + (_bT + _tP) + iui_measureText(getCurText().substr(0, iui_textboxCursorPos - iui_textboxShowPos)) - _cT/2;
        int cursorY = y + h/2 + textHei*(iui_textboxCursorLine - iui_textboxShowLine) - textHei*(numOfLines - 1)/2;
        draw::iui_rect(cursorX, cursorY - textHei/2, _cT, textHei, Fade(style.colButtonLabel, sin(iui_animTimer*0.1f)));
    }

    return;
}
} // namespace element
} // namespace imnotgui