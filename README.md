### [Are you Korean? Check out the Korean version!](README-KOR.md)

![imnotgui logo](https://TandyRum1024.github.io/imnotgui_huge.png)

# imnotgui-raylib-cpp

imnotgui-raylib-cpp is a port of ZIK's ImNotGUI UI framework to the raylib-cpp library.

---

### Features:

* Simple UI system - designed for quick debugging by incorporating everything in the draw loop

* To be added

This code:

![An example](https://TandyRum1024.github.io/imnotgui/example-code.png)

Produces this result:

![A result](https://TandyRum1024.github.io/imnotgui/example-result.png)

---

### Usage:

imnotgui-raylib-cpp provides the following elements:

* Label
* Button
* Tabs
* Slider
* TextBox

Refer to the original imnotgui documentation for usage details of these elements.

---

### How to Use:

1. Include the necessary dependencies (raylib and raylib-cpp) in your project. You can simply running these commands in its root directory:

   **maxOS & Linux**

   ```bash
   $ make setup
   ```

   **Windows**

   ```powershell
   > mingw32-make setup
   ```

2. Build the source code to make a library.

3. place imnotgui.hpp and libimnotgui.a to your projects.

4. enjoy :)

Example:

```cpp
#include "raylib.h"
#include "raylib-cpp.hpp"
#include "imnotgui_raylib.hpp"

int main()
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "ImNotGUI Example");
    raylib::Color background(raylib::Color::White);

    // Initialize ImNotGUI
    imnotgui::Init();

    while (!window.ShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();

        ClearBackground(background);

        // Draw UI elements
        imnotgui::iuiBegin();

        imnotgui::element::label(50, 50, "Hello, ImNotGUI!");

        if (imnotgui::Button(50, 100, 100, 30, "Click Me!"))
        {
            // Button click logic
        }

        // Draw other UI elements...

        imnotgui::iuiEnd();

        EndDrawing();
    }

    return 0;
}
