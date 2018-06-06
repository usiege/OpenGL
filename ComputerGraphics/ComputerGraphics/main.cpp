//
//  main.cpp
//  ComputerGraphics
//
//  Created by charles on 2018/4/18.
//  Copyright © 2018年 charles. All rights reserved.
//

#include <iostream>

#include <GLUT/glut.h>
#include <math.h>

static void test() {
    
}

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

int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Line Graph");
    
//    init();
    
    glutDisplayFunc(pointFunc);
    glutReshapeFunc(winReshapeFun);
    
    glutMainLoop();
    
    return 0;
}






