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

int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Line Graph");
    
    initLineGraph();
    
    glutDisplayFunc(lineGraph);
    glutReshapeFunc(lineWinReshapeFun);
    
    glutMainLoop();
    
    test();
    
    return 0;
}






