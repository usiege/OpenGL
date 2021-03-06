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
#include "Arithmetic.h"

/**
 *  @brief 本例子是《计算机图形学》第三章的示例程序
    由于在学习过程中，将学习重心放在了OpenGL蓝皮书上
    所以除本例均是与蓝皮书相关的，望读者注意；
 */
int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); //初始位置
    glutInitWindowSize(600, 450);//大小
    glutCreateWindow("显示图元");
    
    gluOrtho2D(0, 200, 0, 150);
    
//    lineBres(0, 0, 100, 90);
//    ScreenPt pt = ScreenPt();
//    MetaTest mt = MetaTest();
    
    
    //只调用最后一次的
    glutDisplayFunc(drawLine);
    glutDisplayFunc(drawPolyline);
    glutMainLoop();
    
    return 0;
}
