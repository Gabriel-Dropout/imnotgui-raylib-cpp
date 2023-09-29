#include <iostream>
#include <string>
#include <algorithm>

#include "raylib-cpp.hpp"
#include "imnotgui.hpp"
#include "imnotgui_extra.hpp"

using namespace imnotgui;
using namespace imnotgui::draw;
using namespace imnotgui::element;
using namespace imnotgui::layout;

std::string Linebreak(Font font, std::string text, int maxWidth, float fontSize, float spacing);

int main() {
    InitWindow(1280, 720, "DrawTextRegion test");
    std::string text =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque"
        " malesuada purus augue, condimentum iaculis lectus cursus et. Mo"
        "rbi in metus vitae nulla placerat lacinia. In ac vulputate diam."
        " Vivamus et nulla sed turpis imperdiet efficitur. Nullam massa u"
        "rna, luctus ut urna ut, accumsan luctus lorem. Nunc laoreet ligu"
        "la nisi, vitae ultrices urna sodales nec. Quisque sed bibendum n"
        "ibh, et consequat nisi. Nam hendrerit quam varius, hendrerit nun"
        "c et, tempor purus. Sed pretium scelerisque ligula et elementum."
        " Donec euismod metus vitae neque sodales commodo. Aliquam finibu"
        "s lectus vitae lorem eleifend, molestie aliquam massa consectetu"
        "r. Suspendisse volutpat blandit dolor vitae interdum. Ut sit ame"
        "t ultricies nunc. Pellentesque sed velit quam. In pharetra ipsum"
        " dui, ut bibendum massa tempus quis.\n"
        "Praesent iaculis ex ac turpis facilisis, non dignissim velit cur"
        "sus. Etiam aliquam maximus augue, gravida consequat mauris facil"
        "isis in. Nam scelerisque, ex sit amet tempus auctor, libero dui "
        "rhoncus magna, eu mollis lacus urna sed risus. In hac habitasse "
        "platea dictumst. Vivamus viverra viverra ante, quis fermentum ni"
        "sl lacinia ut. Curabitur tincidunt, felis ac porta egestas, nisl"
        " ante sollicitudin eros, in condimentum arcu arcu ac massa. Phas"
        "ellus nec sem et sem tempus commodo id a mi. Fusce imperdiet arc"
        "u eros, sit amet elementum odio bibendum vel. Vivamus a metus au"
        "gue. Donec tincidunt vitae metus non iaculis. Ut pretium mauris "
        "at mauris convallis pharetra. Morbi convallis, lacus tincidunt s"
        "celerisque ultrices, felis eros sollicitudin felis, eget bibendu"
        "m nisi diam eu arcu.\n"
        "Cras maximus pretium interdum. Aliquam dolor elit, mattis quis e"
        "leifend in, fermentum et nulla. Nulla placerat eleifend neque rh"
        "oncus lobortis. Vestibulum dapibus libero ut arcu fringilla impe"
        "rdiet. Class aptent taciti sociosqu ad litora torquent per conub"
        "ia nostra, per inceptos himenaeos. Vestibulum venenatis elit et "
        "metus finibus, non pulvinar ex cursus. Fusce fermentum enim non "
        "nisl molestie, a tincidunt neque ullamcorper. Fusce maximus augu"
        "e vel mi dignissim convallis. Nam vitae aliquet tortor, eu variu"
        "s eros. Maecenas sollicitudin mi ac mi tincidunt cursus. Praesen"
        "t leo eros, congue luctus purus eu, suscipit eleifend nulla. Pro"
        "in congue commodo posuere. Pellentesque pulvinar interdum enim a"
        " suscipit. Pellentesque non est diam. Mauris quis sapien porttit"
        "or, congue dolor ac, faucibus tortor. Cras congue eros ac metus "
        "sollicitudin aliquam.\n"
        "Quisque nec dapibus odio, eget elementum mi. Integer ac pharetra"
        " augue. Suspendisse gravida porttitor porta. Sed fermentum ipsum"
        " vel felis molestie aliquam. Nam sit amet lacus faucibus diam co"
        "nsequat accumsan eget id massa. Sed placerat malesuada orci plac"
        "erat facilisis. Vivamus ultricies, felis vitae rhoncus imperdiet"
        ", arcu urna blandit ex, eu varius felis libero eu erat. Nulla fa"
        "cilisi.\n"
        "Nunc pretium placerat velit, a commodo massa tempus sed. Vestibu"
        "lum et ex condimentum, condimentum nulla ac, tempus lectus. Nam "
        "vehicula risus id cursus finibus. Nam quis sem at quam suscipit "
        "maximus. Maecenas sollicitudin vehicula diam, ut rhoncus nibh po"
        "rta eu. Nulla finibus sit amet arcu eget finibus. Donec enim ero"
        "s, condimentum eu neque quis, laoreet rhoncus ex.";
    text = Linebreak(GetFontDefault(), text, 1280, 20, (int)(20/GetFontDefault().baseSize));

    raylib::Rectangle region(240, 60, 800, 600);
    float padding = 32;
    raylib::Rectangle inner;

    raylib::Vector2 pos_resize_1, pos_resize_2;
    raylib::Rectangle rect_resize_1, rect_resize_2;

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        // Mouse wheel changes padding
        padding += raylib::Mouse::GetWheelMove() * 4;
        padding = std::clamp(padding, 0.0f, std::min(region.width, region.height) / 2);

        // Update region and resize buttons
        inner = pad(region, padding);
        pos_resize_1 = pos_of(region, 0, 0, 5, 5);
        rect_resize_1 = raylib::Rectangle(pos_resize_1.x, pos_resize_1.y, 15, 15);
        pos_resize_2 = pos_of(region, 1, 1, -5, -5);
        rect_resize_2 = raylib::Rectangle(pos_resize_2.x - 15, pos_resize_2.y - 15, 15, 15);

        BeginDrawing();
        ClearBackground(imnotgui::iuDark2);
        iui_begin();

        // Draw text region
        iui_rect(region, iuDark);
        iui_rect(inner, iuBrown);
        int boxID = iui_get_id("box");
        makeHoverable(boxID, region);
        if(makeActivable(boxID)) {
            region.SetPosition(region.GetPosition() + raylib::Mouse::GetDelta());
            region.x = std::clamp(region.x, 0.0f, 1280 - region.width);
            region.y = std::clamp(region.y, 0.0f, 720 - region.height);
        }

        // Resize button - Top left
        int resizeID1 = iui_get_id("resize1");
        Color col_resize_1 = iuCream;
        makeHoverable(resizeID1, rect_resize_1);
        if(makeActivable(resizeID1)) {
            region.SetPosition(region.GetPosition() + raylib::Mouse::GetDelta());
            region.SetSize(region.GetSize() - raylib::Mouse::GetDelta());
            if(region.x < 0) {
                region.width += region.x;
                region.x = 0;
            }
            if(region.y < 0) {
                region.height += region.y;
                region.y = 0;
            }
            if(region.width < 100) {
                region.x += region.width - 100;
                region.width = 100;
            }
            if(region.height < 100) {
                region.y += region.height - 100;
                region.height = 100;
            }
            col_resize_1 = iuRed;
        }
        DrawTriangle(
            {rect_resize_1.x, rect_resize_1.y + rect_resize_1.height},
            {rect_resize_1.x + rect_resize_1.width, rect_resize_1.y},
            {rect_resize_1.x, rect_resize_1.y},
            col_resize_1
        );

        // Resize button - Bottom right
        int resizeID2 = iui_get_id("resize2");
        Color col_resize_2 = iuCream;
        makeHoverable(resizeID2, rect_resize_2);
        if(makeActivable(resizeID2)) {
            region.SetSize(region.GetSize() + raylib::Mouse::GetDelta());
            if(region.x + region.width > 1280) region.width = 1280 - region.x;
            if(region.y + region.height > 720) region.height = 720 - region.y;
            if(region.width < 100) region.width = 100;
            if(region.height < 100) region.height = 100;
            col_resize_2 = iuRed;
        }
        DrawTriangle(
            {rect_resize_2.x + rect_resize_2.width, rect_resize_2.y},
            {rect_resize_2.x, rect_resize_2.y + rect_resize_2.height},
            {rect_resize_2.x + rect_resize_2.width, rect_resize_2.y + rect_resize_2.height},
            col_resize_2
        );

        iui_label_region(0, 0, text, region, padding, iuCream);

        imnotgui::iui_end();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
std::string Linebreak(Font font, std::string text, int maxWidth, float fontSize, float spacing) {
    float textOffsetX = 0.0f;       // Offset X to next character to draw
    float scaleFactor = fontSize/(float)font.baseSize;     // Character rectangle scaling factor

    for (int i = 0; i < text.size(); i++) {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&(text.c_str())[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n') {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width*scaleFactor : font.glyphs[index].advanceX*scaleFactor;
            if (i + 1 < text.size()) glyphWidth = glyphWidth + spacing;
        }

        if (codepoint == '\n') {
            textOffsetX = 0;
        }
        else if ((textOffsetX + glyphWidth) > maxWidth) {
            textOffsetX = 0;
            text.insert(i - (codepointByteCount - 1), 1, '\n');
        }

        if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;  // avoid leading spaces
    }
    return text;
}