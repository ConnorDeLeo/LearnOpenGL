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

# Hello Window

## Initializing and Killing GLFW
To initialize GLFW, a single function is used: `glfwInit()`
To kill GLFW, a single function is also used: `glfwTerminate()`

## Configuring a Window
In order to configure a window, the `glfwWindowHint()` function is used, with the arguments of `OPTION` and `VALUE`. The available arguments and values are found at [GLFW's window handling docs](http://www.glfw.org/docs/latest/window.html#window_hints). Here are the options and values used for this step of the project:
- `GLFW_CONTEXT_VERSION_MAJOR`: the major version for GLFW currently being used (`3` for this build)
- `GLFW_CONTEXT_VERSION_MINOR`: the minor version for GLFW currently being used (`3` for this build)
- `GLFW_OPENGL_PROFILE`: the profile (core [`GLFW_OPENGL_CORE_PROFILE`], compatibility [`GLFW_OPENGL_COMPAT_PROFILE`], or any [`GLFW_OPENGL_ANY_PROFILE`]) being used (`GLFW_OPENGL_CORE_PROFILE` for this build)

## Creating a Window Object
A specific object holds all "windowing" data and is referenced by other functions. It has the type `GLFWwindow` (typically a pointer) and is set by `glfwCreateWindow()` with the arguments `width`, `height`, `title`, `monitor`, `share`. Here are the options and values:
- `width`: width of the window in px (`800` for this build)
- `height`: height of the window in px (`600` for this build)
- `title`: name/title of the window (`"LearnOpenGL"` for this build)
- `monitor`: monitor to auto-fullscreen to (`nullptr` for this build, stays in windowed mode)
- `share`: window to share resources with (`nullptr` for this build, doesn't share resources)

## Calling Contexts
To give the window object to a thread, call `glfwMakeContextCurrent()` where the argument is a `GLFWwindow`

## Initializing GLAD
If GLAD can't load the address of OpenGL function pointers, we kill the program. To check address loading we cast `glfwGetProcAddress` to a `GLADloadproc`, then send it to `gladLoadGLLoader()`

## Creating Viewports
A viewport can be created using `glViewport()` with the arguments `x`, `y`, `width`, and `height`. We use a function provided by glfw (`glfwSetFramebufferSizeCallback()`) with the arguments `GLFWwindow` and `function`. Our function uses this to take in and dynamically change width and height alongside the window size. Here are the options and values:
- `x`: x-coordinate of viewport (`0` for this build)
- `y`: y-coordinate of viewport (`0` for this build)
- `width`: width in px of viewport (pass in `width` var from our window size for this build)
- `height`: height in px of viewport (pass in `height` var from our window size for this build)
- `window`: the window referenced by `glfwSetFramebufferSizeCallback()` (pass in `window` var from our inits for this build)
- `function`: a custom function that calls `glViewport()` (`framebuffer_size_callback()` for this build)

## Render Loops
### Ending a Loop
Creating a loop for rendering is simple in this context, we specifically use a `while()` loop, with the flag being `glfwWindowShouldClose()` with the argument `GLFWwindow`. This waits until the window is "instructed" to close (varies by system).

### Event Triggers
The `glfwPollEvents()` function (with no arguments), checks for any events that may occur (such as keyboard/mouse inputs) and updates window states (calling callbacks)

### Updating Graphics
The `glfwSwapBuffers()` function (with argument `GLFWwindow`) swaps the color buffers (a 2D buffer containing color values for each pixel in the window). Two buffers are rendered, the **front** buffer is used as an output image, while the **back** buffer has render commands called to it. This helps prevent screen tearing, artifacts, and flickering.

## Input Processing
To check inputs, we call a custom function `processInput()` passing in `GLFWwindow` that uses some built in functions from GLFW. `glfwGetKey()` has arguments `GLFWwindow` and `int GLFW_KEY_INPUT`. This outputs one of two variables (`GLFW_RELEASE` for when the button is *not* pressed, and `GLFW_PRESS` for when the key is pressed). For this program we use `GLFW_KEY_ESCAPE` which is assigned to keycode `256` in accordance to the US layout.

## Coloring
Rendering commands go between input processing and polling/graphics updates. Coloring the entire screen consists of setting a clear color with `glClearColor()` with the arguments `r`, `g`, `b`, and `alpha`. Then you clear the color buffer (`GL_COLOR_BUFFER_BIT`) filling it with our set color with the function `glClear()`, passing in `GL_COLOR_BUFFER_BIT`. Here are the arguments and descriptors:
- `r`: the red channel of standard RGB, set to floats (not hexidecimal) from `0.0f` to `1.0f`
- `g`: the green channel of standard RGB, set to floats (not hexidecimal) from `0.0f` to `1.0f`
- `b`: the blue channel of standard RGB, set to floats (not hexidecimal) from `0.0f` to `1.0f`
- `alpha`: the transparency channel of RGB-A, set to floats (not hexidecimal) from `0.0f` to `1.0f`

I've added a small function that randomly increases / decreases RGB values to display constantly updating colors.

# Hello Triangle
Everything in OpenGL is done in 3D space, but must be "flattened" to display on the 2D screen. This is managed by the **Graphics Pipeline** of OpenGL to make 3D coordinates into colored 2D pixels. Each little program running on your graphics card is called a **shader**, these can be modified from their original state by writing a `.GLSL` file (OpenGL Shading Language).

![Render Pipeline](./assets/README/pipeline.png)

In OpenGL we must define at *least* a vertex and a fragment shader since they are not included on GPU firmware

## Vertex Input
Before we can draw anything, we need to define vertex data. This is stored in a **flat array** and not a nested array for GPU efficiency. Coordinates follow the format:
```cpp
float vertices[] = {
    x1, y1, z1,
    x2, y2, z2,//...
    xN, yN, zN
}
```
They are also normalized to fall under `[-1.0f, 1.0f]` and are later transformed into screen coordinates through the viewport.

## Initializing Buffers
The data stored is managed with a **vertex buffer object** or **VBO**. This is an OpenGL object, so it has an index, we can generate it using `glGenBuffers()` with the following arguments:
- `size`: amount of ID's to generate `GLsizei` (`1` for this build since we only have one vertex buffer)
- `buffers`: the buffers you want to generate indeces for in `GLuint*` (`&VBO` for this build [our vertex buffer])

## Binding Buffers
Binding buffers is done in the `glBindBuffer()` function, passing in:
- `target`: the target array to spit out to (`GL_ARRAY_BUFFER` for this build)
- `buffer`: the buffer to pass into the array (`VBO` for this build)

## Copying Data
To copy data to buffers, you call `glBufferData()` and pass in the data you want to send. Here are the arguments and what we used for this build:
- `mode`: specifies the target buffer to send data to (`GL_ARRAY_BUFFER` for this build)
- `size`: specifies the size of data to send in bytes (`sizeof(vertices)` for this build to get however many vertices we have listed)
- `data`: pointer to data to write (`vertices` for this build)
- `usage`: usage mode, with the most common being `GL_STATIC_DRAW`, `GL_DYNAMIC_DRAW`, & `GL_STREAM_DRAW` (`GL_STATIC_DRAW` for this build to draw once and be done)

## Shaders
Similarly programmed to C, we need a vertex shader and a fragment shader.

### Vertex Shaders
For a vertex shader, we need to be able to take in coordinates from a `vec3` (for 3-space coordinates shown in our `vertices[]`) and output a `vec4` (not 4-space, the 4th var in the vector is **perspective division** [covered in later chapters]). We *can* write shaders in our `main.cpp`, but why not load them from a `.GLSL` file? The vertex shader given by [LearnOpenGL](https://learnopengl.com/Getting-started/Hello-Triangle) is stored in `shaders/vertexShaderSource.GLSL`.

### Compiling Shaders
Using the `glCreateShader()` function to pass in the type of shader (i.e. `GL_VERTEX_SHADER`). Then call `glShaderSource()` with the arguments:
- `shader`: the shader to be replaced with the sourcecode (`vertexShader` for our vertex shader)
- `count`: the number of elements in the string array (for having more than one source compiled into one shader, `1` for this build)
- `string`: the source of the shader (`&vectorShaderSource` for our vector shader)
- `length`: an optional argument for specifying length of string elements (`nullptr` for this build)
Then we call `glCompileShader()` passing in the shader we want to compile.

The compilation of the shader is checked with `glGetShaderiv()` pasing in the following parameters:
- `shader`: the shader you want to recieve information from (for example `vertexShader`)
- `pname`: the piece of information you want to recieve from the shader (`GL_COMPILE_STATUS` for this instance)
- `params`: the `int` to return data to (`vertexSuccess` for this build)
Then we use `glGetShaderInfoLog()` passing in our shader, the size of a char array, `nullptr` for the length of our log, and our char array `infoLog`

### Fragment Shaders
Works the same way as vertex shaders, with some different `.GLSL` code. Specifically utilizing the `out` keyword to output a variable in `vec4`.

### Shader Programs
A shader program object is used to combine all our shaders, we can then activate that and issue render calls to it. we call the `glCreateProgram()` function. Then we can use the `glAttachShader()` function, passing in our program and our shader (one at a time). Then we use `glLinkProgram()` passing in our program. To use the program, we call `glUseProgram()` passing in our program, then delete the shader objects with `glDeleteShader()` passing in each shader one at a time. Testing can be done with the same process as shader compilation.

## Linking Vertex Attributes
A vertex shader takes in vertex attributes, but we need to specify where the attributes go and how OpenGL interprets it. Each position is stored as a 32-bit value, with each coordinate stored as 12-bits. These are tightly packed into a buffer array, meaning there is no space between values.
![Vertex Attributes](./assets/README/vertex_attribute_pointer.png)

We can tell OpenGL how to process data with `glVertexAttribPointer()`, there are quite a few arguments:
- `index`: the "layout location" attribute of your vector shader, for example we have `layout(location = 0)` for `aPos` (`location = 1` for color, `location = 2` for texture position, etc.)
- `size`: the amount of components per vertex (a vertex in 3-space has 3 components)
- `type`: the format your data is in (i.e. `float`)
- `normalized`: if your vector data is in int-based coordinates, normalize to floats from `-1.0f` to `1.0f` (`GL_FALSE` for our usecase)
- `stride`: the space *between* consecutive float attributes, each set of positions are 3 floats away from eachother, so you can call `3*sizeof(float)`
- `pointer`: the offset to the first datapoint, this is cast to a void pointer. For our current usecase, we use `0`, however if you are storing color data in the same array as vertex data, then you set your `stride` to `6*sizeof(float)` and the offset of your color `glVertexAttribPointer()` to `3*sizeof(float)`

## Vertex Array Objects
A vertex array object (VAO) consists of all vertexes that get bound to a VBO. Which means whenever we want to draw an object, we call the corrosponding VAO instead of having to point to each coordinate.

![Vertex Array Object](./assets/README/vertex_array_objects.png)

To generate a vertex array, we call `glGenVertexArrays()` passing in the amount of arrays, and a pointer to the array variable:
- `n`: the amount of VAOs to generate
- `arrays`: the array to point at that stores each VAO (a single unsigned integer for now)

They are then bound with `glBindVertexArray()` passing in one array at a time.

## Drawing Vertices
You can draw your VAO using the function `glDrawArrays()` passing in the following arguments:
- `mode`: the mode to draw your arrays in (`GL_TRIANGLES` for this build)
- `first`: specifies the starting index of your array (`0` for this build, since our triangle starts at the first vertex)
- `count`: the amount of vertices to draw (`3` since its a triangle)

## Element Buffer Objects
EBOs can be used to combine multiple objects into one object that shares vertices, like how a rectangle made of two triangles consists of 6 vertices, but only 4 of them are distinct. We create a new unsigned integer array for our indices, generate and bind it the same way as a VAO or VBO, except we use `GL_ELEMENT_ARRAY_BUFFER`. I haven't included code for that here to keep it simpler.