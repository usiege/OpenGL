//
//  Primitieves.hpp
//  OGL_03_BaseRender
//
//  Created by 先锋电子技术 on 16/6/30.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#ifndef Primitieves_hpp
#define Primitieves_hpp

#include <stdio.h>

#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLGeometryTransform.h>

#include <math.h>
#include <GLUT/GLUT.h>

void SetupRC();
void RenderScene(void);
void SpecialKeys(int key, int x, int y);
void KeyPressFunc(unsigned char key, int x, int y);
void ChangeSize(int w, int h);

#endif /* Primitieves_hpp */
