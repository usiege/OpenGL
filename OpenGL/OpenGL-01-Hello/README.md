# OpenGL日常01-在Mac上从glfw开始

标签（空格分隔）： Logic

---

之前因为使用红宝书调用了`gl3w`库，今天我们仅使用`glew`和`glfw`配置一个纯`opengl`的开发环境。有想使用`gl3w`的请参看这篇 [glfw/glew/gl3w与Mac的纠葛](https://www.jianshu.com/p/0dba037ec3af)

其实就是省去了添加`gl3w`的部分，测试代码不使用`gl3w`；

## glfw/glew

首先通过`homebrew`安装这两个工具，
```
brew install glew
brew install glfw
```
安装完成后，库文件会在你的Mac系统目录下，
```
/usr/local/Cellar
```
接下来就是将这两个库的目录添加到`Xcode`工具设置里；

点击`Command+.`，在设置中`Locations->Custom Path`中添加，

![gl3w02.png](https://upload-images.jianshu.io/upload_images/1429775-fa1a34e7b3104248.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


```
glew_header
/usr/local/Cellar/glew/2.1.0/include
glew_lib
/usr/local/Cellar/glew/2.1.0/lib
glfw_header
/usr/local/Cellar/glfw/3.3/include
glfw_lib
/usr/local/Cellar/glfw/3.3/lib
```

## 工程中头文件与库查找配置

将文章开头使用`python`下载的`gl3w.c`以及头文件引入工程文件中，并配置头文件搜索目录：

![gl3w03.png](https://upload-images.jianshu.io/upload_images/1429775-0f884424b93f231c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


`.c`文件则直接引入工程中（这里也可以将`.c`编译成动态库或者静态库，我们这里不多作讨论）；

![gl3w04.png](https://upload-images.jianshu.io/upload_images/1429775-28f63387dae2db27.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


接下来配置`glfw`和`glew`搜索，`Header search paths`中添加：
```
$(glew_header)
$(glfw_header)
```
`Library search paths`中添加：
```
$(glew_lib)
$(glfw_lib)
```

## 配置测试

```
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void display(GLFWwindow *window);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    GLFWwindow* window;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowRefreshCallback(window, display);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Render
        display(window);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void display(GLFWwindow *window)
{
    static int count = 1;
    if (count==255) {
        count = 1;
    }
    std::cout << count++ << std::endl;
    
    GLfloat r = 1.0/255, g = 0.5, b = 0.5;
    glClearColor(r*count, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
```

## 这样做的好处

由于opengl红书与蓝书在对opengl讨论的时候，代码都用到了自己编写的库，这样虽然集成了某些功能，但是不便于理解，我们将这些过程拆解，从零开始讨论。

