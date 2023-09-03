#include <stdlib.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "imnotgui.hpp"

namespace imnotgui {
namespace element {

static std::string getStrLine(const std::string &str, int n);  // get nth line of a string
static int getStrLineNum(const std::string &str);  // get the number of lines in a string

static int linpos2Idx(const std::string &str, int line, int pos);  // get the index of the nth line and mth character
static int idx2Lin(const std::string &str, int idx);
static int idx2Pos(const std::string &str, int idx);
static int coord2Pos(std::string str, int relX);
static int coord2Idx(const std::string &str, int relX, int relY);  // get the index of the character at the position (relX, relY)

static Rectangle clipRect(Rectangle rect1, Rectangle rect2);

static bool isSelectMode();

void iui_multi_textbox(int x, int y, int w, int h, std::string &text, const std::string ID) {

    IuiStyle &style = iuiGlobalStyle;
    // variable names are too long...
    int _bT = style.textboxBorderThickness;
    int _tP = style.textboxTextPadding;
    int _cT = style.textboxCursorThickness;
    int _cP = style.textboxCursorPadding;

    /// Setup
    int textHei = style.labelFontsize * 1.5f;
    int insideWid = w - _bT*2, insideHei = h - _bT*2; // used both for drawing and trimming string.
    int maxTextWid = insideWid - _tP*2;
    int numOfLines = insideHei/textHei;
    Rectangle innerRect = {(float)x + _bT, (float)y + _bT, (float)insideWid, (float)insideHei};
    Rectangle textRect = {(float)x + (_bT + _tP), (float)(int)(y + h/2 - textHei*numOfLines/2), (float)maxTextWid, (float)textHei*numOfLines};

    /// Get ID & vars
    int _ID = iui_get_id(ID);

    /// Textbox logic
    auto getCurText = [&text]() -> std::string {
        return getStrLine(text, idx2Lin(text, iui_textboxCursorIdx));
    };

    // is hover
    makeHoverable(_ID, x, y, w, h);
    makeActivable(_ID);
    if(makePressable(_ID)) {
        iui_kbFocusItem = _ID;

        // Cursor pointing logic
        int _relX = GetMouseX() - x - _bT - _tP + iui_textboxShowOff;
        int _relY = GetMouseY() - (y + h/2 - textHei*numOfLines/2 - textHei*iui_textboxShowLine);
        iui_textboxSelectIdx = coord2Idx(text, _relX, _relY);
        iui_textboxPreferredX = -1;
    } else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && iui_kbFocusItem == _ID) {
        iui_kbFocusItem = -1;
        iui_textboxSelectIdx = 0;
        iui_textboxShowOff = 0;
        iui_textboxCursorIdx = 0;
        iui_textboxShowLine = 0;
        iui_textboxPreferredX = -1;
    }

    // typing stuff
    bool isActive = iui_activeItem == _ID;
    bool isFocus = iui_kbFocusItem == _ID;
    bool isHot = iui_hotItem == _ID;
    bool isChanged = false;

    if(isFocus) {
        // selection
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            int _relX = GetMouseX() - x - _bT - _tP + iui_textboxShowOff;
            int _relY = GetMouseY() - (y + h/2 - textHei*numOfLines/2 - textHei*iui_textboxShowLine);
            iui_textboxCursorIdx = coord2Idx(text, _relX, _relY);
            isChanged = true;
        }

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
            // first delete selected region
            if(isSelectMode()) {
                int _start = std::min(iui_textboxSelectIdx, iui_textboxCursorIdx),
                    _len = std::abs(iui_textboxSelectIdx - iui_textboxCursorIdx);

                text.erase(_start, _len);
                iui_textboxSelectIdx = iui_textboxCursorIdx = _start;
            }

            // insert codepoint and move cursor
            text.insert(text.begin() + iui_textboxSelectIdx, (char)codepoint);
            iui_textboxSelectIdx++;
            iui_textboxCursorIdx = iui_textboxSelectIdx;

            iui_textboxPreferredX = -1;
            isChanged = true;
        }
        if(IsKeyPressedOrRepeated(KEY_BACKSPACE)) {
            if(isSelectMode()) {
                int _start = std::min(iui_textboxSelectIdx, iui_textboxCursorIdx),
                    _len = std::abs(iui_textboxSelectIdx - iui_textboxCursorIdx);

                text.erase(_start, _len);
                iui_textboxSelectIdx = iui_textboxCursorIdx = _start;
            } else {
                if(iui_textboxSelectIdx > 0) {  // delete one char
                    text.erase(iui_textboxSelectIdx - 1, 1);
                    iui_textboxSelectIdx = iui_textboxCursorIdx = iui_textboxSelectIdx - 1;
                }
            }
            iui_textboxPreferredX = -1;
            isChanged = true;
        }
        if(IsKeyPressedOrRepeated(KEY_RIGHT)) {
            if(isSelectMode()) {
                int _end = std::max(iui_textboxSelectIdx, iui_textboxCursorIdx);
                iui_textboxSelectIdx = iui_textboxCursorIdx = _end;
            } else {
                iui_textboxSelectIdx = iui_textboxCursorIdx = std::min(iui_textboxSelectIdx + 1, (int)text.size());
            }
            
            iui_textboxPreferredX = -1;
            isChanged = true;
        }
        if(IsKeyPressedOrRepeated(KEY_LEFT)) {
            if(isSelectMode()) {
                int _end = std::min(iui_textboxSelectIdx, iui_textboxCursorIdx);
                iui_textboxSelectIdx = iui_textboxCursorIdx = _end;
            } else {
                iui_textboxSelectIdx = iui_textboxCursorIdx = std::max(iui_textboxSelectIdx - 1, 0);
            }

            iui_textboxPreferredX = -1;
            isChanged = true;
        }
        if(IsKeyPressedOrRepeated(KEY_DOWN)) {
            int _p1 = idx2Pos(text, iui_textboxSelectIdx),
                _p2 = idx2Pos(text, iui_textboxCursorIdx);
            int _end = std::max(_p1, _p2);

            if(iui_textboxPreferredX == -1)
                iui_textboxPreferredX = iui_measureText(getCurText().substr(0, _end));

            int _line = std::min(idx2Lin(text, iui_textboxSelectIdx) + 1, getStrLineNum(text) - 1);
            int _pos = coord2Pos(getStrLine(text, _line), iui_textboxPreferredX);

            iui_textboxSelectIdx = iui_textboxCursorIdx = linpos2Idx(text, _line, _pos);
            isChanged = true;
        }
        if(IsKeyPressedOrRepeated(KEY_UP)) {
            int _p1 = idx2Pos(text, iui_textboxSelectIdx),
                _p2 = idx2Pos(text, iui_textboxCursorIdx);
            int _start = std::min(_p1, _p2);

            if(iui_textboxPreferredX == -1)
                iui_textboxPreferredX = iui_measureText(getCurText().substr(0, _start));

            int _line = std::max(idx2Lin(text, iui_textboxSelectIdx) - 1, 0);
            int _pos = coord2Pos(getStrLine(text, _line), iui_textboxPreferredX);

            iui_textboxSelectIdx = iui_textboxCursorIdx = linpos2Idx(text, _line, _pos);
            isChanged = true;
        }
        if(IsKeyPressed(KEY_ENTER)) {
            // first delete selected region
            if(isSelectMode()) {
                int _start = std::min(iui_textboxSelectIdx, iui_textboxCursorIdx),
                    _len = std::abs(iui_textboxSelectIdx - iui_textboxCursorIdx);

                text.erase(_start, _len);
                iui_textboxSelectIdx = iui_textboxCursorIdx = _start;
            }

            // insert codepoint and move cursor
            text.insert(text.begin() + iui_textboxSelectIdx, '\n');
            iui_textboxSelectIdx++;
            iui_textboxCursorIdx = iui_textboxSelectIdx;

            iui_textboxPreferredX = -1;
            isChanged = true;
        }
        if(GetMouseWheelMove() > 0) {
            iui_textboxShowLine--;
        }
        if(GetMouseWheelMove() < 0) {
            iui_textboxShowLine++;
        }

        /* showOff correction : based on the two invariances *\
            1. 0 <= showOff <= max(width(~) - maxTextWid, 0)  // text invariance
            2. width(0 ~ cursorPos - 1) - maxTextWid <= showOff <= width(0 ~ cursorPos - 1) + spacing  // cursor invariance

            important part: invariant 1 dominates 2.
        \*                                                   */
        int spacing = std::max(style.labelFontsize/10, 1);
        int _lenTotal = iui_measureText(getCurText());
        int _lenBeforeCursor = iui_measureText(getCurText().substr(0, idx2Pos(text, iui_textboxCursorIdx)));
        iui_textboxShowOff = std::clamp(iui_textboxShowOff, 0, std::max(0, _lenTotal - maxTextWid));  // invariant 1
        iui_textboxShowOff = std::clamp(iui_textboxShowOff, _lenBeforeCursor - maxTextWid, _lenBeforeCursor + spacing);  // invariant 2

        /* showLine correction : based on the four invariances *\
            1. showLine >= 0
            2. showLine + numOfLines <= textVec.size()
            3. showLine <= selectLine
            4. selectLine < showLine + numOfLines

            important part: invariant 1 dominates 2.
            important part 2: 3 and 4 are activated only when an action is taken.
        \*                                                     */
        iui_textboxShowLine = std::max(iui_textboxShowLine, 0);  // invariant 1
        while(iui_textboxShowLine>0 && iui_textboxShowLine + numOfLines > getStrLineNum(text))  // invariant 2
            iui_textboxShowLine--;
        if(isChanged) {
            iui_textboxShowLine = std::min(iui_textboxShowLine, idx2Lin(text, iui_textboxCursorIdx));  // invariant 3
            while(idx2Lin(text, iui_textboxCursorIdx) >= iui_textboxShowLine + numOfLines)  // invariant 4
                iui_textboxShowLine++;
        }
    }

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
    draw::iui_rect(innerRect.x, innerRect.y, innerRect.width, innerRect.height, fillColor);

    // stripe
    Rectangle _stripeRect = {innerRect.x, textRect.y - textHei, innerRect.width, (float)textHei};
    for(int i = 0; i < numOfLines + 2; i++) {
        // cut off the part that is out of the box
        Rectangle _rect = clipRect(innerRect, _stripeRect);
        draw::iui_rect(_rect.x, _rect.y, _rect.width, _rect.height, iui_colLighter(fillColor, (i + iui_textboxShowLine)%2 ? 0 : 5));
        _stripeRect.y += textHei;
    }
    

    // cursor or selection
    if(isFocus) {
        if(!isSelectMode()) {  // cursor
            if(isChanged) iui_cursorTimer = 0;
            int _line = idx2Lin(text, iui_textboxCursorIdx),
                _pos = idx2Pos(text, iui_textboxCursorIdx);

            if(_line >= iui_textboxShowLine && _line < iui_textboxShowLine + numOfLines) {
                int cursorX = x + (_bT + _tP) + iui_measureText(getCurText().substr(iui_textboxShowPos, _pos)) - iui_textboxShowOff - _cT/2;
                int cursorY = y + h/2 + textHei*(_line - iui_textboxShowLine) - textHei*(numOfLines - 1)/2;

                float _alpha = (iui_cursorTimer < 120) ? 1 : cos((iui_cursorTimer - 120)*0.1f);
                draw::iui_rect(cursorX, cursorY - textHei/2, _cT, textHei, Fade(style.colButtonLabel, _alpha));
            }
        } else {  // selection
            int firstLine, lastLine, firstPos, lastPos;
            if(iui_textboxSelectIdx < iui_textboxCursorIdx) {
                firstLine = idx2Lin(text, iui_textboxSelectIdx);
                firstPos = idx2Pos(text, iui_textboxSelectIdx);
                lastLine = idx2Lin(text, iui_textboxCursorIdx);
                lastPos = idx2Pos(text, iui_textboxCursorIdx);
            } else {
                firstLine = idx2Lin(text, iui_textboxCursorIdx);
                firstPos = idx2Pos(text, iui_textboxCursorIdx);
                lastLine = idx2Lin(text, iui_textboxSelectIdx);
                lastPos = idx2Pos(text, iui_textboxSelectIdx);
            }

            for(int i = firstLine; i<=lastLine; i++) {
                if(i < iui_textboxShowLine - 1 || i >= iui_textboxShowLine + numOfLines + 1) continue;

                std::string _text = getStrLine(text, i);
                int _startPos = (i == firstLine) ? firstPos : 0;
                int _endPos = (i == lastLine) ? lastPos : _text.size();
                if(i != lastLine) {  // act as if linebreak has width
                    _text += ' ';
                    _endPos++;
                }

                int _startX = textRect.x + iui_measureText(_text.substr(0, _startPos)) - iui_textboxShowOff;
                int _endX = textRect.x + iui_measureText(_text.substr(0, _endPos)) - iui_textboxShowOff;
                int _startY = textRect.y + textHei*(i - iui_textboxShowLine);

                Rectangle _selectRect = {(float)_startX, (float)_startY, (float)(_endX - _startX), (float)textHei};
                for(int i = 0; i < numOfLines + 2; i++) {
                    // cut off the part that is out of the box
                    Rectangle _rect = clipRect(innerRect, _selectRect);
                    draw::iui_rect(_rect.x, _rect.y, _rect.width, _rect.height, Fade(style.colButtonLabel, 0.5f));
                }
            }
        }
    }

    // text
    int _textX = textRect.x;
    int _textY = (int)textRect.y + textHei/2;

    {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_MIDDLE);
        int _lineNum = getStrLineNum(text);
        for(int i = iui_textboxShowLine; i < std::min(iui_textboxShowLine + numOfLines, _lineNum); i++) {
            std::string _text = getStrLine(text, i);

            // line number
            {   ScopedAlignmentSetter _(IUI_LABEL_ALIGN_RIGHT, IUI_LABEL_ALIGN_MIDDLE);
                draw::iui_label(x - _tP, _textY, std::to_string(i + 1), style.colTextBoxText);
            }

            // text trimming
            int lenToCut = iui_textboxShowOff;
            std::string trimText = _text;
            int spacing = std::max(style.labelFontsize/10, 1);

            while(lenToCut > 0 && trimText.size() > 0) {
                lenToCut -= iui_measureText(std::string(1, trimText[0])) + spacing;
                trimText = trimText.substr(1);
            }
            while(iui_measureText(trimText) > maxTextWid && trimText.size() > 0) {
                trimText = trimText.substr(0, trimText.size() - 1);
            }
            
            // actual text
            if((i == iui_textboxShowLine && i > 0) || (i == iui_textboxShowLine + numOfLines - 1 && i < _lineNum - 1))
                draw::iui_label(_textX - lenToCut, _textY + sin(iui_animTimer * 0.1f)*4, trimText, Fade(style.colTextBoxText, 0.50f));
            else {
                draw::iui_label(_textX - lenToCut, _textY, trimText, style.colTextBoxText);
            }
            _textY += textHei;
        }
    }

    return;
}

// get nth line of a string
static std::string getStrLine(const std::string &str, int n) {
    if(n<0) return "";
    
    int _start = 0;
    for(int i = 0; i < n; i++) {
        _start = str.find('\n', _start);
        if(_start == std::string::npos) return "";
        _start++;
    }
    
    int _end = str.find('\n', _start);
    if(_end == std::string::npos) return str.substr(_start);
    
    return str.substr(_start, _end - _start);
}

// get the number of lines in a string
static int getStrLineNum(const std::string &str) {
    return std::count(str.begin(), str.end(), '\n') + 1;
}

// get the index of the nth line and mth character
static int linpos2Idx(const std::string &str, int line, int pos) {
    int _idx = 0;
    for(int i = 0; i < line; i++) {
        _idx = str.find('\n', _idx);
        if(_idx == std::string::npos) return -1;
        _idx++;
    }
    return _idx + pos;
}

static int idx2Lin(const std::string &str, int idx) {
    int _start = 0, _end = 0, _cnt = 0;
    while(true) {
        // invariance: _start is start of certain line
        // invariance: _tmp is end of certain line

        _end = str.find('\n', _start);
        if(_end == std::string::npos || _end>=idx)
            return _cnt;
        _start = _end + 1;
        _cnt++;
    }
}

static int idx2Pos(const std::string &str, int idx) {
    int _start = 0, _end = 0;
    while(true) {
        // invariance: _start is start of certain line
        // invariance: _tmp is end of certain line

        _end = str.find('\n', _start);
        if(_end == std::string::npos || _end>=idx)
            return idx - _start;
        _start = _end + 1;
    }
}

static int coord2Pos(std::string str, int relX) {
    // if there is a linebreak in str, take only the first line
    if(str.find('\n') != std::string::npos)
        str = str.substr(0, str.find('\n'));

    // find the nearest character to relX and return its position
    int _pos = str.size();
    for(int i = 0; i < str.size(); i++) {
        int _tmpWid1 = iui_measureText(str.substr(0, i));
        int _tmpWid2 = iui_measureText(str.substr(0, i+1));
        int _averWid = (_tmpWid1 + _tmpWid2) / 2;
        if(relX < _averWid) {
            _pos = i;
            break;
        }
    }
    return _pos;
}

// get the index of the character at the position (relX, relY)
static int coord2Idx(const std::string &str, int relX, int relY) {
    int textHei = iuiGlobalStyle.labelFontsize * 1.5f;

    int _line = std::clamp(relY/textHei, 0, getStrLineNum(str) - 1);

    std::string _text = getStrLine(str, _line);
    int _pos = coord2Pos(_text, relX);
    return linpos2Idx(str, _line, _pos);
}

static Rectangle clipRect(Rectangle rect1, Rectangle rect2) {
    Rectangle ret = {0};
    ret.x = std::max(rect1.x, rect2.x);
    ret.y = std::max(rect1.y, rect2.y);
    ret.width = std::min(rect1.x + rect1.width, rect2.x + rect2.width) - ret.x;
    ret.height = std::min(rect1.y + rect1.height, rect2.y + rect2.height) - ret.y;
    return ret;
}

static bool isSelectMode() {
    return iui_textboxCursorIdx != iui_textboxSelectIdx;
}

} // namespace element
} // namespace imnotgui