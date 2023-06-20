![imnotgui logo](https://TandyRum1024.github.io/imnotgui_huge.png)

# imnotgui-raylib-cpp

imnotgui-raylib-cpp 는 ZIK's ImNotGUI UI 프레임워크를 Raylib-cpp에서 사용할 수 있도록 포팅한 버전입니다.

---

### Features:

* 간단한 UI - 드로잉 함수에 컨트롤 로직이 포함되어 있어요! 여러분은 화면에 그리기만 하면 돼요
* 짱 멋진 디자인 - 개인적으로 ImNotGUI의 UI 디자인은 참 멋지다고 생각해요 :)
* 활성화 객체 인식 - 버튼을 클릭한 채로 커서가 범위를 벗어났을 때 버튼이 눌린 상태가 유지돼요! 사실 원래부터 ImNotGUI에도 구현돼 있어요

---

### ImNotGUI-raylib-cpp Controls:

기본적으로 다섯 개의 컨트롤이 있어요(더 추가될 거에요!)

* Button
* Tabs
* Slider
* TextBox
* CheckBox

추가적으로 간편한 드로잉 함수를 제공해요!(만약 백엔드로 ImNotGUI를 사용하고 싶으시다면 이 함수를 구현하시면 돼요)

- Label
- Rectangle
- Sprite(with / without atlas)

---

### How to Build:

1. 빌드 및 개발에 필요한 종속성 라이브러리 (raylib / raylib-cpp)를 설치해야 해요. 이미 서브모듈에 등록되어 있기 때문에 루트 디렉토리에서 아래 명령어를 입력하시면 돼요.

   **maxOS & Linux**

   ```bash
   $ make setup
   ```

   **Windows**

   ```powershell
   > mingw32-make setup
   ```

2. 소스코드를 빌드해서 라이브러리를 만들어요. 예제가 함께 빌드되면서 자동으로 실행돼요!

   ```bash
   $ make
   ```

3. `bin/libimnotgui.a`와 `include/imnotgui.hpp`, `include/imnotgui_extra.hpp`를 여러분의 프로젝트에 가져다 놓으세요.

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

- **ImNotGUI**의 모든 기능 구현
- 비트맵 폰트 지원
- 텍스트박스 한글 입력 지원
- Raylib 종속성 코드를 프로젝트와 분리
- 사용법 설명서 작성
