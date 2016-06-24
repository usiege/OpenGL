//
//  Arithmetic.h
//  OpenGL_03
//
//  Created by 先锋电子技术 on 16/6/24.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#ifndef Arithmetic_h
#define Arithmetic_h

#include <OpenGL/OpenGL.h>

/*
* 一些算法
*/
void lineDDA(int x0,int y0,int xEnd,int yEnd);
void lineBres(int x0,int y0,int xEnd,int yEnd);
void circleMidpoint(GLint x, GLint y, GLint radius);
void ellipseMidpoint(int xCenter, int yCenter, int rx, int ry);

class ScreenPt {
private:
    GLint x,y;
    
public:
    ScreenPt(){
        x = y = 0;
    }
    void setCroods(GLint xCoordValue, GLint yCoordValue){
        x = xCoordValue;
        y = yCoordValue;
    }
    
    GLint getX(void) const { return x; }
    GLint getY(void) const { return y; }
    
    void incrementX(void) { x++; }
    void decrementY(void) { y--; }
    
};

#endif /* Arithmetic_h */






