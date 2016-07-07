//
//  GeoTest.h
//  OGL_03_BaseRender
//
//  Created by 先锋电子技术 on 16/7/7.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#ifndef GeoTest_h
#define GeoTest_h

#include <GLTools.h>	// OpenGL toolkit
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>

#include <math.h>
#include <GLUT/GLUT.h>

void ProcessMenu(int value);
void RenderScene(void);
void SetupRC();
void SpecialKeys(int key, int x, int y);
void ChangeSize(int w, int h);

#endif /* GeoTest_h */
