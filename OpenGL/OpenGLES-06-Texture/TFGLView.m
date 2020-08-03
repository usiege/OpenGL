//
//  TFGLView.m
//  OpenGLES-05-Texture
//
//  Created by charles on 2020/7/30.
//  Copyright © 2020 charles. All rights reserved.
//


#import "TFGLView.h"
#import <OpenGLES/ES2/gl.h>

@interface TFGLView ()

//OpenGL环境
@property(nonatomic,strong) CAEAGLLayer *myEagLayer;
@property(nonatomic,strong) EAGLContext *myContext;

//buffers
@property(nonatomic,assign)GLuint colorRenderBuffer;
@property(nonatomic,assign)GLuint colorFrameBuffer;

@end

@implementation TFGLView


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
//- (void)drawRect:(CGRect)rect
//{
//    // Drawing code
//    NSLog(@"draw rect");
//
//}

- (void)layoutSubviews
{
    NSLog(@"layout subviews");
    
    [self setupLayer];
    
    [self setupContext];
    
    //为了便于区分，这里我们有一个分界线
    
    [self clearBuffers];
    
    [self renderBuffer];
    
    [self frameBuffer];
    
    [self drawTexture];
}



- (void)drawTexture
{
    //1
    glClearColor(0.3f, 0.45f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    CGFloat scale = [[UIScreen mainScreen]scale];
    glViewport(self.frame.origin.x * scale, self.frame.origin.y * scale,
               self.frame.size.width * scale, self.frame.size.height * scale);
    
    //2
    GLuint program = [self setupProgram];
    glLinkProgram(program);
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
    NSLog(@"Program Link Success!");
    
    //3
    GLfloat attrArr[] =
    {
        0.5f, -0.5f, -1.0f,     1.0f, 0.0f,
        -0.5f, 0.5f, -1.0f,     0.0f, 1.0f,
        -0.5f, -0.5f, -1.0f,    0.0f, 0.0f,
        
        0.5f, 0.5f, -1.0f,      1.0f, 1.0f,
        -0.5f, 0.5f, -1.0f,     0.0f, 1.0f,
        0.5f, -0.5f, -1.0f,     1.0f, 0.0f,
    };
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
    
    //4
    [self setupTexture:@"texture"];
    
    //5
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "colorMap"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    [self.myContext presentRenderbuffer:GL_RENDERBUFFER];
}

- (GLuint)setupTexture:(NSString *)fileName {

    CGImageRef spriteImage = [UIImage imageNamed:fileName].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName);
        exit(1);
    }

    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);

    GLubyte * spriteData = (GLubyte *) calloc(width * height * 4, sizeof(GLubyte));
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4,CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    CGRect rect = CGRectMake(0, 0, width, height);
    CGContextDrawImage(spriteContext, rect, spriteImage);
    CGContextRelease(spriteContext);

    glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    float fw = width, fh = height;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fw, fh, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    
    free(spriteData);
    return 0;
}


- (GLuint)setupProgram
{
    GLuint program = glCreateProgram();
    
    NSString *vertFile = [[NSBundle mainBundle]pathForResource:@"shaderv" ofType:@"vsh"];
    GLuint vShader = [self genShader:vertFile :GL_VERTEX_SHADER];
    NSLog(@"vertFile:%@",vertFile);

    NSString *fragFile = [[NSBundle mainBundle]pathForResource:@"shaderf" ofType:@"fsh"];
    GLuint fShader = [self genShader:fragFile :GL_FRAGMENT_SHADER];
    NSLog(@"fragFile:%@",fragFile);
    
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    
    return program;
}

- (GLuint)genShader:(NSString *)file :(GLenum)type
{
    NSString* content = [NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil];
    const GLchar* source = (GLchar *)[content UTF8String];
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    return shader;
}

- (void)frameBuffer
{
    GLuint buffer;
    glGenFramebuffers(1, &buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, buffer);
    
    self.colorFrameBuffer = buffer;
}

- (void)renderBuffer
{
    GLuint buffer;
    glGenRenderbuffers(1, &buffer);

    glBindRenderbuffer(GL_RENDERBUFFER, buffer);
    [self.myContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:self.myEagLayer];
    self.colorRenderBuffer = buffer;
}

- (void)clearBuffers
{
    glDeleteBuffers(1, &_colorFrameBuffer);
    self.colorFrameBuffer = 0;
    
    glDeleteBuffers(1, &_colorRenderBuffer);
    self.colorRenderBuffer = 0;
}


- (void)setupLayer
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
    self.contentScaleFactor = [[UIScreen mainScreen] scale];
}

- (void)setupContext
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


+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

@end
