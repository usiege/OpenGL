//
//  Metafile.cpp
//  OpenGL_03
//
//  Created by 先锋电子技术 on 16/6/23.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include "Metafile.hpp"
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>

void drawLine(void){
    printf("画一条线\n");
    
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    
    glColor3f(0, 1, 0);
    
    int point1[] = {},point2[] = {},point3[] = {};
    glBegin(GL_LINES);
    glVertex2iv(point1);
    glVertex2iv(point2);
    glVertex2iv(point3);
    glEnd();
    /*
     如果列出的端点数为奇数，则最后一个端点被忽略；
     
     */
    
    glFlush();
}