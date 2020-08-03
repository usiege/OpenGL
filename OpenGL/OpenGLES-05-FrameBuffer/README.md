# OpenGL日常06(上)-FrameBuffer

标签（空格分隔）： OpenGL

---

大家好，欢迎来到听风的OpenGL日常。

## 写在前面

[本文主要代码](https://github.com/usiege/OpenGL_LIB/tree/master/OpenGL/OpenGLES-05-Texture)

上篇主要讲OpenGLES FrameBuffer的使用；理解本篇主要理解下面这幅图，所以文中会再出现一次：

![002.png-90kB][3]

## 快速添加到View

我们要自己渲染页面，建一个单视图的App，接下来创建一个`TFGLView:UIView`，令该`View`指针指向视图控制的`view`；

```
@interface ViewController ()
@property (nonatomic, strong) TFGLView* mView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.mView = (TFGLView*)self.view;
}
@end
```

### 添加OpenGLES上下文

OpenGLES在iOS端有自己的实现，直接在`Build Phases`里添加：

![001.png-34.9kB][1]

在`TFGLView`里引用，这里我们使用ES2.0版本：
```
#import <OpenGLES/ES2/gl.h>
```

OpenGLES绘制在View的特殊图层`CAEAGLLayer:CALayer`上，由于它是CALayer的子类，所以直接更改`View`的`layerClass`：

```
+(Class)layerClass
{
    return [CAEAGLLayer class];
}
```
同修改VC的view类似，我们也要对layer作修改，设置全局layer属性：
```
@property(nonatomic,strong)CAEAGLLayer *myEagLayer;
```
另外我们需要获取上下文来保存OpenGL的状态：
```
@property(nonatomic,strong)EAGLContext *myContext;
```
接下来在`layoutSubviews`方法中我们设置图层以及上下文；

1. 图层
```
-(void)setupLayer
{
    self.myEagLayer = (CAEAGLLayer *)self.layer;
    /*
     kEAGLDrawablePropertyRetainedBacking 表示绘图表面显示后，是否保留其内容
     kEAGLDrawablePropertyColorFormat 可绘制表面的内部颜色缓存区格式.
     这个key对应的值是一个NSString指定特定颜色缓存区对象,默认是kEAGLColorFormatRGBA8
     */
    self.myEagLayer.drawableProperties = @{
        kEAGLDrawablePropertyRetainedBacking:@false,
        kEAGLDrawablePropertyColorFormat:kEAGLColorFormatRGBA8
    };
}
```
这里注意下layer的`drawableProperties`属性，之后具体用到了再修改；
2. 上下文
```
-(void)setupContext
{
    //1.指定OpenGL ES 渲染API版本，我们使用2.0
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    //2.创建图形上下文
    EAGLContext *context = [[EAGLContext alloc]initWithAPI:api];
    //3.判断是否创建成功
    if (!context) {
        NSLog(@"Create context failed!");
        return;
    }
    //4.设置图形上下文
    if (![EAGLContext setCurrentContext:context]) {
        NSLog(@"setCurrentContext failed!");
        return;
    }
    //5.将局部context，变成全局的
    self.myContext = context;
}
```

## 进入GL渲染部分（本文主要部分）

完成了以上环境设置，就可以进入正式OpenGLES环节，在此之前我们要先认识两个缓冲区；

### 两个缓冲区
这篇文章我们要用这两个缓冲区来完成操作，先来了解一下这两个缓冲区：

在应用程序调用OpenGLES之前，都要创建一个上下文和一个绘图表面，而在EAGL、GLFW、GLAD等中都默认提供了一个上下文和绘图表面（即帧缓冲区）；

- frame buffer object
Frame Buffer Object(FBO)扩展，被推荐用于把数据渲染到纹理对象，即绑定到纹理对象；用于写入颜色值的颜色缓冲、用于写入深度信息的深度缓冲和允许我们根据一些条件丢弃特定片段的模板缓冲。这些缓冲结合起来叫做帧缓冲(Framebuffer)，它被储存在内存中。OpenGL允许我们定义我们自己的帧缓冲，也就是说我们能够定义我们自己的颜色缓冲，甚至是深度缓冲和模板缓冲。
对于这个对象的理解，我们现在可能并不能很好的感知，具体到实际场景会更好理解，之后遇到就更清楚了，今天我们用它来渲染纹理。
可参考：[OpenGL FrameBuffer Object](http://longzxr.blog.sohu.com/168909774.html)
framebuffer是帧缓冲对象，它是一个容器，但是自身不能用于渲染，需要与一些可渲染的缓冲区绑定在一起，像纹理或者渲染缓冲区。
在OpenGL渲染管线中，几何数据和纹理经过一系列的测试最终以2d像素图渲染到屏幕上，最终的渲染目的地就叫做帧缓冲区。它是由一些OpenGL操作的2d数组和存储空间组成。这个缓冲区完全的由窗口系统创建和管理，这个默认的缓冲区就叫做“窗口系统提供的”缓冲区。

- render buffer
可参考：[RenderBuffer](https://zhuanlan.zhihu.com/p/81336334)
RenderBuffer为颜色、深度和模板缓冲值提供了附着点，而RenderBuffer又被附加到帧缓冲。这样帧缓冲才具有了操作深度信息能力。（为了记忆可以理解为为帧缓冲打开了对应的测试）
![001.png-355kB][2]

上面两个缓冲区及缓冲区对象的关系大致如下图：

![002.png-90kB][3]

## 清空缓冲区

我们需要添加两个标识用来获取RenderBuffer和FrameBuffer：

```
@property(nonatomic,assign)GLuint colorRenderBuffer;
@property(nonatomic,assign)GLuint colorFrameBuffer;
```

清空缓冲区操作：
```
- (void)clearBuffers
{
    glDeleteBuffers(1, &_colorFrameBuffer);
    self.colorFrameBuffer = 0;
    
    glDeleteBuffers(1, &_colorRenderBuffer);
    self.colorRenderBuffer = 0;
}
```


## 设置RenderBuffer

生成渲染缓冲区，并保存在全局变量中：

```
GLuint buffer;
glGenRenderbuffers(1, &buffer);
self.colorRenderBuffer = buffer;
```
将标识符绑定到渲染缓存：
```
glBindRenderbuffer(GL_RENDERBUFFER, buffer);
```
将渲染缓存绑定到OpenGLES上：
```
[self.myContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:self.myEagLayer];
```
而对于不使用EAGL的情况，则需要：
```
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
```
上面这段代码创建了一个800*600的模板缓冲对象用于将模板缓冲绑定在RenderBuffer上；
    
## 设置FrameBuffer

同样，设置帧缓存对象，并绑定到帧缓存：

```
GLuint buffer;
glGenFramebuffers(1, &buffer);
self.colorFrameBuffer = buffer;

glBindFramebuffer(GL_FRAMEBUFFER, buffer);
```

接下来就是建立RenderBuffer与FrameBuffer的联系，将RBO附着在FBO的`GL_COLOR_ATTACHMENT0`上：
```
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, buffer);
```

## 设置着色器程序

本篇直接给出着色器程序，有关于着色器的用法见`05-Shader`；
顶点着色器`shader.vsh`：
```
attribute vec4 position;
attribute vec2 textCoordinate;
varying lowp vec2 varyTextCoord;

void main()
{
    varyTextCoord = textCoordinate;
    gl_Position = position;
}
```
片元着色器`shader.fsh`：
```
precision highp float;
varying lowp vec2 varyTextCoord;
uniform sampler2D colorMap;

void main()
{
    //lowp vec4 temp = texture2D(colorMap, varyTextCoord);
    //gl_FragColor = temp;
    
    gl_FragColor = texture2D(colorMap, varyTextCoord);

}
```
将生成写成一个函数：
```
- (GLuint)genShader:(NSString *)file :(GLenum)type
{
    NSString* content = [NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil];
    const GLchar* source = (GLchar *)[content UTF8String];
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    return shader;
}

```
生成并编译着色器：
```
//顶点着色器
NSString *vertFile = [[NSBundle mainBundle]pathForResource:@"shaderv" ofType:@"vsh"];
GLuint vShader = [self genShader:vertFile :GL_VERTEX_SHADER];

//片元着色器
NSString *fragFile = [[NSBundle mainBundle]pathForResource:@"shaderf" ofType:@"fsh"];
GLuint fShader = [self genShader:fragFile :GL_FRAGMENT_SHADER];
```
生成着色器程序，并把编译好的着色器附着在其上：
```
GLuint program = glCreateProgram();
glAttachShader(program, vShader);
glAttachShader(program, fShader);
//链接到程序后就可以删除着色器了，这是个好习惯
glDeleteShader(vShader);
glDeleteShader(fShader);
```

## 绘制

前面的准备工作已经完成，接下来开始正式开始绘制纹理；我们将在一个`drawTexture`完成
```
- (void)drawTexture
{
    //
}
```

### 渲染背景

```
//设置清屏颜色
glClearColor(0.3f, 0.45f, 0.5f, 1.0f);
//清除屏幕
glClear(GL_COLOR_BUFFER_BIT);
```

### 链接并检查着色器程序

```
GLuint program = [self setupProgram];
glLinkProgram(program);
```
链接后需要对程序做检查，如果有错误，则我们容易定位着色器出错的细节：
```
GLint linkStatus;
//获取链接状态
glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
if (linkStatus == GL_FALSE) {
    GLchar message[512];
    glGetProgramInfoLog(program, sizeof(message), 0, &message[0]);
    NSString *messageString = [NSString stringWithUTF8String:message];
    NSLog(@"Program Link Error:%@",messageString);
    return;
}
```

## 见下篇

[下篇](https://github.com/usiege/OpenGL_LIB/tree/master/OpenGL/OpenGLES-06-FrameBuffer)

  [1]: http://static.zybuluo.com/usiege/fqbp8pzeka7b32ohhh30mkay/001.png
  [2]: http://static.zybuluo.com/usiege/tclv736q675qyf0au6d3acql/001.png
  [3]: http://static.zybuluo.com/usiege/nhbgyvhu806lshewb046y2ll/002.png
  [4]: http://static.zybuluo.com/usiege/jmcxzq3byzuyn4sad2089zto/003.png