//
//  LineGraph.hpp
//  ComputerGraphics
//
//  Created by charles on 2018/4/23.
//  Copyright © 2018年 charles. All rights reserved.
//

#ifndef LineGraph_hpp
#define LineGraph_hpp

#include <stdio.h>

#include <GLUT/glut.h>

namespace LineGraph {
    void init();
    void _init();
}

void winReshapeFun(GLint newWinth, GLint newHeight);

void lineGraph();
//直方图
void barChart();

#endif /* LineGraph_hpp */
