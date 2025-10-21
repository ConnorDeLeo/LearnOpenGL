# Creating a Window

## Building GLFW
1. Install the GLFW source code from [their website](https://www.glfw.org/download.html), then unzip and add a `build/` directory
2. Open the CMake GUI (or CLI), and select the root directory of glfw as "source", and the `build/` directory as "build"
3. Click configure and select MinGW as your generator, with default makefiles
4. Click generate to create the makefiles
5. Navigate to your `build/` dir in a MinGW terminal, and run `mingw32-make`
6. Navigate to `build/src/` and copy your `libglfw3.a` file into your libraries path

## Setting up GLAD
1. Navigate to the [GLAD website](https://glad.dav1d.de/)
2. Select C/C++, OpenGL, and a modern version of OpenGL
3. Check the "Generate a Loader" option, and click generate
4. Download and extract the `.zip` the site generates, place the files under `src/` in your `src/` folder, and the subfolders under `include/` into your `include/` folder

## Compiling with G++
`g++ src/main.cpp src/glad.c -o main.exe -Iinclude -Llib -lglfw3 -lopengl32 -lgdi32`