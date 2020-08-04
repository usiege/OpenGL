# OpenGL日常06(下)-纹理帧图

标签（空格分隔）： OpenGL

---

大家好，欢迎来到听风的OpenGL日常。

## 写在前面

[本文主要代码：https://github.com/usiege/OpenGL_LIB/tree/master/OpenGL/OpenGLES-06-FrameBuffer](https://github.com/usiege/OpenGL_LIB/tree/master/OpenGL/OpenGLES-06-FrameBuffer)

## 纹理绘制

接上篇，在前面着色器程序链接成功后添加纹理坐标并绘制；

添加纹理坐标：
```
//前3个是顶点坐标，后2个是纹理坐标
GLfloat attrArr[] =
{
    0.5f, -0.5f, -1.0f,     1.0f, 0.0f,
    -0.5f, 0.5f, -1.0f,     0.0f, 1.0f,
    -0.5f, -0.5f, -1.0f,    0.0f, 0.0f,
    
    0.5f, 0.5f, -1.0f,      1.0f, 1.0f,
    -0.5f, 0.5f, -1.0f,     0.0f, 1.0f,
    0.5f, -0.5f, -1.0f,     1.0f, 0.0f,
};
```
将数据绑定到顶点缓存并连接到着色器：
```
GLuint attrBuffer;
glGenBuffers(1, &attrBuffer);
glBindBuffer(GL_ARRAY_BUFFER, attrBuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(attrArr), attrArr, GL_DYNAMIC_DRAW);

GLuint position = glGetAttribLocation(program, "position");
glEnableVertexAttribArray(position);
glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, NULL);

GLuint textCoor = glGetAttribLocation(program, "textCoordinate");
glEnableVertexAttribArray(textCoor);
glVertexAttribPointer(textCoor, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (float *)NULL + 3);
```

![003.png-234.1kB][1]

其中，如图箭头，我想这应该很清楚能看的出程序与着色器间的关系了；

接下来，本文重点，加载纹理：

## OpenGLES加载纹理

我们先来准备数据，首先要把渲染的纹理数据写到位图里，这个用iOS的CG框架就可以做到。

### 纹理数据准备

我们要把图片文件读取出来保存成位图绑定到OpenGLES：

```
CGImageRef spriteImage = [UIImage imageName:"texture"].CGImage; 
//这里一般要做检查，我们这里用注释代替

size_t width = CGImageGetWidth(spriteImage);
size_t height = CGImageGetHeight(spriteImage);

//最终将 spriteImage -> spriteData
GLubyte spriteData = (GLubyte *)calloc(width*height*4, sizeof(GLubyte));
CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteData), kCGImageAlphaPremultipliedLast);
CGRect rect = CGRectMake(0, 0, width, height);
CGContextDrawImage(spriteContext, rect, spriteImage);
CGContextRelease();
```
这样纹理的数据就保存在了`spriteData`中，解释下里面的`CGBitmapContextCreate`：
```
CGBitmapContextCreate
data -> 指向要渲染的绘制内存的地址。这个内存块的大小至少是（bytesPerRow*height）个字节
width -> bitmap的宽度,单位为像素
height -> bitmap的高度,单位为像素
bitsPerComponent -> 内存中像素的每个组件的位数.例如，对于32位像素格式和RGB 颜色空间，你应该将这个值设为8.
bytesPerRow -> bitmap的每一行在内存所占的比特数
colorspace -> bitmap上下文使用的颜色空间。
bitmapInfo -> 指定bitmap是否包含alpha通道，像素中alpha通道的相对位置，像素组件是整形还是浮点型等信息的字符串。
```

### 敲黑板画重点啦！！！

我们看一下数据是怎么被绑定到OpenGLES端的；
```
glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

float fw = width, fh = height;
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fw, fh, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);

free(spriteData);
```
首先解释一下前面我们说到，着色器程序分别设置了顶点和纹理坐标，

![007.png-311.1kB][2]

其中，顶点坐标控制OpenGL渲染三角形的位置，而纹理坐标最后被设置到片元着色器的`gl_FragColor`，这里的`gl_FragColor`与`gl_Position`一样，都是OpenGL的内置变量，主要用来设置片元像素的颜色，那上面`textCoord`明明是坐标，为什么会跟颜色有关系呢？

答案就在`texture2D`，在GLSL（着色器语言中），因为纹理位图只是表示像素的颜色值，在绘制的时候，OpenGL并不知道这些颜色值要对应哪些坐标上，这就涉及到了一个帖图的问题。

我们知道OpenGL绘制的单元是点，线或者三角形，对于2D和3D图形图片最终是由多个三角形拼接而成的，

![009.png-598.8kB][3]

对于每个三角形，纹理需要通过映射的方式将位图上的每一个像素对应到三角形的顶点上，

![008.png-88.5kB][4]

于是，

```
glBindTexture
```
绑定纹理到`GL_TEXTURE_2D`上，
接下来设置绑定的规则：
```
glTexParameteri 
/*
 参数1：纹理维度
 参数2：线性过滤、为s,t坐标设置模式
 参数3：wrapMode,环绕模式
 */
GL_TEXTURE_MIN_FILTER
GL_TEXTURE_MAG_FILTER
GL_TEXTURE_WRAP_S
GL_TEXTURE_WRAP_T
GL_CLAMP_TO_EDGE
```
接下来`glTexImage2D`载入纹理2D数据：
```
glTexImage2D
/*
 参数1：纹理模式，GL_TEXTURE_1D、GL_TEXTURE_2D、GL_TEXTURE_3D
 参数2：加载的层次，一般设置为0
 参数3：纹理的颜色值GL_RGBA
 参数4：宽
 参数5：高
 参数6：border，边界宽度
 参数7：format
 参数8：type
 参数9：纹理数据
 */
```
接下来把纹理缓存连接到着色器程序中，OpenGL从纹理缓存中将纹理数据传到GLSL中的`colorMap`，纹理数据与纹理坐标共同合成最终像素点：

![010.png-147.1kB][5]

这样看来`glUniform1i`这个函数就是GLSL与OpenGL传递数据用的；
```
glUniform1i
```

## 绘图并渲染

最后一步绘图并渲染：

```
//调用着色器程序
glUseProgram(program);
//绘制
glDrawArrays(GL_TRIANGLES, 0, 6);
//从渲染缓存区显示到屏幕上
[self.myContext presentRenderbuffer:GL_RENDERBUFFER];

```

![res.png-707.2kB][6]


  [1]: http://static.zybuluo.com/usiege/rkatxqgiwv7spaz92ckdjdqt/003.png
  [2]: http://static.zybuluo.com/usiege/9ei4qg31lshu2zzirvjh6yi8/007.png
  [3]: http://static.zybuluo.com/usiege/4mdlcn681thitot1x6v0qpih/009.png
  [4]: http://static.zybuluo.com/usiege/zakf10wqoj6vlihxn4cmcjln/008.png
  [5]: http://static.zybuluo.com/usiege/rhn9sxxjufvql3lvfo6tw397/010.png
  [6]: http://static.zybuluo.com/usiege/uk5tpo7ijlj3tfguyu7yitlf/res.png