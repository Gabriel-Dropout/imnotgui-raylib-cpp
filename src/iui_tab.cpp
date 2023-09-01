#include <string>

#include "imnotgui.hpp"

namespace imnotgui {
namespace element {
int iui_tab(int x, int y, int w, int h, const std::vector<std::string> &textVec, int &tabIdx, int trimMode) {
    IuiStyle &style = iuiGlobalStyle;

    int numTabs = textVec.size();
    
    //array
    std::vector<int> IDs;
    std::vector<std::string> labels;

    /// ID for each tabs
    int _ID;
    std::string _label;
    for(int i = 0; i < numTabs; i++) {
        iui_get_all(textVec[i], _ID, _label);
        IDs.push_back(_ID);
        labels.push_back(_label);
    }

    /// Button logic for each tabs

    bool isHot, isCurrent;
    int tabBoxX = x, tabBoxY = y, tabBoxW = w, tabBoxH = h;
    for(int i = 0; i < numTabs; i++) {
        isCurrent = false;
        isHot = false;

        int tabID = IDs[i];
        std::string tabLabel = labels[i];

        tabBoxY = y;
        tabBoxW = w;
        tabBoxH = h;
        
        // Tab label
        int tabLabelWidth = iui_measureText(tabLabel);
        if(tabLabelWidth > w) {
            if (tabIdx != i) {
                switch(trimMode) {
                    case IUI_TAB_TRIM: // TRIM
                        tabLabel = iui_strTrimDots(tabLabel, w);
                        break;
                    case IUI_TAB_FLEX:
                        tabBoxW = tabLabelWidth + 20;
                        break;
                }
            } else {
                tabBoxW = tabLabelWidth + 20;
            }
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

int iui_tab_v(int x, int y, int w, int h, const std::vector<std::string> &textVec, int &tabIdx, int trimMode) {
    IuiStyle &style = iuiGlobalStyle;

    int numTabs = textVec.size();
    
    //array
    std::vector<int> IDs;
    std::vector<std::string> labels;

    /// ID for each tabs
    int _ID;
    std::string _label;
    for(int i = 0; i < numTabs; i++) {
        iui_get_all(textVec[i], _ID, _label);
        IDs.push_back(_ID);
        labels.push_back(_label);
    }

    /// Button logic for each tabs

    bool isHot, isCurrent;
    int tabBoxX = x, tabBoxY = y, tabBoxW = w, tabBoxH = h;
    for(int i = 0; i < numTabs; i++) {
        isCurrent = false;
        isHot = false;

        int tabID = IDs[i];
        std::string tabLabel = labels[i];

        tabBoxX = x;
        tabBoxW = w;
        tabBoxH = h;
        
        // Tab label
        int tabLabelWidth = iui_measureText(tabLabel);
        if(tabLabelWidth > w) {
            if (tabIdx != i) {
                switch(trimMode) {
                    case IUI_TAB_TRIM: // TRIM
                        tabLabel = iui_strTrimDots(tabLabel, w);
                        break;
                    case IUI_TAB_FLEX:
                        tabBoxW = tabLabelWidth + 20;
                        // tabBoxX = x + w - tabBoxW;
                        break;
                }
            } else {
                tabBoxW = tabLabelWidth + 20;
            }
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
        //int hprev, yprev;
        //iui_setAlignment(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE, hprev, yprev);
        {
            ScopedAlignmentSetter sas(IUI_LABEL_ALIGN_MIDDLE, IUI_LABEL_ALIGN_MIDDLE);
            draw::iui_label(tabBoxX + tabBoxW/2, tabBoxY + tabBoxH/2, tabLabel, style.colTabLabel);
        }
        //draw::iui_label(tabBoxX + tabBoxW/2, tabBoxY + tabBoxH/2, tabLabel, style.colTabLabel);
        //iui_setAlignment(hprev, yprev);

        // for next tab
        tabBoxY += tabBoxH;
    }
    return tabIdx;
}
} // namespace element
} // namespace imnotgui