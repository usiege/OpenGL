//
//  Square.hpp
//  OGE_03_Triangle
//
//  Created by 先锋电子技术 on 16/6/29.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#ifndef Square_hpp
#define Square_hpp

#include <stdio.h>

#include <GLTools.h>	// OpenGL toolkit
#include <GLShaderManager.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif


void SetupRC();
void RenderScene(void);
void ChangeSize(int w, int h);
void SpecialKeys(int key, int x, int y);

#endif /* Square_hpp */
