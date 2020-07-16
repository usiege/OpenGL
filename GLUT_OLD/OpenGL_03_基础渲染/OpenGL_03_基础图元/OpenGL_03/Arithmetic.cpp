//
//  Arithmetic.c
//  OpenGL_03
//
//  Created by 先锋电子技术 on 16/6/24.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include "Arithmetic.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <OpenGL/gl.h>

//inline int round(const float a) { return (int)(a+0.5); };

//设置像素
void setPixel(int x, int y){
    printf("(%d,%d)\n",x,y);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void lineDDA(int x0,int y0,int xEnd,int yEnd)
{
    int dx = xEnd - x0, dy = yEnd - y0, steps;
    float xIncrement, yIncrement, x = x0, y = y0;
    
    //fabs abs
    if (abs(dx) > abs(dy)) {
        steps = abs(dx);
    }else{
        steps = abs(dy);
    }
    
    //round函数功能:四舍五入取整
    xIncrement = (float)dx / steps;
    yIncrement = (float)dy / steps;
    
    setPixel(round(xIncrement),round(yIncrement));
    for (int k = 0; k < steps; k++) {
        x += xIncrement;
        y += yIncrement;
        setPixel(round(xIncrement),round(yIncrement));
    }
}

// |m| < 1.0时的bresenham算法
//光栅画线算法
void lineBres(int x0,int y0,int xEnd,int yEnd)
{
    int dx = abs(xEnd - x0), dy = abs(yEnd - y0);
    int p = 2*dy - dx;
    int twoDy = 2*dy, twoDyMinusDx = 2*(dy - dx);
    
    int x,y;
    //决定起始点
    if (x0 > xEnd) {
        x = xEnd;
        y = yEnd;
        xEnd = x0;
    }else{
        x = x0;
        y = y0;
        xEnd = xEnd;
    }
    
    setPixel(x, y);
    while (x < xEnd){
        x++;
        if (p < 0) {
            p += twoDy;
        }else{
            y++;
            p += twoDyMinusDx;
        }
        setPixel(x, y);
    }
}

//中点画圆算法
void circleMidpoint(GLint xc, GLint yc, GLint radius)
{
    ScreenPt circlePt;
    
    GLint p = 1 - radius;
    circlePt.setCroods(0, p);//圆的顶点
    
    void circlePlotPoints(GLint xc, GLint yc, ScreenPt circlePt);
    //初始化四分之一圆并标出 qradrant(四分之一) octant(八分之一)
    circlePlotPoints(xc,yc,circlePt);
    
    //计算下一个点并标出八分之一
    while (circlePt.getX() < circlePt.getY()) {
        circlePt.incrementX();
        if (p < 0) {
            p += 2*circlePt.getX() + 1;
        }else{
            circlePt.decrementY();
            p += 2*(circlePt.getX() - circlePt.getY()) + 1;
        }
        circlePlotPoints(xc, yc, circlePt);
    }
}

void circlePlotPoints(GLint xc, GLint yc, ScreenPt circlePt)
{
    setPixel(xc + circlePt.getX(), yc + circlePt.getY());
    setPixel(xc - circlePt.getX(), yc + circlePt.getY());
    setPixel(xc + circlePt.getX(), yc - circlePt.getY());
    setPixel(xc - circlePt.getX(), yc - circlePt.getY());
    
    setPixel(xc + circlePt.getY(), yc + circlePt.getX());
    setPixel(xc - circlePt.getY(), yc + circlePt.getX());
    setPixel(xc + circlePt.getY(), yc - circlePt.getX());
    setPixel(xc - circlePt.getY(), yc - circlePt.getX());
}


//中心椭圆算法
void ellipseMidpoint(int xCenter, int yCenter, int rx, int ry)
{
    int rx2 = rx * rx, ry2 = ry * ry;
    int twoRx2 = 2*rx2, twoRy2 = 2*ry2;
    
    int p;//决策参数
    int x = 0, y = ry;
    int px = 0, py = twoRx2 * y;
    
    void ellipsePlotPoints(int,int,int,int);
    ellipseMidpoint(xCenter, yCenter, x, y);
    
    //区域1
    p = round(ry2 - (rx2 * ry) + (0.25*rx2));
    while (px < py) {
        x++;
        px += twoRy2;
        if (p < 0) {
            p += ry2 + px;
        }else{
            y--;
            py -= twoRx2;
            p += ry2 + px - py;
        }
        ellipseMidpoint(xCenter, yCenter, x, y);
    }
    
    //区域2
    p = round(ry2 * (x+0.5) * (x+0.5) + rx2 * (y-1) * (y-1) + rx2 * ry2);
    while (y > 0) {
        y--;
        py -= twoRx2;
        if (p > 0) {
            p += rx2 - py;
        }else{
            x++;
            px += twoRy2;
            p += rx2 - py + px;
        }
        ellipseMidpoint(xCenter, yCenter, x, y);
    }
}

void ellipsePlotPoints(int xCenter,int yCenter,int x,int y)
{
    setPixel(xCenter + x, yCenter + y);
    setPixel(xCenter - x, yCenter + y);
    setPixel(xCenter + x, yCenter - y);
    setPixel(xCenter + x, yCenter - y);
}







