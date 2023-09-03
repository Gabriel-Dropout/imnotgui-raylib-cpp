![imnotgui logo](https://TandyRum1024.github.io/imnotgui_huge.png)

# imnotgui-raylib-cpp

imnotgui-raylib-cpp 는 ZIK's ImNotGUI UI 프레임워크를 Raylib-cpp에서 사용할 수 있도록 포팅한 버전입니다.

---

### Features:

* 직접 모드 GUI - 드로잉 함수에 컨트롤 로직이 포함되어 있어요! 코드 자체가 UI 레이아웃을 구성해요.
* 짱 멋진 디자인 - 개인적으로 ImNotGUI의 UI 디자인은 참 멋지다고 생각해요 :)
* 활성화 객체 인식 - 버튼을 클릭한 채로 커서가 범위를 벗어났을 때 버튼이 눌린 상태가 유지돼요! 사실 원래부터 ImNotGUI에도 구현돼 있어요.
* 유한 상태 머신 - ImNotGUI는 OpenGL이나 게임메이커처럼 몇 가지 정보를 상태 머신에 저장해요. UI함수에 지나치게 많은 파라미터를 전달하지 않도록 하기 위해서 이런 디자인을 채택했어요. 현재는 텍스트 정렬 위치, 폰트 종류, 폰트 크기에 대한 상태가 존재해요.

---

### ImNotGUI-raylib-cpp Controls:

기본적으로 여섯 개의 컨트롤이 있어요(더 추가될 거에요!)

* Button
* Tabs
* Slider
* TextBox
* Multiline-textbox
* CheckBox

추가적으로 간편한 드로잉 함수를 제공해요!(만약 다른 백엔드를 사용하고 싶으시다면 이 함수를 구현하시면 돼요)

- iui_label_* : 텍스트를 그리는 함수에요.

- iui_rect_* : 사각형을 그리는 함수에요.

- ~~iui_sprite(with / without atlas) : 아틀라스 스프라이트를 그리기 위해 고유한 데이터 구조를 사용해요. 이것 때문에 `imnotgui_extra.hpp` 에 따로 선언되어 있어요.~~

  라이브러리 사용자에게 특정 아틀라스 구조체를 강요하는 것은 나쁜 행동이에요!

  스프라이트 그리기가 필요하시다면 [rTexLoader](https://github.com/Gabriel-Dropout/rTexLoader)를 참고해주세요 :)

---

### How to Build:

이제 CMake를 지원하게 되었어요! 종속성 라이브러리(raylib / raylib-cpp)를 자동으로 처리해요 :)

아래 예시는 Makefiles 사용자를 기준으로 작성되었습니다.

1. 프로젝트 내에 build 폴더를 생성하고 여기에서 빌드를 진행해요. 예제가 `example` 폴더에 함께 빌드됩니다.

   **maxOS & Linux**

   ```bash
   $ mkdir build && cd build
   $ cmake .. -G "Unix Makefiles"
   $ make
   ```

   **Windows**

   ```powershell
   > mkdir build && cd build
   > cmake .. -G "MinGW Makefiles"
   > mingw32-make
   ```

2. `example/*.exe`가 잘 실행되는지 확인해보세요.

3. `build/src/libimnotgui.a`와 `include/imnotgui.hpp`, `include/imnotgui_extra.hpp`를 여러분의 프로젝트에 가져다 놓으세요.

4. 즐거운 개발 되세요!

---

Example:

```cpp
#include <string>

#include "raylib-cpp.hpp"
#include "imnotgui.hpp"
#include "imnotgui_extra.hpp"


int main() {
    int screenWidth = 800;
    int screenHeight = 450;
    std::string TextboxStr;
    imnotgui::iuiButtonShadow = true;

    raylib::Window w(screenWidth, screenHeight, "ImNotGUI Example");

    SetTargetFPS(60);
    
    while (!w.ShouldClose())
    {
        BeginDrawing();
        imnotgui::iui_begin();
        ClearBackground(imnotgui::iuDark);
        imnotgui::draw::iui_label(50, 50, "ImNotGUI Example", imnotgui::iuCream);
        imnotgui::element::iui_button(50, 100, 100, 50, "A Button");
        imnotgui::element::iui_textbox(50, 175, 400, 50, TextboxStr, "A Textbox");
        imnotgui::iui_end();
        EndDrawing();
    }

    return 0;
}
```

![ex1](example1.png)

---

### TODO

- [x] 비트맵 폰트 지원
- [x] CMake 지원
- [x] raylib-cpp 종속성 제거
- [x] 멀티라인 텍스트박스 구현
- [ ] 텍스트박스 한글 입력 지원
- [ ] 사용법 설명서 작성
