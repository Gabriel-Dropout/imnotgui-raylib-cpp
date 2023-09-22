#include <string>
#include <iostream>

#include "imnotgui.hpp"

namespace imnotgui {
namespace layout {
std::pair<Rectangle, Rectangle> split_h(Rectangle rect, float ratio, int offset) {
    float _tmp = ratio*rect.width + offset;
    return std::make_pair(
        Rectangle{rect.x, rect.y, _tmp, rect.height},
        Rectangle{rect.x + _tmp, rect.y, rect.width - _tmp, rect.height}
    );
}
std::pair<Rectangle, Rectangle> split_v(Rectangle rect, float ratio, int offset) {
    float _tmp = ratio*rect.height + offset;
    return std::make_pair(
        Rectangle{rect.x, rect.y, rect.width, _tmp},
        Rectangle{rect.x, rect.y + _tmp, rect.width, rect.height - _tmp}
    );
}
Rectangle pad(Rectangle rect, int offset) {
    return pad(rect, offset, offset, offset, offset);
}
Rectangle pad(Rectangle rect, int offsety, int offsetx) {
    return pad(rect, offsety, offsetx, offsety, offsetx);
}
Rectangle pad(Rectangle rect, int offsett, int offsetr, int offsetb, int offsetl) {
    if(rect.width < offsetr + offsetl) {
        rect.x += rect.width*offsetl/(offsetl + offsetr);
        rect.width = 0;
        offsetr = offsetl = 0;
    }
    if(rect.height < offsett + offsetb) {
        rect.y += rect.height*offsett/(offsett + offsetb);
        rect.height = 0;
        offsett = offsetb = 0;
    }
    return Rectangle{rect.x + offsetl, rect.y + offsett, rect.width - offsetl - offsetr, rect.height - offsett - offsetb};
}
Vector2 pos_of(Rectangle rect, float ratiox, float ratioy, int offsetx, int offsety) {
    return {rect.x + ratiox*rect.width + offsetx, rect.y + ratioy*rect.height + offsety};
}
Vector2 pos_of(Rectangle rect, Vector2 ratiov, Vector2 offsetv) {
    return pos_of(rect, ratiov.x, ratiov.y, offsetv.x, offsetv.y);
}
} // namespace element
} // namespace imnotgui