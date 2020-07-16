//
//  main.cpp
//  ComputerGraphics
//
//  Created by charles on 2018/4/18.
//  Copyright © 2018年 charles. All rights reserved.
//

#include <iostream>

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <math.h>

//static void test() {
//
//}

#include "ScreenPt.hpp"
/*
 You can solve this if you review your main-function and remove "const" from the following main function:
 int main(int argc, char * argv[])
 and build again.
 */

#include "LineGraph.hpp"

extern GLint winWidth;
extern GLint winHeight;


void pointFunc() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glVertex2i(50, 100);
    
    glPointSize(2.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2i(75, 150);
    
    glPointSize(3.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2i(100, 200);
    glEnd();
    
//    glFlush();
}

void openglFunc();
int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(1024, 768);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Line Graph");
    
    glutDisplayFunc(openglFunc);
//    glutReshapeFunc(winReshapeFun);
    
    // 缓存清空后的颜色值
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    
    glutMainLoop();
    
    return 0;
}
bool trangle = true;
void openglFunc() {
    // 清空颜色缓存
    glClear(GL_COLOR_BUFFER_BIT);
    // 交换前后缓存
    glutSwapBuffers();
    
    if(trangle) {
        //创建一个顶点数组对象
//        GLuint VertexArrayID;
//        glGenVertexArraysAPPLE(1, &VertexArrayID);
//        glBindVertexArrayAPPLE(VertexArrayID);

        static const GLfloat g_vertext_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,1.0f, 0.0f,
        };
        
        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertext_buffer_data), g_vertext_buffer_data, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        
    }
}






