[KOREAN VERSION HERE](README-KOR.md)

![imnotgui logo](https://TandyRum1024.github.io/imnotgui_huge.png)

# imnotgui-raylib-cpp

imnotgui-raylib-cpp is a ported version of [ZIK's ImNotGUI UI framework](https://github.com/TandyRum1024/sincerly-imnotgui-gml/tree/master) for use with [raylib-cpp](https://github.com/RobLoach/raylib-cpp)

The repository is also influenced by [Omar Cornut's Dear ImGUI](https://github.com/ocornut/imgui)

---

### Features:

* Simple UI - Drawing functions include control logic! You just need to draw on the screen.
* Stylish design - Personally, I think ImNotGUI's UI design is really cool.
* Active object recognition - When you click a button and move the cursor outside its range, the button remains pressed. Actually, this feature is already implemented in ImNotGUI.

---

### ImNotGUI-raylib-cpp Controls:

There are initially five controls (more will be added!)

* Button
* Tabs
* Slider
* TextBox
* CheckBox

Additionally, it provides convenient drawing functions! (If you want to use ImNotGUI as the backend, you can implement these functions.)

- Label
- Rectangle
- Sprite (with/without atlas)

---

### How to Build:

1. You need to install the required dependency libraries (raylib/raylib-cpp) for building and development. Since they are already registered as submodules, you can run the following command in the root directory:

   **macOS & Linux**

   ```bash
   $ make setup
   ```

   **Windows**

   ```powershell
   > mingw32-make setup
   ```

2. Build the source code to create the library. The examples will be built and executed automatically!

   ```bash
   $ make
   ```

3. Place `bin/libimnotgui.a`, `include/imnotgui.hpp`, and `include/imnotgui_extra.hpp` in your project.

4. Happy coding!

---

Example:

```cpp
#include <string>

#include "raylib/raylib-cpp.hpp"
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

<img src="example1.png" alt="ex1" style="zoom: 80%;" />

<img src="example2.png" alt="ex2" style="zoom: 80%;" />

<img src="example3.png" alt="ex3" style="zoom: 80%;" />

<img src="example4.png" alt="ex4" style="zoom: 80%;" />

<img src="example5.png" alt="ex5" style="zoom: 80%;" />

---

### TODO

- Implement all features of **ImNotGUI**
- Support bitmap fonts
- Support Korean input in text boxes
- Separate Raylib dependencies from the project
- Write usage documentation
