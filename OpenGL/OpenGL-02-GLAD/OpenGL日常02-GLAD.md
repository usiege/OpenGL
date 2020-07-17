# OpenGL日常02-GLAD

标签（空格分隔）： Logic

---

## 写在之前

~~GLEW（OpenGL Extension Wrangler Library）；~~

在编写OpenGL代码时，如果希望静态链接 必须包含该预处理宏
```
#define GLEW_STATIC
```
而如果用GCC编译`glew`和`glfw`动态库，则可以用命令行：
```
-lGLEW -lgfw3 -lGL -lX11 -lpthread -lXrandr -lXi
```

~~SOIL(Simple OpenGL Image Library)，[下载](http://lonesock.net/soil.html);~~

在Demo开始前，我们今天要引一下`glad`库，这个库的作用跟`glew`相同，glad与glew相比，简单说glad是glew的升级版，它是2018.09月发布的，我们之后的例子要基于glad库。

在demo中我们会将glad的源码也包含在内，我们可以看清所有glad集成的功能，其实就是对opengl api的封装，但由于某些函数需要不停的调用，就产生了许多封装好的工具库，像之前的glut，到glew，再到我们将到使用的glad。这些工具包是为了跨平台而出现的，它隐藏了不同平台下调用opengl api的细节。


## 添加`glad`库

将glad两个文件添加到工程中：

![tri01.png-310.1kB][1]

当然对于`glad`引用到的其他库，想办法自己解决这个问题，否则迟早会遇到相同的问题；需要库的到这里去找[你会发现大宝藏的！！！](https://github.com/upointed/LearnOpenGL)

首先引入我们的库文件，配置`header search path`：

![tri02.png-88.1kB][2]

代码中添加：
```
#include <glad/glad.h>
#include <GLFW/glfw3.h>
```
这里我们忘记一项，就是下面的动态库链接，千万别忘了，否则会给你报一堆的某个函数没这个符号云云；

![tri03.png-53.2kB][3]

`Command + B`验证通过！

## 构建我们的窗口

在这里与glfwWindow有关的内容不作详细解释，直接进入opengl，至于`glfwWindowHint`参数中的枚举值表示的意义，可见[https://www.glfw.org/docs/latest/window.html#window_hints](https://www.glfw.org/docs/latest/window.html#window_hints)
在这里需要注意的一点是，如果是苹果系统，需要添加如下一行：
```
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
```

### glfw初始化

```
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
```

### 创建窗口对象

```
GLFWwindow* window = glfwCreateWindow(width, height, "Triangle", NULL, NULL);
if(window == NULL) {
    std::cout << "failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
}
glfwMakeContextCurrent(window);
```
判断是否创建成功，并将上下文最终保存起来；

这个时候我们已经可以创建窗口了，不信你可以运行一下；

### 窗口变化回调

在进行OpenGL渲染之前我们必须定义视口，所以这里要区别于窗口，因为在OpenGL里是根本没有窗口的概念的，在OpenGL里渲染必须要定义视口（如果不定义，你猜会发生什么）；

```
//glViewport(0, 0, width, height);
glad_glViewport(0, 0, width, height);
```
`glfw`可以为窗口设置不同的回调函数，具体我们遇到一个个说明，这里先说一个窗口变化回调：
```
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
```
我们在`main`函数里监控窗口变化，当`glfw`窗口发生变化回调到我们自己写的`framebuffer_size_callback`，在这里我们让视口跟随窗口大小变化；

```
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glad_glViewport(0, 0, width, height);
}
```

## GLAD pointer

接下来这段不是很懂，我之前一直想绕过`glut/glew/glad`之类的库，但是貌似OpenGL内部需要在外部保存指针才能对OpenGL作修改，所以这里这些库是必要的，具体为什么在后面学习过程中再作讨论吧，现在的重点是先打通渲染主道路；

```
// glad: load all OpenGL function pointers
    // ---------------------------------------
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
{
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
}  
```

### Render loop

```
// render loop
    // -----------
while (!glfwWindowShouldClose(window))
{
    // input
    // -----
    process_input(window);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// glfw: terminate, clearing all previously allocated GLFW resources.
// ------------------------------------------------------------------
glfwTerminate();
    
```

接下来到了渲染主循环中，逐个解释：

```
glfwWindowShouldClose
```
每次循环glfw是否退出，如果返回0则直接关闭应用程序:
```
glfwTerminate
```
下面这个函数会在每次循环中检查触发事件，根据平台的某些硬件有关的，具体有哪些回调以后讨论；
```
glfwPollEvents
```
这里要说下双缓冲的概念，自己理解：
![tri04.png-150.8kB][4]

```
glfwSwapBuffers
```
我们的`process_input`是一个主动调用的函数，在每次循环中我们会获取键盘的按键，当按下`ESC`时直接关闭窗口；
```
void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
```

### 渲染一下

最后我们要做一个最原始的渲染动作，也就是迈进OpenGL的第一步，改变一下视口内的颜色，在循环中不停地改变视口颜色：

```
static float count = 1;
if (count == 255) {
    count = 1;
}
glClearColor(1.0f/255*count++, 0.3f, 0.3f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);
```
恭喜你，来到GLAD的世界！！！


  [1]: http://static.zybuluo.com/usiege/3jk7wzjja2jp8wa1ii6uy542/tri01.png
  [2]: http://static.zybuluo.com/usiege/q2aqx19gbuv0sukv3e3mk7yc/tri02.png
  [3]: http://static.zybuluo.com/usiege/ud62qsehkrwu5ofambvqhkza/tri03.png
  [4]: http://static.zybuluo.com/usiege/293cikan952re0mivu3ftkug/tri04.png