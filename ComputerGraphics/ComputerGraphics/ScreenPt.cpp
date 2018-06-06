//
//  ScreenPt.cpp
//  ComputerGraphics
//
//  Created by charles on 2018/4/22.
//  Copyright © 2018年 charles. All rights reserved.
//

#include "ScreenPt.hpp"
#include <GLUT/glut.h>
#include <math.h>

const double TWO_PI = 6.2831853;

static GLsizei winWidth = 600;

GLsizei SwinWidth = 400, SwinHeight = 400;
GLuint regHex;

class screenPt {
    
private:
    GLint x, y;
    
public:
    screenPt() {
        x = y = 0;
    }
    
    void setCoords(GLint xCoord, GLint yCoord) {
        x = xCoord;
        y = yCoord;
    }
    
    GLint getx() const {
        return x;
    }
    
    GLint gety() const {
        return y;
    }
};

void initScreenPt(void)
{
    screenPt hexVertex, circCtr;
    GLdouble theta;
    GLint k;
    
    circCtr.setCoords(SwinWidth / 2, SwinHeight / 2);
    
    glClearColor(1.0, 1.0, 1.0, 0.0); //display window color = white
    //显示表
    regHex = glGenLists(1);
    glNewList(regHex, GL_COMPILE);
    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for (k = 0; k < 6; k++) {
        theta = TWO_PI * k / 6.0;
        hexVertex.setCoords(circCtr.getx() + 150 * cos(theta),
                            circCtr.gety() + 150 * sin(theta));
        glVertex2i(hexVertex.getx(), hexVertex.gety());
    }
    glEnd();
    glEndList();
}

void regHexagon(void)
{
    //GL_COLOR_BUFFER_BIT 缓冲标识位，颜色xeuk
    glClear(GL_COLOR_BUFFER_BIT);
    glCallList(regHex);
    glFlush();
}

void winReshapeFun2(int newWidth, int newHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(0.0, (GLdouble)newWidth, 0.0, (GLdouble)newHeight);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void test() {
    
}


