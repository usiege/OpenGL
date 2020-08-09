# OpenGL日常05-着色器

标签（空格分隔）： OpenGL

---

大家好，欢迎来到听风的OpenGL日常。

## 写在前面

[本文代码](https://github.com/usiege/OpenGL_LIB/blob/master/OpenGL/OpenGL-04-Shader)

本篇我们通过OpenGL的数据流动来理解着色器，通过数据流会很清楚的看到OpenGL着色器的工作原理。所以今天要说的内容全部跟GLSL语言有关，当然这样的一篇不可能全部讲完（可能连皮毛都没有），理解着色器是如何传数据的才是我们的重点。

话不多说，一图以蔽之。

![res.png-147.7kB][1]

如图中，上部CPU的部分为顶点数据，其中包含顶点、颜色数据（一般情况下）；下部分橙色为顶点着色器，绿色为片元着色器；`gl_Position`内置变量用于顶点位置设置，`gl_FlagColor`内置变量用于片元颜色设置。

## 着色器程序示例

### 顶点着色器

顶点着色器`shader.vs`：
```
#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

attribute vec2 tex_coord;

varying vec2 transform_to_frag;
out vec4 vertex_color;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertex_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
```

### 片元着色器

片元着色器`shader.frag`
```
#version 330 core
in vec4 vertex_color;

varying vec2 transform_to_frag;
uniform sampler2D color_map;

out vec4 color;

void main()
{
    color = vertex_color;
    gl_FragColor = texture2D(color_map, transform_to_frag);
}
```

## 由属性展开去

### in/out

如其英文所代表的含义，理解起来也很直观，对于每一个着色器而言，从外部获取的数据为`in`属性，传出的数据为`out`属性；
这里有一点，对于顶点着色器中的`out`属性变量，如在片元着色器中有同名的`in`属性变量，则为同一变量，因为这样的属性不可以在全局使用，所以采用这样的方式可以传值。（还有其他方式也可以传值）

### layout (location=0)

![006.png-142.2kB][2]

在上面的顶点着色器中，

```
layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
```
包含有`layout (location=0)`属性的`in`变量，在前面的几节中，我们知道着色器通过`glVertexAttribPointer`分配顶点数据，其中，`location`决定着色器从顶点缓存中获取数据的方式。
```
// 位置属性
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
// 颜色属性
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
glEnableVertexAttribArray(1);
```
![002.png-74.7kB][3]

顶点和颜色数据在内存中的分配如图，`glEnableVertexAttribArray`中的参数标识`location`的数字，表示向着色器中传送的数据在内存中的起始点；每个Vertex在内存中都是固定的大小，包含我们所需要的所有数据，在选取其中的数据时，需要提供步长来指明下一次的初始位置，如图，每个顶点数据有6个字节，位置与颜色数据分别占了3个字节。

### attribute

![003.png-34.5kB][4]

attribute变量是只能在vertex shader中使用的变量。（它不能在fragment shader中声明attribute变量，也不能被fragment shader中使用）

一般用attribute变量来表示一些顶点的数据，如：顶点坐标，法线，纹理坐标，顶点颜色等。

在application中，一般用函数`glBindAttribLocation`来绑定每个attribute变量的位置，然后用函数`glVertexAttribPointer`为每个attribute变量赋值。

### varing

![005.png-32.5kB][5]

varying变量是vertex和fragment shader之间做数据传递用的。一般vertex shader修改varying变量的值，然后fragment shader使用该varying变量的值。因此varying变量在vertex和fragment shader二者之间的声明必须是一致的。所以它是只着色器之间使用，不能被应用程序调用；


### uniform

![004.png-26.6kB][6]

uniform变量是外部程序传递给（vertex和fragment）shader的变量。因此它是application通过函数`glUniform**`函数赋值的。在（vertex和fragment）shader程序内部，uniform变量就像是C语言里面的常量（const ），它不能被shader程序修改。（shader只能用，不能改）

如果uniform变量在vertex和fragment两者之间声明方式完全一样，则它可以在vertex和fragment共享使用。（相当于一个被vertex和fragment shader共享的全局变量）

uniform变量一般用来表示：变换矩阵，材质，光照参数和颜色等信息。


## 向量

关于这一节从网上复制了一下，就不做具体讨论了，需要注意的是“重组”的概念。

和其他编程语言一样，GLSL有数据类型可以来指定变量的种类。GLSL中包含C等其它语言大部分的默认基础数据类型：int、float、double、uint和bool。GLSL也有两种容器类型，它们会在这个教程中使用很多，分别是向量(Vector)和矩阵(Matrix)，今天只针对向量做一些介绍。

GLSL中的向量是一个可以包含有1、2、3或者4个分量的容器，分量的类型可以是前面默认基础类型的任意一个。它们可以是下面的形式（n代表分量的数量）：

|类型|含义|
|:-:|-|
|vecn|包含n个float分量的默认向量|
|bvecn|包含n个bool分量的向量|
|ivecn|包含n个int分量的向量|
|uvecn|包含n个unsigned int分量的向量|
|dvecn|包含n个double分量的向量|

一个向量的分量可以通过vec.x这种方式获取，这里x是指这个向量的第一个分量。你可以分别使用.x、.y、.z和.w来获取它们的第1、2、3、4个分量。GLSL也允许你对颜色使用rgba，或是对纹理坐标使用stpq访问相同的分量。

```
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```
你可以使用上面4个字母任意组合来创建一个和原来向量一样长的（同类型）新向量，只要原来向量有那些分量即可；然而，你不允许在一个vec2向量中去获取.z元素。我们也可以把一个向量作为一个参数传给不同的向量构造函数，以减少需求参数的数量：
```
vec2 vect = vec2(0.5f, 0.7f);
vec4 result = vec4(vect, 0.0f, 0.0f);
vec4 otherResult = vec4(result.xyz, 1.0f);
```



## 写在后面

个人理解着色器更靠近硬件，可以直接使用硬件进行计算以提升效率，但是外部程序并不能用高级语言来对其进行操作，所以着色器程序就建立起来一个这样的桥梁，由外部获取再由着色器计算并最终渲染，GLSL语言提供了外部传入以及传出的“接口”。


  [1]: http://static.zybuluo.com/usiege/ixhuijztk8o82d54bdu2565v/res.png
  [2]: http://static.zybuluo.com/usiege/4n2l4etj089hozpeeyvq13bv/006.png
  [3]: http://static.zybuluo.com/usiege/7ee96kfs1p0lx58kacwel2r7/002.png
  [4]: http://static.zybuluo.com/usiege/1kc8y3nkitd989ov0rhhy1yw/003.png
  [5]: http://static.zybuluo.com/usiege/z380roy8f85vv2t8y6e8gn03/005.png
  [6]: http://static.zybuluo.com/usiege/zhj5xciax08ixmsire1szzfn/004.png