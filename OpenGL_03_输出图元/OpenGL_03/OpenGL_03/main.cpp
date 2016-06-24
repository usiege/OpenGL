//
//  main.cpp
//  OpenGL_03
//
//  Created by 先锋电子技术 on 16/6/23.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>

#include "Metafile.hpp"

int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); //初始位置
    glutInitWindowSize(600, 450);//大小
    glutCreateWindow("显示图元");
    
    gluOrtho2D(0, 200, 0, 100);
    glutDisplayFunc(drawLine);
    glutMainLoop();
    
    return 0;
}
