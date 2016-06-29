//
//  Triangle.cpp
//  OGE_03_Triangle
//
//  Created by 先锋电子技术 on 16/6/29.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include "Triangle.hpp"

GLBatch trangle_batch;
GLShaderManager shander_manger;

void changeSize(int width, int height)
{
    printf("width = %d, height = %d\n",width,height);
    glViewport(0, 0, width, height);
}

void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GLfloat vReds[] = { 1,0,0,1 };
    shander_manger.UseStockShader(GLT_SHADER_IDENTITY,vReds);
    trangle_batch.Draw();
    
    glutSwapBuffers();
}

//Render context渲染环境
void setupRC(void)
{
    glClearColor(0, 0, 1, 1);
    shander_manger.InitializeStockShaders();
    
    float vVerts[] = {
        -0.5,0,0,
        0.5,0,0,
        0,0.5,0
    };
    
    trangle_batch.Begin(GL_TRIANGLES, 3);
    trangle_batch.CopyVertexData3f(vVerts);
    trangle_batch.End();
}






