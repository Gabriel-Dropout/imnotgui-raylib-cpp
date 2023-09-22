#include <string>
#include <vector>
#include <algorithm>  // std::sort
#include <numeric>    // std::accumulate

#include "imnotgui.hpp"
#include <iostream>

typedef struct TabElement {
    int ID;
    std::string label;
    float width;
};

// if sum of width > maxSum, width = min(width, supremum) is applied to all elements. What should be the supremum?
float calcSupremum(const std::vector<TabElement> &elements, float maxSum) {
    if(elements.size() == 0) return 0;

    std::vector<float> widths;
    for(auto &e : elements) {widths.push_back(e.width);}
    std::sort(widths.rbegin(), widths.rend());
    widths.push_back(0);

    float sum = std::accumulate(widths.begin(), widths.end(), 0.0f);
    int supIdx = 0;

    while(sum > maxSum) {
        float delta = (widths[supIdx] - widths[supIdx + 1])*(supIdx + 1);
        sum -= delta;
        supIdx++;
    }
    float supremum = widths[supIdx] + (supIdx==0 ? 0 : (maxSum - sum)/supIdx);

    return supremum;
}

namespace imnotgui {
namespace element {
int iui_tab_h(Rectangle rect, int minElemW, int maxElemW, const std::vector<std::string> &textVec, int &tabIdx) {
    return iui_tab_h(rect.x, rect.y, rect.width, rect.height, minElemW, maxElemW, textVec, tabIdx);
}
int iui_tab_v(Rectangle rect, int elemH, const std::vector<std::string> &textVec, int &tabIdx) {
    return iui_tab_v(rect.x, rect.y, rect.width, rect.height, elemH, textVec, tabIdx);
}
int iui_tab_h(int x, int y, int w, int h, int minElemW, int maxElemW, const std::vector<std::string> &textVec, int &tabIdx) {
    IuiStyle &style = iuiGlobalStyle;

    int numTabs = textVec.size();
    if(numTabs == 0) return -1;

    /// construct elements
    std::vector<TabElement> elements;
    std::vector<float> widths;
    int _ID;
    std::string _label;
    for(int i = 0; i < numTabs; i++) {
        iui_get_all(textVec[i], _ID, _label);
        float _width = (float)std::clamp(iui_measureText(_label) + 20, minElemW, maxElemW);
        elements.push_back(TabElement{_ID, _label, _width});
        widths.push_back(_width);
    }

    /// calculate supremum
    float supremum = calcSupremum(elements, w);

    /// Button logic for each tabs
    bool isHot, isCurrent;
    int tabBoxX = x, tabBoxY, tabBoxW, tabBoxH;
    for(int i = 0; i < numTabs; i++) {
        isCurrent = false;
        isHot = false;
        TabElement &_elem = elements[i];
        int tabID = _elem.ID;
        std::string tabLabel = _elem.label;

        tabBoxY = y;
        tabBoxW = std::min(_elem.width, supremum);
        tabBoxH = h;
        
        // trim label
        if(iui_measureText(tabLabel) > tabBoxW) {
            tabLabel = iui_strTrimDots(tabLabel, tabBoxW);
        }

        // is hover
        isHot = makeHoverable(tabID, tabBoxX, tabBoxY, tabBoxW, tabBoxH);
        makeActivable(tabID);
        if(makePressable(tabID)) {
            tabIdx = i;
        }
        isCurrent = tabIdx == i;

        /// Button draw
        int colIdx = (i % IuiStyle::colTabNum);
        Color colBackdrop = style.colTab[colIdx];
        Color colAccent = style.colTabAccent[colIdx];
        
        if(isCurrent) {
            colBackdrop = style.colTabCurrent;
            colAccent = style.colTabCurrentAccent;

            tabBoxY -= style.tabAccentHei;
            tabBoxH += style.tabAccentHei;
        } else if(isHot) {
            colBackdrop = style.colTabHot;
            colAccent = style.colTabHotAccent;
        }

        draw::iui_rect(tabBoxX, tabBoxY, tabBoxW, tabBoxH, colBackdrop);
        draw::iui_rect(tabBoxX, tabBoxY, tabBoxW, style.tabAccentHei, colAccent);

        // label
        int hprev, yprev;
        iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
        draw::iui_label(tabBoxX + tabBoxW/2, tabBoxY + tabBoxH/2, tabLabel, style.colTabLabel);
        iui_setAlignment(hprev, yprev);

        // for next tab
        tabBoxX += tabBoxW;
    }
    return tabIdx;
}

int iui_tab_v(int x, int y, int w, int h, int elemH, const std::vector<std::string> &textVec, int &tabIdx) {
    IuiStyle &style = iuiGlobalStyle;

    int numTabs = textVec.size();
    if(numTabs == 0) return -1;
    
    /// construct elements
    std::vector<TabElement> elements;
    int _ID;
    std::string _label;
    for(int i = 0; i < numTabs; i++) {
        iui_get_all(textVec[i], _ID, _label);
        elements.push_back(TabElement{_ID, _label, (float)w});
    }

    /// calculate elemH
    elemH = std::min(elemH, h / numTabs);

    /// Button logic for each tabs
    bool isHot, isCurrent;
    int tabBoxX, tabBoxY = y, tabBoxW, tabBoxH;
    for(int i = 0; i < numTabs; i++) {
        isCurrent = false;
        isHot = false;
        TabElement &_elem = elements[i];
        int tabID = _elem.ID;
        std::string tabLabel = _elem.label;

        tabBoxX = x;
        tabBoxW = _elem.width;
        tabBoxH = elemH;
        
        // trim label
        if(iui_measureText(tabLabel) > tabBoxW) {
            tabLabel = iui_strTrimDots(tabLabel, tabBoxW);
        }
        
        // is hover
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)tabBoxX, (float)tabBoxY, (float)tabBoxW, (float)tabBoxH})) {
            iui_hotItem = tabID;
            isHot = true;

            // ... and is clicked
            if(iui_activeItem == -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                iui_activeItem = tabID;
                tabIdx = i;
            }
        }
        isCurrent = tabIdx == i;

        /// Button draw
        // TODO : Make fancy tab style IDK lol
        int colIdx = (i % style.colTabNum);
        Color colBackdrop = style.colTab[colIdx];
        Color colAccent = style.colTabAccent[colIdx];
        
        if(isCurrent) {
            colBackdrop = style.colTabCurrent;
            colAccent = style.colTabCurrentAccent;

            tabBoxX -= style.tabAccentHei;
            tabBoxW += style.tabAccentHei;
        } else if(isHot) {
            colBackdrop = style.colTabHot;
            colAccent = style.colTabHotAccent;
        }

        draw::iui_rect(tabBoxX, tabBoxY, tabBoxW, tabBoxH, colBackdrop);
        draw::iui_rect(tabBoxX, tabBoxY, style.tabAccentHei, tabBoxH, colAccent);

        // label
        {   ScopedAlignmentSetter sas(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE);
            draw::iui_label(tabBoxX + tabBoxW/2, tabBoxY + tabBoxH/2, tabLabel, style.colTabLabel);
        }

        // for next tab
        tabBoxY += tabBoxH;
    }
    return tabIdx;
}
} // namespace element
} // namespace imnotgui