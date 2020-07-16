//
//  main.cpp
//  OpenGL_Ch_02
//
//  Created by 先锋电子技术 on 16/6/22.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>

using namespace std;

void display(void);
void draw(void);

int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    glutInit(&argc, argv); //const修饰的指针常量与常量指针的区别
    glutInitWindowPosition(100, 100); //初始位置
    glutInitWindowSize(600, 450);//大小
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//默认显示模式：单缓存 RGB显示模型
    glutCreateWindow("Xcode Glut Demo"); //给定一个标题
    
    glutDisplayFunc(draw);
    glutMainLoop();
    
    return 0;
}

void draw()
{
    std::cout << "书中例子" << std::endl;
    gluOrtho2D(10, 200, 10, 150);
    
    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); //正投影
    
    glColor3f(1, 0, 0);
    
    glBegin(GL_LINES);
    glVertex2i(200, 0);
    glVertex2i(0, 150);
    glVertex2i(100, 100);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2i(200-10*1.4, 0);
    glVertex2i(0, 140);
    glEnd();
    
    glFlush();
}

void display()
{
    std::cout << "画一个方形";
    glClearColor(0.5, 0.5, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor3f(0.1, 1.0, 0.5);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
    glFlush();
}

//openGL 显示表
void showList()
{
    const double TWO_PI = 6.2831853;
    
    GLuint regHex;
    
    GLdouble theta;
    GLint x, y, k;
    
    //Get an identifier for the display list.
    regHex = glGenLists(1);
    
    glNewList(regHex, GL_COMPILE);
    glBegin(GL_POLYGON);
    for (k = 0; k < 6; k++) {
        theta = TWO_PI * k / 6.0;
        x = 200 + 150 * cos(theta);
        y = 200 + 150 * sin(theta);
        glVertex2i(x, y);
    }
    glEnd();
    glEndList();
    
    glCallList(regHex);
//    glCallLists(GLsizei n, GLenum type, const GLvoid *lists)
    //删除显示表
//    glDeleteLists(startID, nLists)
}

//












