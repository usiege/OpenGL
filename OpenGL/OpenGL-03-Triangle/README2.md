# OpenGL日常04-三角形（下）——VBO/VAO/EBO

标签（空格分隔）： Logic

---

大家好，欢迎来到听风的OpenGL日常。

## 写在前面

[本文代码](https://github.com/usiege/OpenGL_LIB/blob/master/OpenGL/OpenGL-03-Triangle/main.cpp)

上回说到，预期的三角形并没有，并没有，并没有渲染出来，今天我们来补充下，看看问题出在哪里。

## 本文重点

我们先来搞清楚VAO，VBO缓存到底做的是什么工作？

首先是VBO（vertex buffer object），为什么我们要用VBO？
> 不使用VBO时，我们每次绘制（glDrawArrays）图形时都是从本地内存处获取顶点数据然后传输给OpenGL来绘制，这样就会频繁的操作CPU->GPU增大开销，从而降低效率。
使用VBO，我们就能把顶点数据缓存到GPU开辟的一段内存中，然后使用时不必再从本地获取，而是直接从显存中获取，这样就能提升绘制的效率。

在讲清楚这个概念之前，我们还要补充一些概念；

### glBegin/glEnd

以此文的例子解释，我们在传递顶点位置数据的时候，在OpenGL旧版本里，是通过glVertex逐个从CPU传递到GPU的，代码示例如下：

```
glBegin(GL_TRIANGLES);
    glVertex(0.0f, 0.0f);
    glVertex(1.0f, 0.0f);
    glVertex(0.0f, 1.0f);
glEnd();
```
这样每进行一次`glVertex`调用就会向GPU传递一次，由于传输是同步的，所以效率很低；

![000.png-55.7kB][1]

于是：

### DL

Display List（显示列表）的出现可以使CPU在传输的过程中等待数据打包完成，待其结束一次性发送到GPU，代码如：
```
GLuint listName = glGenLists (1);
glNewList (listName, GL_COMPILE);
    glBegin (GL_TRIANGLES);
        glVertex2f (0.0, 0.0);
        glVertex2f (1.0, 0.0);
        glVertex2f (0.0, 1.0);
    glEnd ();
glEndList ();
...
// 绘制（不传输数据）
glCallList(listName);
```

![001.png-42.7kB][2]

显示列表加快了传输效率，但是绘制时是一次性的，那么如果列表中的某单个顶点发生变化时，那么就需要CPU重新生成新的顶点再发送到GPU，GPU收集完成后完成绘制，这样做是极其浪费资源的，当每一帧都有变化时，它就退化成了单个顶点传输的方式。

### VA

Vertex Array，顶点数据要区别于我们开头提到的VAO，它跟缓存是没有关系的，它也是一种传输方案。VA也是通过收集顶点的方式来减少传输次数，但与显示列表不同的是，CPU端将会负责收集所有顶点，收集完成后一次性传输到GPU再进行绘制。

![002.png-44.7kB][3]

这样做导致的结果是，每次进行绘制时，都会进行一次传输，所以绘制速度会低于显示列表。

```
// 每次绘制都将 vertices 传输一次
GLfloat vertices[] = {
    0.0f, 0.0f,
    1.0f, 0.0f, 
    0.0f, 1.0f
}
glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(2,GL_FLOAT,0,vertices);
glDrawArray(GL_TRIANGLES, 0, 3); 
```

### VBO

VBO是结合DL和VA的特点，既方便传输，又要兼顾修改。

由于VA在CPU收集的顶点是一个整体，所以在GPU向渲染流水线提交数据是由一个整体提交的，无法在渲染时做修改；而DL虽然可以单个修改，但是渲染时却需要等待CPU端修改完成等GPU端收集完成再进行。

为了既提高传输效率，又可以使渲染时数据在GPU端也可以修改，VBO应运而生。

![003.png-63.2kB][4]

这样一来VBO保存了一份顶点数据，修改操作可以直接在GPU上进行，修改完成直接绘制。

所以按照这样理解，它的传输与修改是分开的，体现在代码上：

```
//生成VBO，并传输保存到GPU上
GLuint vbo;
glGenBuffer(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
...

// 绘制时直接从VBO中取得顶点数据
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2, (void*)0);
glDrawArray(GL_TRIANGLES, 0, 3);

...
```
但是这里只用VBO进行渲染没有可以参考的代码，本人也是进行了实验但是最终没有画出来，希望有厉害的小伙伴可以一起来交流一下这个问题，这里就留作一个探索。

### VAO

本文最终采用的是VAO结合VBO画出的三角形，上篇我们讨论过了，结果没有出来，今天我们把坑填上。

Vertex Array Object，顶点数据对象是为了简化VBO的流程，当所要传输的VBO有很多的时候，我们需要管理多个VBO，这样对每个VBO都进行记录会比较乱，我们首先想到的管理方式就是用一个数组将它们保存起来，没错，这就是VAO，很直观。

![VAO.png-154.2kB][5]

如图所示，VAO保存了不同VBO的指针，用户可以通过这些指针来对数据进行操作。

本文中，我们先分别生成VAO和VBO，将VBO绑定到VAO中，将VAO绑定到缓存里
```
//VAO
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);

//VBO
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//设置对缓冲区访问的步长为3以及相位为0，告诉着色器，这个数据输入到着色器的第一个(索引为0)输入变量，数据的长度是3个float
GLuint uPos = glGetAttribLocation( shaderProgram, "aPos" );
glVertexAttribPointer(uPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(uPos);

//delete buffer and array
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
```
这里，`glGetAttribLocation( shaderProgram, "aPos" );`一句中，`shaderProgram`就是我们上节中编译好的OpenGL程序，`"aPos"`是顶点着色器代码中的顶点，还记得吗？

![004.png-63.3kB][6]

这里的`location`你可以修改下它的值，看看结果`uPos`的值，会有助于理解它的意义，当然着色器关键字这里先不进行讨论。

```
glVertexAttribPointer(uPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0)；
```
重点讨论一下这个函数：
```
第一个参数：对应着色器代码中的location；
第二个参数：顶点属性的大小，在这里是vec3，所以是3；
第三个参数：数据类型，没什么好说的；
第四个参数：定义是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间；
第五个参数：第五个参数叫做步长(Stride)，还是用图来解释一下吧，比较直观；
第六个参数：表示位置数据在缓冲中起始位置的偏移量(Offset)，当有多个VBO里，可以通过偏移量进行位置锁定；
```

上面的第五个参数中的步长为3，即在VBO里每一个顶点占12个位置，每个位置所占字节由其保存数据类型决定。

![005.png-54.4kB][7]

函数`glVertexAttribPointer`给出了如何从VBO中取得顶点数据的方式，所谓的OpenGL位置学（我又开始胡说八道了）。

接下来，快结束战斗了，画三角形吧。

```
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawArrays( GL_TRIANGLES, 0, 3 );
```
那么我们就愉快的结束了。等下！！！

![res.png-64.8kB][8]

### EBO

对于VAO的这种方式，我们有点小想法，现有一个问题，如果我们要画两个三角形，你觉得最少可以用几个顶点呢？答案肯定是4个。但是如果用前面的方法，恐怕我们需要至少6个来完成，那么EBO，索引缓冲对象(Element Buffer Object，EBO，也叫Index Buffer Object，IBO)的出现就是为了重复利用顶点。

个人觉得索引这个概念特别好，将顶点用索引作标记，当使用顶点时用索引间接访问，如图：

![EBO.png-193.9kB][9]

我们来定义一下数据和索引；

```
float vertices2[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

unsigned int indices[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};
```
建立索引缓冲对象：
```
unsigned int EBO;
glGenBuffers(1, &EBO);
```
接下来同VBO类似，将索引复制到缓冲里，
```
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```
最终进行绘制：
```
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```
数据调用的过程是，绘制时先从EBO从找到索引，再通过VAO中找到对应的VBO中的顶点，`glDrawElements`的参数：

```
第一个：与glDrawArrays一样，设置显示模式；
第二个：总共要绘制的顶点的个数；
第三个：索引的类型；
第四个：指定EBO中的偏移量，类似于VBO中的偏移量；
```
好了，就到这吧，EBO的部分不多做解释了；

![res2.png-60.1kB][10]

## 总结

BE -> DL -> VA -> VBO -> VAO -> EBO；

如果你最终懂得了这个链条的来源，那么恭喜你已经理解了。

本文参考:
- [OpenGL Vertex Buffer Object (VBO)](http://www.songho.ca/opengl/gl_vbo.html)
- [VAO 与 VBO 的前世今生](http://www.photoneray.com/opengl-vao-vbo/)
- [你好，三角形](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/)
- [OpenGL简介-基于VBO绘制三角形](https://blog.csdn.net/jaccen2012/article/details/70307836?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-8.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-8.channel_param)

最后这篇复现没有成功，希望有复现的朋友可以给出点提示。


  [1]: http://static.zybuluo.com/usiege/jyg8o9nxhqdljwa9gq5kmrs3/000.png
  [2]: http://static.zybuluo.com/usiege/h859i9zlowfma14w1knxgqa0/001.png
  [3]: http://static.zybuluo.com/usiege/1v3uv8wvy5puzjyrpx7bb2q5/002.png
  [4]: http://static.zybuluo.com/usiege/erfnj87404rlkjwcuh635uet/003.png
  [5]: http://static.zybuluo.com/usiege/hrolj1ghxzujvucs7xgs3328/VAO.png
  [6]: http://static.zybuluo.com/usiege/ncf9zekcdyga4k1cwtdzuj83/004.png
  [7]: http://static.zybuluo.com/usiege/jly8ytrpo87ei1hq7e31r50h/005.png
  [8]: http://static.zybuluo.com/usiege/nct532qh0wgst18m1gt2ic22/res.png
  [9]: http://static.zybuluo.com/usiege/q3lf03gpsdbsgz25betl9r1s/EBO.png
  [10]: http://static.zybuluo.com/usiege/znpf0nvd2osk04zdw826pnbj/res2.png