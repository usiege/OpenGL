//
//  PieChart.cpp
//  ComputerGraphics
//
//  Created by charles on 2018/4/24.
//  Copyright © 2018年 charles. All rights reserved.
//

#include "PieChart.hpp"
#include <math.h>

//部分函数缺失，不作补充

const GLdouble twoPi = 6.283185;

class scrPt
{
public:
    GLint x, y;
    
};

GLsizei PwinWidth = 400, PwinHeight = 300;

void pieChartInit()
{
    scrPt circCtr, piePt;
    GLint radius = PwinWidth / 4;
    
    GLdouble sliceAngle, previousSliceAngle = 0.0;
    
    GLint k, nSlices = 12;
    GLfloat dataValue[12] = { 10.0, 7.0, 13.0, 5.0, 13.0, 14.0, 3.0, 16.0, 5.0, 3.0, 17.0, 8.0 };
    
    GLfloat dataSum = 0.0;
    
    circCtr.x = PwinWidth/2;
    circCtr.y = PwinHeight/2;
//    circleMidpoint(
    
    
}

void pieChart()
{
    
}

void displayFuncPieChart(void)
{
    
}

void winReshapeFuncPieChart(GLint newWidth, GLint newHeight)
{
    
}
