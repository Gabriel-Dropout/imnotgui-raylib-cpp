#pragma once

#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "Vector2.hpp"


namespace myspline {

class Curve{
protected:
    std::vector<raylib::Vector2> points;

public:
    Curve(){}
    virtual raylib::Vector2 getPosition(float x) = 0;
    virtual raylib::Vector2 getTangent(float x) = 0;
    virtual raylib::Vector2 getCurvature(float x) = 0;

    virtual float getMaxT(void) const = 0;
    virtual size_t segmentCount(void) const = 0;
    virtual size_t segmentForT(float t) const = 0;
    virtual float segmentT(size_t segmentIndex) const = 0;
    virtual int getK(void) const = 0;
    virtual float getWeightForN(int n, float t) const = 0;

    virtual size_t getPointCount(void) const { return points.size(); }
    virtual raylib::Vector2 getPoint(size_t index) const { return points[index]; }
    virtual void setPoint(size_t index, raylib::Vector2 point) { points[index] = point; }
    virtual void addPoint(raylib::Vector2 point) { points.push_back(point); }
    virtual void removePoint(size_t index) { points.erase(points.begin() + index); }
    virtual void clearPoints(void) { points.clear(); }
};

class Bezier : public Curve{
private:
    std::vector<raylib::Vector2> positionCache;
    std::vector<raylib::Vector2> tangentCache;
    std::vector<raylib::Vector2> curvatureCache;
public:
    Bezier() {
    }
    Bezier(std::vector<raylib::Vector2> &_points) {
        points = _points;
    }
    raylib::Vector2 getPosition(float x) {
        // size check and retur NaN
        if(points.empty()) return raylib::Vector2(NAN, NAN);

        int pNum = points.size();
        positionCache = points;
        tangentCache.assign(pNum, 0);
        curvatureCache.assign(pNum, 0);
        for(int i=0; i<pNum-1; i++){
            for(int j=0; j<pNum-1-i; j++){
                curvatureCache[j] = curvatureCache[j]*(1-x) + curvatureCache[j+1]*x - tangentCache[j]*2 + tangentCache[j+1]*2;
                tangentCache[j] = tangentCache[j]*(1-x) + tangentCache[j+1]*x - positionCache[j] + positionCache[j+1];
                positionCache[j] = positionCache[j]*(1-x) + positionCache[j+1]*x;
            }
        }
        return positionCache[0];
    }
    raylib::Vector2 getTangent(float x) {
        // size check and retur NaN
        if(points.empty()) return raylib::Vector2(NAN, NAN);

        int pNum = points.size();
        positionCache = points;
        tangentCache.assign(pNum, 0);
        curvatureCache.assign(pNum, 0);
        for(int i=0; i<pNum-1; i++){
            for(int j=0; j<pNum-1-i; j++){
                curvatureCache[j] = curvatureCache[j]*(1-x) + curvatureCache[j+1]*x - tangentCache[j]*2 + tangentCache[j+1]*2;
                tangentCache[j] = tangentCache[j]*(1-x) + tangentCache[j+1]*x - positionCache[j] + positionCache[j+1];
                positionCache[j] = positionCache[j]*(1-x) + positionCache[j+1]*x;
            }
        }
        return tangentCache[0];
    }
    raylib::Vector2 getCurvature(float x) {
        // size check and retur NaN
        if(points.empty()) return raylib::Vector2(NAN, NAN);

        int pNum = points.size();
        positionCache = points;
        tangentCache.assign(pNum, 0);
        curvatureCache.assign(pNum, 0);
        for(int i=0; i<pNum-1; i++){
            for(int j=0; j<pNum-1-i; j++){
                curvatureCache[j] = curvatureCache[j]*(1-x) + curvatureCache[j+1]*x - tangentCache[j]*2 + tangentCache[j+1]*2;
                tangentCache[j] = tangentCache[j]*(1-x) + tangentCache[j+1]*x - positionCache[j] + positionCache[j+1];
                positionCache[j] = positionCache[j]*(1-x) + positionCache[j+1]*x;
            }
        }
        return curvatureCache[0];
    }
    float getMaxT(void) const {return 1;}
    size_t segmentCount(void) const {return 1;}
    size_t segmentForT(float t) const {return 0;}
    float segmentT(size_t segmentIndex) const {return 0;}
    int getK(void) const {return points.size();}
    float getWeightForN(int n, float t) const {
        float res = 1;
        for(int i=0; i<n; i++){
            res *= t;
        }
        for(int i=0; i<points.size()-n-1; i++){
            res *= (1-t);
        }
        return res;
    }
};

template<size_t k, size_t step>
class UniformSpline : public Curve{
protected:
    const float (*matrix)[k] = {};

public:
    UniformSpline(const float _matrix[4][k]):matrix(_matrix) {}

    raylib::Vector2 getPosition(float x) {
        // size check and retur NaN
        if(points.size() < k) return raylib::Vector2(NAN, NAN);

        x = std::clamp(x, .0f, getMaxT());
        int seg = segmentForT(x);
        x -= seg;
        int idx = seg * step;
        raylib::Vector2 v1, v2, v3, v4;
        for(int i=0; i<k; i++){
            v1 += points[idx+i] * matrix[0][i];
            v2 += points[idx+i] * matrix[1][i];
            v3 += points[idx+i] * matrix[2][i];
            v4 += points[idx+i] * matrix[3][i];
        }
        raylib::Vector2 res = v1 + (v2 + (v3 + v4*x)*x)*x;
        return res;
    }
    
    raylib::Vector2 getTangent(float x) {
        // size check and retur NaN
        if(points.size() < k) return raylib::Vector2(NAN, NAN);

        x = std::clamp(x, .0f, getMaxT());
        int seg = segmentForT(x);
        x -= seg;
        int idx = seg * step;
        raylib::Vector2 v1, v2, v3, v4;
        for(int i=0; i<k; i++){
            // v1 += points[idx+i] * matrix[0][i];
            v2 += points[idx+i] * matrix[1][i];
            v3 += points[idx+i] * matrix[2][i];
            v4 += points[idx+i] * matrix[3][i];
        }
        raylib::Vector2 res = v2 + (v3*2 + v4*3*x)*x;
        return res;
    }
    raylib::Vector2 getCurvature(float x) {
        // size check and retur NaN
        if(points.size() < k) return raylib::Vector2(NAN, NAN);

        x = std::clamp(x, .0f, getMaxT());
        int seg = segmentForT(x);
        x -= seg;
        int idx = seg * step;
        raylib::Vector2 v1, v2, v3, v4;
        for(int i=0; i<k; i++){
            // v1 += points[idx+i] * matrix[0][i];
            // v2 += points[idx+i] * matrix[1][i];
            v3 += points[idx+i] * matrix[2][i];
            v4 += points[idx+i] * matrix[3][i];
        }
        raylib::Vector2 res = v3*2 + v4*6*x;
        return res;
    }
    
    float getMaxT(void) const {return points.size()>=k ? (points.size()-k)/step + 1 : 0;}
    size_t segmentCount(void) const {return points.size()>=k ? (points.size()-k)/step + 1 : 0;}
    size_t segmentForT(float t) const {return std::min((size_t)std::clamp(t,.0f,getMaxT()), segmentCount()-1);}
    float segmentT(size_t segmentIndex) const {return (float)segmentIndex;}
    int getK() const {return k;}
    float getWeightForN(int n, float t) const {
        while(t>1) {n -= step; t -= 1;}
        while(t<0) {n += step; t += 1;}
        if(n<0 || n>=k) return 0;
        return matrix[0][n] + (matrix[1][n] + (matrix[2][n] + matrix[3][n]*t)*t)*t;
    }
};

class CubicBezierSpline: public UniformSpline<4, 3> {
private:
    float matrix[4][4] = {
        {1, 0, 0, 0},
        {-3, 3, 0, 0},
        {3, -6, 3, 0},
        {-1, 3, -3, 1},
    };
public:
    CubicBezierSpline(): UniformSpline<4, 3>(matrix){
    }
    CubicBezierSpline(std::vector<raylib::Vector2> &_points): UniformSpline<4, 3>(matrix) {
        points = _points;
    }
};

class CRSpline : public UniformSpline<4, 1> {
private:
    float matrix[4][4] = {
        {0, 1, 0, 0},
        {-.5f, 0, .5f, 0},
        {1, -2.5f, 2, -.5f},
        {-.5f, 1.5f, -1.5f, .5f},
    };
public:
    CRSpline(): UniformSpline<4, 1>(matrix){
    }
    CRSpline(std::vector<raylib::Vector2> &_points): UniformSpline<4, 1>(matrix) {
        points = _points;
    }
};

class HermiteSplineCommon : public UniformSpline<4, 2> {
private:
    float matrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {-3,-2,3,-1},
        {2, 1,-2, 1},
    };
public:
    HermiteSplineCommon(): UniformSpline<4, 2>(matrix){
    }
    HermiteSplineCommon(std::vector<raylib::Vector2> &_points): UniformSpline<4, 2>(matrix) {
        points = _points;
    }
};

class HermiteSpline : public HermiteSplineCommon {
public:
    HermiteSpline(): HermiteSplineCommon(){
    }
    HermiteSpline(std::vector<raylib::Vector2> &_points): HermiteSplineCommon(_points) {
    }
    HermiteSpline(std::vector<raylib::Vector2> &_points, std::vector<raylib::Vector2> &_directions): HermiteSplineCommon() {
        // intersperse tangents and points
        points.resize(_points.size()*2);
        for(int i=0; i<_points.size(); i++){
            points[i*2] = _points[i];
            points[i*2+1] = _directions[i];
        }
    }

    size_t getPointCount() const {return points.size()/2;}
    raylib::Vector2 getPoint(size_t index) const { return HermiteSplineCommon::getPoint(index*2); }
    raylib::Vector2 getDirection(size_t x) const { return HermiteSplineCommon::getPoint(x*2+1); }
    void setPoint(size_t index, raylib::Vector2 point) { HermiteSplineCommon::setPoint(index*2, point); }
    void setDirection(size_t x, raylib::Vector2 p) { HermiteSplineCommon::setPoint(x*2+1, p); }
    void addPoint(raylib::Vector2 point, raylib::Vector2 direction) { points.push_back(point); points.push_back(direction); }
    void removePoint(size_t index) { points.erase(points.begin() + index*2, points.begin() + index*2 + 2); }
    void clearPoints(void) { points.clear(); }
    
    float getWeightForN(int n, float t) const {
        return HermiteSplineCommon::getWeightForN(n*2, t);
    }
    float getWeightForNDir(int n, float t) const {
        return HermiteSplineCommon::getWeightForN(n*2 + 1, t);
    }
};

class B_Spline : public UniformSpline<4, 1> {
private:
    float matrix[4][4] = {
        {1.0f/6.0f, 2.0f/3.0f, 1.0f/6.0f, 0},
        {-0.5f, 0, 0.5f, 0},
        {0.5f, -1, 0.5f, 0},
        {-1.0f/6.0f, 0.5f, -0.5f, 1.0f/6.0f},
    };
public:
    B_Spline(): UniformSpline<4, 1>(matrix){
    }
    B_Spline(std::vector<raylib::Vector2> &_points): UniformSpline<4, 1>(matrix) {
        points = _points;
    }
};

}