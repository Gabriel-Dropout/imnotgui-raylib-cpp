#include <iostream>
#include <string>
#include <utility>

#include "raylib-cpp.hpp"
#include "imnotgui.hpp"
#include "imnotgui_extra.hpp"

using namespace imnotgui;
using namespace imnotgui::draw;
using namespace imnotgui::element;

std::string Linebreak(Font font, std::string text, int maxWidth, float fontSize, float spacing);

std::string sampleText =
    u8"'박제가 되어 버린 천재'를 아시오? 나는 유쾌하오. 이런 때 연애까지가 유쾌하오.\n\n"
    "육신이 흐느적흐느적하도록 피로했을 때만 정신이 은화처럼 맑소. 니코틴이 내 횟배 앓는 뱃속으로 스미면 머릿속에 으레 백지가 준비되는 법이오. "
    "그 위에다 나는 위트와 패러독스를 바둑 포석처럼 늘어놓소. 가증할 상식의 병이오.\n\n"
    "나는 또 여인과 생활을 설계하오. 연애기법에마저 서먹서먹해진 지성의 극치를 흘깃 좀 들여다본 일이 있는, 말하자면 일종의 정신분일자말이오. "
    "이런 여인의 반 — 그것은 온갖 것의 반이오. — 만을 영수하는 생활을 설계한다는 말이오. "
    "그런 생활 속에 한 발만 들여놓고 흡사 두 개의 태양처럼 마주 쳐다보면서 낄낄거리는 것이오. "
    "나는 아마 어지간히 인생의 제행이 싱거워서 견딜 수가 없게끔 되고 그만둔 모양이오. 굿바이.\n\n"
    "굿바이. 그대는 이따금 그대가 제일 싫어하는 음식을 탐식하는 아이러니를 실천해 보는 것도 좋을 것 같소. 위트와 패러독스와…….\n\n"
    "그대 자신을 위조하는 것도 할 만한 일이오. 그대의 작품은 한 번도 본 일이 없는 기성품에 의하여 차라리 경편하고 고매하리다.\n\n"
    "19세기는 될 수 있거든 봉쇄하여 버리오. 도스토옙스키 정신이란 자칫하면 낭비일 것 같소. 위고를 불란서의 빵 한 조각이라고는 누가 그랬는지 지언인 듯싶소. "
    "그러나 인생 혹은 그 모형에 있어서 ‘디테일’ 때문에 속는다거나 해서야 되겠소?";

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "ImNotGUI KOR Font");
    raylib::Font fnt_kor_big("resources/font_big.fnt");
    raylib::Font fnt_kor("resources/font_medium.fnt");

    // Onetime Linebreak
    int maxWidth = 1280 - 69*2;
    int fontSize = 24;
    sampleText = Linebreak(fnt_kor, sampleText, maxWidth, fontSize, fontSize/fnt_kor.baseSize);

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(imnotgui::iuDark2);
        iui_begin();


        int SCREEN_CENTER_X = GetScreenWidth()/2;
        int SCREEN_CENTER_Y = GetScreenHeight()/2;

        // backdrop
        iui_rect(42, 0, 1196, 85, imnotgui::iuHellaDark);
        iui_rect(42, 85, 1196, 600, imnotgui::iuDark);
        // iui_rect(69, 85, 1280 - 69*2, 600, imnotgui::iuBrown);
        iui_rect(42, 680, 1196, 40, imnotgui::iuHellaDark);

        // Title
        iui_setFont(&fnt_kor_big);
        iui_setFontSize(30);
        iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_CENTER);
        iui_label(42 + (85-30)/2, 85/2, "아임-낫-구이 한글 폰트 테스트", WHITE);
        
        iui_setFont(&fnt_kor);
        iui_setFontSize(24);
        iui_setAlignment(IUI_LABEL_ALIGN_LEFT, IUI_LABEL_ALIGN_TOP);
        iui_label(42 + (85-30)/2, 85 + (85-30)/2, sampleText, WHITE);
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