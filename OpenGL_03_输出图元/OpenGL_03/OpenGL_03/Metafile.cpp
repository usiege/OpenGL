//
//  Metafile.cpp
//  OpenGL_03
//
//  Created by 先锋电子技术 on 16/6/23.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include "Metafile.hpp"
#include <OpenGL/gl.h>

void drawLine(void){
    printf("画一条线\n");
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    
    glColor3f(0, 1, 0);
    
    int point1[] = {0,0};
    int point2[] = {200,150};
    int point3[] = {0,200};
    
    glBegin(GL_LINES);
    glVertex2iv(point1);
    glVertex2iv(point2);
    glVertex2iv(point3);
    glEnd();
    /**
     *  如果列出的端点数为奇数，则最后一个端点被忽略；
     */
    glFlush();
}

void drawPolyline(void){
    printf("画折线");
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0, 0, 1);
    int point[5][2] = {{0,0},{100,20},{30,100},{150,10},{150,150}};
    
    glBegin(GL_LINE_STRIP);//strip条状，剥去
    for (int i=0; i<5; i++) {
        glVertex2iv(point[i]);
    }
    glEnd();
    /**
     *  使用图元常量GL_LINE_STRIP可获得折线；
     *  如果不列出至少两个点，则什么也不显示；
     *
     */
    
    glColor3f(1, 0, 0);
    int pointLoop[5][2] = {{100,100},{20,20},{50,50},{150,10},{70,10}};
    
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<5; i++) {
        glVertex2iv(pointLoop[i]);
    }
    glEnd();
    /**
     *  使用图元常量GL_LINE_LOOP可获得闭合折线；
     *  如果不列出至少两个点，则什么也不显示；
     *  最后一个端点与第一个端点相连接；
     *
     */

    
    glFlush();
}
