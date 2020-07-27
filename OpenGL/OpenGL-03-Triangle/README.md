# OpenGL日常03-三角形（上）——图形渲染管线

标签（空格分隔）： Logic

---

大家好，欢迎来到听风的OpenGL日常。

## 写在前面

[本文代码](https://github.com/usiege/OpenGL_LIB/blob/master/OpenGL/OpenGL-03-Triangle/main.cpp)

本节我们的理解核心：
```
VBO，Vertex Buffer Object 顶点缓冲对象
VAO，Vertex Array Object 顶点数组对象
EBO，Element Buffer Ojbect 索引缓冲对象
```
理解了这三个概念，就理解了什么叫做图形渲染管线（Graphics Pipeline）。

## 画三角形

本篇我们接02的`glad`配置环境与视口，我想你一定不会忘记窗口与视口的区别。

所以如果你不会如何使用`glfw`去画一个OpenGL窗口，那么请看听风的02-GLAD一节，这里就不给出链接了，自己想办法找。

在上篇中我们给视口区域渲染了底色，如果你做的足够好，那么我们会看到启动的程序背景会不停的发生变化。毫无疑问，画三角形的位置应该与改变视口底色的位置是一样的。

OpenGL会知道在某次渲染循环中改变视口区域的所有颜色值，但是对于一个三角形，OpenGL需要知道在什么位置渲染以及渲染什么样的颜色值。

于是。。。

![pipeline.png-58.2kB][1]

## 这张图讲了什么

对于上面我们提出的问题，答案就在这张图里，我们一点点探索。对于图中写的着色器(Shader)，虽然现在不理解这个是什么意思，但是我们只要知道，这里的着色不仅仅代表颜色就够了，因为这个翻译会影响我们的理解。

上面这张图就是整个OpenGL渲染管线的全过程，你会发现这个过程就是一条流水线而矣，没错它就是如此憨憨的一条流水作业罢了。

### 从顶点数据开始说起

首先一个三角形是由三个顶点组成，`OpenGL`得知道三个顶点的位置，但是由于`OpenGL`是与硬件无关的，它并不知道实际应用中窗口的像素是多少，因此在它自己的理解里，它只知道顶点的概念。于是在它自己的认知内，它得为自己的顶点设定一个范围，这个范围就是标准化设备坐标（Normailzed Device Coordinates），它是一个三维笛卡尔坐标系（百度左转），x,y,z轴的范围都是-1~1。所以如果假想我们的窗口里的物体是在这个范围内，那么我们可以定义一些基础图形：

```
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
//今天的例子是一个2D图形，先忽略z
```
用一张图来理解：

![tri01.png-118.1kB][2]

### 记录位置

顶点有了，`OpenGL`需要把这些位置记录下来，然后发送并告诉GPU，这样GPU才能把正确的位置信息最终输出到屏幕上。

于是第一个概念顶点缓冲对象（VBO）开始工作，

首先生成一个顶点缓冲对象，用它来对这些数据进行管理：
```
unsigned int VBO;
glGenBuffers(1, &VBO);
```
//这里第一个参数表示缓存对象个数，第二个参数是唯一ID用于标识绑定的VBO对象；

`OpenGL`还需要知道绑定的对象是什么类型，这样方便它去管理，这里我们的VBO是一个`GL_ARRAY_BUFFER`；
```
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```
之后我们要把顶点数据存储到VBO对象中，
```
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```
这里会把我们的顶点数据复制到内存里，第一个参数如上个绑定所说，第二个参数是数据大小，第三个是顶点数据，第四个解释如下，作区分的话个人理解也是`OpenGL`便于管理，对于要进行频繁修改的一些缓冲数据，要放置在一些便于访问的位置（臆断）;

- GL_STATIC_DRAW ：数据不会或几乎不会改变。
- GL_DYNAMIC_DRAW：数据会被改变很多。
- GL_STREAM_DRAW ：数据每次绘制时都会改变。

### 顶点着色器

接下来我们有了这些顶点数据，`OpenGL`应该知道这些点应该画在什么地方，因为数据虽然已经到达内存，但是`OpenGL`并不知道，这里就需要用到顶点着色器了，着色器由类似C语言写法的GLSL(OpenGL Shading Language)，直接作用在GPU上，是`OpenGL`与我们写的程序的一个桥梁。这是目前我能理解到的程度。

```
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```
这里我们不对其语法作过多的说明，后续慢慢了解，我们现在看它是如何被连接到我们程序里的。

上面说到VBO对象得将顶点数据发送到`OpenGL`里，那么绑定在VBO的顶点数据就作为顶点着色器的输入直接传入到着色器中，所以`OpenGL`必须先定义一个着色器；

```
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);
```
可以看到这里的着色器类型是`GL_VERTEX_SHADER`，之后着色器是由GLSL编写的，那么：

```
const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
```
着色器程序是由字符串组织的，使用下面的代码绑定到着色器上并编译：
```
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);
```
下面这一段是检查着色器编译是否成功的，从这里可以看到，`OpenGL`的许多功能都是内置的，我们在外部可以通过某些函数对其内部的状态进行窥视，有种偷偷摸摸的感脚，此处省略画面：
```
//检查编译错误
int  success;
char infoLog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}
```
### gl_Position

所以是怎么传到`OpenGL`里的呢，一张图自己体会：
![tri02.png-255kB][3]

这里的`gl_Position`很重要，这个是最终`OpenGL`接收数据的地方，是它的一个内置变量，它的值将会让`OpenGL`知道顶点的位置；

### 片段着色器

接下来我们先跳过开头图中其他的着色器环节，来给三角形设置颜色，这里我们在片段着色器阶段搞一些事情，同前面的顶点着色器类似的过程；

```
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```
这次我们定义的着色器类型是`GL_FRAGMENT_SHADER`：
```
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);
if (!success)
{
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
}
```

## 着色器程序

接下来我们要把最开始的那张图的所有过程链接起来，需要使用`OpenGL`定义一个着色器程序，并把我们写好的两个着色器也链接进去：
```
int shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
```
同样，我们的着色器程序链接过程也可以进行检查，
```
// check for linking errors
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if (!success)
{
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
}
```
之后因为着色器已经链接到了程序中，所以可以对进行删除，这是一个好的习惯：
```
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```

### 如何解析顶点数据

这个部分我觉得需要充分展开说明，涉及到了下面代码段，这个留在下一节中吧，我们就快要完成了，三角形很快要出现。

```
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
glEnableVertexAttribArray(0);
```

## 可以画三角形了

上节说到我们渲染了视口的颜色，前面我们已经写好了渲染程序，所以在改变视口颜色的位置调用着色器程序渲染三角形：
```
glUseProgram(shaderProgram);
glBindVertexArray(VBO); 
glDrawArrays(GL_TRIANGLES, 0, 3);
```
很不幸今年的Demo过程并没有得到最终的结果，那么问题出在什么地方呢？上面说的解析顶点数据到底做了些什么？为什么顶点跟颜色都绑定好了内存却没有在Draw时画出三角形？

下一篇继续讨论[下篇](./README2.md)。

  [1]: http://static.zybuluo.com/usiege/sy9kwcirrf09wmyy6qdun824/pipeline.png
  [2]: http://static.zybuluo.com/usiege/pwrymy6fsz1qwg68ngmhik6d/tri01.png
  [3]: http://static.zybuluo.com/usiege/kpfxgj0s75rpb9l0vgwfe239/tri02.png