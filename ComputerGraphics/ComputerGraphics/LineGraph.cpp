//
//  LineGraph.cpp
//  ComputerGraphics
//
//  Created by charles on 2018/4/23.
//  Copyright © 2018年 charles. All rights reserved.
//

#include "LineGraph.hpp"

GLsizei winWidth = 600;
GLsizei winHeight = 500;

GLint xRaster = 25;
GLint yRaster = 150; // raster 光栅

GLubyte label[36] = {'J','a','n',   'F','e','b',    'M','a','r',
    'A','p','r',    'M','a','y',    'J','u','n',
    'J','u','l',    'A','u','g',    'S','e','p',
    'O','c','t',    'N','o','v',    'D','e','c'};
GLint dataValue[12] = {420, 342, 324, 310, 262, 185, 190, 196, 217, 240, 312, 438};


void initLineGraph()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 500.0);     //指定二维世界坐标系
}

void lineGraph()
{
    GLint month, k;
    GLint x = 30;
    
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);   //set line color blue
    
    glBegin(GL_LINE_STRIP); //折线
    for (k = 0; k < 12; k++) {
        glVertex2i(x + k*50, dataValue[k]);
    }
    glEnd();
    
    glColor3f(1.0, 0.0, 0.0);
    for (k = 0; k < 12; k++) {
        glRasterPos2i(xRaster + k*50, dataValue[k] - 4);
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
    }
    
    glColor3f(0.0, 0.0, 0.0);
    xRaster = 20;
    for (month = 0; month < 12; month++) {
        glRasterPos2i(xRaster, yRaster);
        for (k = 3*month; k < 3*month + 3; k++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[k]);
        }
        xRaster += 50;
    }
    glFlush();
}

void lineWinReshapeFun(GLint newWinth, GLint newHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GLdouble(newWinth), 0.0, GLdouble(newHeight));
    
    glClear(GL_COLOR_BUFFER_BIT);
}










