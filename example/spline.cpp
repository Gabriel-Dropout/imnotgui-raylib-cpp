#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "imnotgui.hpp"
#include "imnotgui_extra.hpp"

#include "spline.hpp"

#define SP_EPS 1e-5

int screenWidth;
int screenHeight;

raylib::Color ColorLerp(raylib::Color c1, raylib::Color c2, float t) {
    return raylib::Color(
        int(c1.r + (c2.r - c1.r) * t),
        int(c1.g + (c2.g - c1.g) * t),
        int(c1.b + (c2.b - c1.b) * t),
        int(c1.a + (c2.a - c1.a) * t)
    );
}

enum SPLINEIDX {
    SP_BEZIER,
    SP_CB,
    SP_C2,
    SP_HERMITE,
    SP_CARDINAL,
    SP_BSP,
};

int main() {
    
    // Initialization
    screenWidth = 1600;
    screenHeight = 900;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screenWidth, screenHeight, "Raylib C++ Spline Demo");
    
    SetTargetFPS(60);

    myspline::Bezier bezier;
    myspline::CubicBezierSpline bezierSpline;
    myspline::HermiteSpline hermite;
    myspline::CRSpline cardinal;
    myspline::B_Spline bSpline;

    myspline::Curve* curveMap[6] = {&bezier, &bezierSpline, nullptr, &hermite, &cardinal, &bSpline};
    int toggleGroupActive = 0;
    int prec = 60;

    int nearestPointIdx = -1;
    float nearestPointDis = 999999999;
    int curCtl = -1, curCtlR = -1;

    float circleTimer = 0, transitionTimer = 0;
    float rainbowTimer = 0;


    // Main game loop
    while (!w.ShouldClose())
    {
        myspline::Curve* curCurve = curveMap[toggleGroupActive];

        nearestPointIdx = -1;
        nearestPointDis = 999999999;
        if(curCurve != nullptr) {
            raylib::Vector2 mousePos = GetMousePosition();
            float threshold = 20;  // minimum distance to be selected
            
            for(int idx=0; idx<curCurve->getPointCount(); idx++) {
                float _tmpDis = mousePos.Distance(curCurve->getPoint(idx));
                if(_tmpDis < std::min(threshold, nearestPointDis)) {
                    nearestPointDis = _tmpDis;
                    nearestPointIdx = idx;
                }
            }
        }

        // Left Button: Move point
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) curCtl = nearestPointIdx;
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) curCtl = -1;
        if(curCurve != nullptr && curCtl != -1){
            raylib::Vector2 mousePos = GetMousePosition();
            mousePos.x = std::max(mousePos.x, 280.0f);  // must not collapse with UI
            curCurve->setPoint(curCtl, mousePos);
        }

        // Right Button(for hermite): Move tangent
        if(toggleGroupActive == SP_HERMITE) { 
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) curCtlR = nearestPointIdx;
            if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) curCtlR = -1;
            if(curCurve != nullptr && curCtlR != -1){
                raylib::Vector2 mousePos = GetMousePosition();
                mousePos.x = std::max(mousePos.x, 280.0f);  // must not collapse with UI
                hermite.setDirection(curCtlR, mousePos - hermite.getPoint(curCtlR));
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(imnotgui::iuDark);
        imnotgui::iui_begin();
        imnotgui::draw::iui_rect(0, 0, 1600, 85, imnotgui::iuHellaDark);
        //imnotgui::draw::iui_rect(42, 85, 1600-84, 900-40, imnotgui::iuDark);
        imnotgui::draw::iui_rect(0, 900-40, 1600, 40, imnotgui::iuHellaDark);
        
        imnotgui::iui_setFontSize(48);
        imnotgui::iui_setAlignment(imnotgui::IUI_LABEL_ALIGN_LEFT, imnotgui::IUI_LABEL_ALIGN_MIDDLE);
        imnotgui::draw::iui_label_shadow(85/2, 85/2, "Spline Demo", imnotgui::iuCream, 4, 4, imnotgui::iuDark);
        imnotgui::iui_setFontSize(24);

        raylib::Vector2 v_prev;

        if(curCurve != nullptr) {
            // Draw curve
            rainbowTimer += 0.01f;
            if(rainbowTimer > curCurve->getMaxT() + 0.5f) rainbowTimer -= curCurve->getMaxT() + 0.5f;
            v_prev = curCurve->getPosition(0);
            for(double i = 1.0f/prec; i < curCurve->getMaxT()+SP_EPS; i+=1.0f/prec){
                // while nearest not -1, color represents the weight
                // raylib::Color _col = raylib::Color::FromHSV(360.0f*i/curCurve->getMaxT(), 1, 1);
                float _thick = 5;
                raylib::Color _col(160, 160, 160, 255);
                if(nearestPointIdx != -1) {
                    int _segIdx = curCurve->segmentForT(i);
                    float _w = curCurve->getWeightForN(nearestPointIdx, i);
                    if(_w < 0){
                        _col = ColorLerp({120, 120, 120}, {255, 70, 70}, -_w);
                    } else {
                        _col = ColorLerp({120, 120, 120}, {70, 70, 230}, _w);
                    }
                }
                raylib::Vector2 v = curCurve->getPosition(i);
                DrawLineEx(v_prev, v, _thick, _col);
                if(rainbowTimer-0.5f < i && i < rainbowTimer) {
                    _col = raylib::Color::FromHSV(360.0f*i/curCurve->getMaxT(), 1, 1);
                    _thick = 5;
                    DrawLineEx(v_prev, v + (v-v_prev)*0.1f, _thick, _col);
                }
                v_prev = v;
            }
            
            // Draw each point
            for(int i=0; i<curCurve->getPointCount(); i++){
                raylib::Color _col = raylib::Color::FromHSV(360.0f*i/(curCurve->getPointCount()-1), 1, 1);
                DrawCircleV(curCurve->getPoint(i), 7, WHITE);
                DrawCircleV(curCurve->getPoint(i), 5, (i==curCtl)?ORANGE:_col);
            }
            // Draw circle ring
            float _target = (nearestPointIdx != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT));
            transitionTimer += (_target - transitionTimer) * 0.2f;
            if(nearestPointIdx != -1) {
                float _radius = Lerp(20, 25, transitionTimer);
                raylib::Color _dotC(255, 255, 255, Lerp(255, 0, transitionTimer));
                raylib::Color _linC(255, 255, 255, Lerp(0, 255, transitionTimer));
                // circle ring with dotted line
                circleTimer += .8f;
                float _x = curCurve->getPoint(nearestPointIdx).x;
                float _y = curCurve->getPoint(nearestPointIdx).y;
                for(float angle = 0; angle < 360; angle+=45){
                    float _radAngle = (circleTimer + angle) * DEG2RAD;
                    float _radAngle2 = (circleTimer + angle + 25) * DEG2RAD;
                    DrawLineEx({_x+cos(_radAngle)*_radius, _y+sin(_radAngle)*_radius}, {_x+cos(_radAngle2)*_radius, _y+sin(_radAngle2)*_radius}, 2, _dotC);
                }
                DrawRing({_x, _y}, _radius-1, _radius+1, 0, 360, 45, _linC);
            }

            // Draw tangent and curvature
            raylib::Vector2 graphSize(200,200);
            raylib::Vector2 tangentOffset(1450,220);
            raylib::Vector2 curvatureOffset(1450,470);
            float tScale = 0.3f;
            float cScale = 0.1f;
            for(float t = 0; t < curCurve->getMaxT()+SP_EPS; t+=1.0f/prec){
                raylib::Vector2 _t = (curCurve->getTangent(t));
                raylib::Vector2 _c = (curCurve->getCurvature(t));
                tScale = std::min(tScale, graphSize.x/std::max(abs(_t.x), abs(_t.y))*0.5f);
                cScale = std::min(cScale, graphSize.x/std::max(abs(_c.x), abs(_c.y))*0.5f);
            }
            raylib::Vector2 _v;

            imnotgui::draw::iui_groupbox(tangentOffset.x-graphSize.x/2, tangentOffset.y-graphSize.y/2, graphSize.x, graphSize.y, "Tangent");
            Vector2 graphLineV = tangentOffset - raylib::Vector2(graphSize.x/2, 0);
            imnotgui::draw::iui_line(graphLineV.x, graphLineV.y, graphSize.x, 0, 1, imnotgui::iuMint);
            graphLineV = tangentOffset - raylib::Vector2(0, graphSize.y/2);
            imnotgui::draw::iui_line(graphLineV.x, graphLineV.y, graphSize.y, 90, 1, imnotgui::iuMint);
            
            int _segNum = curCurve->segmentCount();
            for(int i=0; i<_segNum; i++){
                float _start = curCurve->segmentT(i);
                v_prev = tangentOffset + curCurve->getTangent(_start+SP_EPS)*tScale;
                for(double i = _start+1.0f/prec; i < _start+1+SP_EPS; i+=1.0f/prec){
                    float _thick = 2;
                    raylib::Color _col(255, 255, 255, 20);
                    raylib::Vector2 v = tangentOffset + curCurve->getTangent(i-SP_EPS)*tScale;
                    DrawLineEx(v_prev, v, _thick, _col);
                    if(rainbowTimer-0.2f < i && i < rainbowTimer) {
                        _col = raylib::Color::FromHSV(360.0f*i/curCurve->getMaxT(), 1, 1);
                        _thick = 2;
                        DrawLineEx(v_prev, v + (v-v_prev)*0.1f, _thick, _col);
                    }
                    v_prev = v;
                }
            }
            _v = tangentOffset + curCurve->getTangent(std::min(rainbowTimer, curCurve->getMaxT()))*tScale;
            DrawCircleV(_v, 2, {220,220,220,255});

            imnotgui::draw::iui_groupbox(curvatureOffset.x-graphSize.x/2, curvatureOffset.y-graphSize.y/2, graphSize.x, graphSize.y, "Curvature");
            graphLineV = curvatureOffset - raylib::Vector2(graphSize.x/2, 0);
            imnotgui::draw::iui_line(graphLineV.x, graphLineV.y, graphSize.x, 0, 1, imnotgui::iuMint);
            graphLineV = curvatureOffset - raylib::Vector2(0, graphSize.y/2);
            imnotgui::draw::iui_line(graphLineV.x, graphLineV.y, graphSize.y, 90, 1, imnotgui::iuMint);
            
            _segNum = curCurve->segmentCount();
            for(int i=0; i<_segNum; i++){
                float _start = curCurve->segmentT(i);
                v_prev = curvatureOffset + curCurve->getCurvature(_start+SP_EPS)*cScale;
                for(double i = _start+1.0f/prec; i < _start+1+SP_EPS; i+=1.0f/prec){
                    float _thick = 2;
                    raylib::Color _col(255, 255, 255, 20);
                    raylib::Vector2 v = curvatureOffset + curCurve->getCurvature(i-SP_EPS)*cScale;
                    DrawLineEx(v_prev, v, _thick, _col);
                    if(rainbowTimer-0.2f < i && i < rainbowTimer) {
                        _col = raylib::Color::FromHSV(360.0f*i/curCurve->getMaxT(), 1, 1);
                        _thick = 2;
                        DrawLineEx(v_prev, v + (v-v_prev)*0.1f, _thick, _col);
                    }
                    v_prev = v;
                }
            }
            _v = curvatureOffset + curCurve->getCurvature(std::min(rainbowTimer, curCurve->getMaxT()))*cScale;
            DrawCircleV(_v, 2, {220,220,220,255});
            
            // Draw weight graph
            float _wWidth = 900.0f;
            if(toggleGroupActive == SP_BEZIER) _wWidth = 300.0f;
            imnotgui::draw::iui_groupbox(360, 70 + 50, _wWidth, 120, "Weight Graph");
            imnotgui::draw::iui_line(360, 70+150, _wWidth, 0, 1, imnotgui::iuMint);

            _segNum = curCurve->segmentCount();
            float _wPerSegment = _wWidth/_segNum;
            
            int wprec = 30;
            float wdelta = 1.0f/wprec;
            for(int i=0; i<curCurve->segmentCount(); i++) {
                int _k = curCurve->getK();
                for(int j=0; j<_k; j++) {
                    raylib::Color _col;
                    if(toggleGroupActive == SP_CB) {
                        _col = raylib::Color::FromHSV(360.0f*(i*3+j)/curCurve->getPointCount(), 1, 1);
                    }else if(toggleGroupActive == SP_HERMITE) {
                        _col = raylib::Color::FromHSV(360.0f*(i+j)/curCurve->getPointCount(), 1, 1);
                    }else{
                        _col = raylib::Color::FromHSV(360.0f*(i+j)/curCurve->getPointCount(), 1, 1);
                    }

                    float w_prev = curCurve->getWeightForN(j, 0);
                    for(float t = wdelta; t < 1+SP_EPS; t+=wdelta) {
                        float _w = curCurve->getWeightForN(j, t);
                        DrawLineEx(
                            {360 + i*_wPerSegment + (t-wdelta)*_wPerSegment, 70 + 150 - w_prev*100}, 
                            {360 + i*_wPerSegment + t*_wPerSegment, 70 + 150 - _w*100},
                            2,
                            _col
                        );
                        w_prev = _w;
                    }
                }
            }
        }
        
        // Draw arrow for hermite
        switch(toggleGroupActive) {
            case SP_HERMITE:
                for(int i=0; i<hermite.getPointCount(); i++){
                    raylib::Vector2 _ori = hermite.getPoint(i);
                    raylib::Vector2 _dst = hermite.getDirection(i);
                    raylib::Vector2 _dir = _dst.Normalize();
                    raylib::Vector2 _ort(_dir.y, -_dir.x);
                    DrawLineV(_ori, _ori + _dst, SKYBLUE);
                    DrawTriangle(_ori + _dst, _ori + _dst - _dir*10 + _ort*6, _ori + _dst - _dir*10 - _ort*6, SKYBLUE);
                }
                break;
        }

        // sidebar
        imnotgui::element::iui_tab_v(30, 120, 240, 64,
            {"Bezier", "Bezier Spline", "Natural Spline", "Hermite Spline", "Cardinal Spline", "B-spline"},
            toggleGroupActive, imnotgui::IUI_TAB_TRIM);
        if(toggleGroupActive == SP_C2) {
            // center, red, big
            std::string _not_available = "Currently Not Available :(";
            int _size = raylib::MeasureText(_not_available, 64);
            raylib::DrawText(_not_available, 800 - _size/2, 450 - 32, 64, raylib::Color(0xe05050ff));
        }
        
        // randomize button
        if (imnotgui::element::iui_button(1450-100, 620, 200, 64, "GENERATE")) {
            if(curCurve != nullptr) {
                curCurve->clearPoints();
                int _num = GetRandomValue(4, 12);
                for(int i=0; i<_num; i++) {
                    if(toggleGroupActive != SP_HERMITE)
                        curCurve->addPoint(raylib::Vector2(GetRandomValue(450, 1300), GetRandomValue(300, 800)));
                    else
                        hermite.addPoint(raylib::Vector2(GetRandomValue(450, 1300), GetRandomValue(200, 800)),
                                           raylib::Vector2(GetRandomValue(-100, 100), GetRandomValue(-100, 100)));
                }
            }
        }
        // untie button
        if (imnotgui::element::iui_button(1450-100, 620 + 64 + 50, 200, 64, "UNTIE")) {
            if(curCurve != nullptr) {
                if(curCurve->getPointCount() >= 2) for(int r=0; r<5; r++) {
                    raylib::Vector2 _p1 = curCurve->getPoint(0);
                    raylib::Vector2 _p2 = curCurve->getPoint(curCurve->getPointCount()-1);

                    // repulse of endpoint
                    raylib::Vector2 _p1tmp;
                    raylib::Vector2 _p2tmp;

                    for(int i=1; i<curCurve->getPointCount()-1; i++) {
                        raylib::Vector2 _p = curCurve->getPoint(i);
                        raylib::Vector2 _dir = (_p1 - _p).Normalize();
                        float _dist = _p.Distance(_p1);
                        _p1tmp += _dir * 70.0f / std::max(_dist, 1.0f);
                        _dir = (_p2 - _p).Normalize();
                        _dist = _p.Distance(_p2);
                        _p2tmp += _dir * 70.0f / std::max(_dist, 1.0f);
                    }
                    // boundary repulsion
                    raylib::Rectangle _bnd = {450, 200, 850, 600};
                    _p1tmp.x += 4000.0f / std::pow(std::max(10.0f, _p1.x - _bnd.x), 2);
                    _p1tmp.x -= 4000.0f / std::pow(std::max(10.0f, _bnd.x + _bnd.width - _p1.x), 2);
                    _p1tmp.y += 4000.0f / std::pow(std::max(10.0f, _p1.y - _bnd.y), 2);
                    _p1tmp.y -= 4000.0f / std::pow(std::max(10.0f, _bnd.y + _bnd.height - _p1.y), 2);
                    
                    _p2tmp.x += 4000.0f / std::pow(std::max(10.0f, _p2.x - _bnd.x), 2);
                    _p2tmp.x -= 4000.0f / std::pow(std::max(10.0f, _bnd.x + _bnd.width - _p2.x), 2);
                    _p2tmp.y += 4000.0f / std::pow(std::max(10.0f, _p2.y - _bnd.y), 2);
                    _p2tmp.y -= 4000.0f / std::pow(std::max(10.0f, _bnd.y + _bnd.height - _p2.y), 2);

                    curCurve->setPoint(0, _p1 + _p1tmp);
                    curCurve->setPoint(curCurve->getPointCount()-1, _p2 + _p2tmp);

                    // attract of inner point
                    for(int i=1; i<curCurve->getPointCount()-1; i++) {
                        raylib::Vector2 _p = curCurve->getPoint(i);
                        raylib::Vector2 _dir = (_p1 - _p).Normalize();
                        float _dist = _p.Distance(_p1);
                        _p += _dir * 3.0f * std::pow(curCurve->getPointCount() - i, 0.5);
                        _dir = (_p2 - _p).Normalize();
                        _dist = _p.Distance(_p2);
                        _p += _dir * 4.0f * std::pow(i, 0.5);
                        curCurve->setPoint(i, _p);
                    }

                    //repulse between every points
                    for(int i=0; i<curCurve->getPointCount(); i++) {
                        raylib::Vector2 _p1 = curCurve->getPoint(i);
                        for(int j=i+1; j<curCurve->getPointCount(); j++) {
                            raylib::Vector2 _p2 = curCurve->getPoint(j);
                            raylib::Vector2 _dir = (_p1 - _p2).Normalize();
                            float _dist = _p1.Distance(_p2);
                            _p1 += _dir * 100.0f / std::pow(std::max(_dist, 10.0f), 1);
                            _p2 -= _dir * 100.0f / std::pow(std::max(_dist, 10.0f), 1);
                            curCurve->setPoint(i, _p1);
                            curCurve->setPoint(j, _p2);
                        }
                    }

                }
            }
        }
        // DrawFPS(10, 10);

        imnotgui::iui_end();
        EndDrawing();
    }

    return 0;
}