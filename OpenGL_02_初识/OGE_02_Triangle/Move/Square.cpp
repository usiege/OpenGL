//
//  Square.cpp
//  OGE_03_Triangle
//
//  Created by 先锋电子技术 on 16/6/29.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include "Square.hpp"

GLBatch	squareBatch;
GLShaderManager	shaderManager;

GLfloat blockSize = 0.1f;
GLfloat vVerts[] = {
    -blockSize, -blockSize, 0.0f,
    blockSize, -blockSize, 0.0f,
    blockSize,  blockSize, 0.0f,
    -blockSize,  blockSize, 0.0f};

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
    // Black background
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f );
    
    shaderManager.InitializeStockShaders();
    
    // Load up a triangle
    squareBatch.Begin(GL_TRIANGLE_FAN, 4);
    squareBatch.CopyVertexData3f(vVerts);
    squareBatch.End();
}

// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
{
    GLfloat stepSize = 0.025f;
    
    GLfloat blockX = vVerts[0];   // Upper left X
    GLfloat blockY = vVerts[7];  // Upper left Y
    
    if(key == GLUT_KEY_UP)
        blockY += stepSize;
    
    if(key == GLUT_KEY_DOWN)
        blockY -= stepSize;
    
    if(key == GLUT_KEY_LEFT)
        blockX -= stepSize;
    
    if(key == GLUT_KEY_RIGHT)
        blockX += stepSize;
    
    // Collision detection
    if(blockX < -1.0f) blockX = -1.0f;
    if(blockX > (1.0f - blockSize * 2)) blockX = 1.0f - blockSize * 2;;
    if(blockY < -1.0f + blockSize * 2)  blockY = -1.0f + blockSize * 2;
    if(blockY > 1.0f) blockY = 1.0f;
    
    // Recalculate vertex positions
    /**
     *  @brief 注意Z坐标全部是0，所以不用设置，以下为四个点的新坐标
     */
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize*2;
    
    vVerts[3] = blockX + blockSize*2;
    vVerts[4] = blockY - blockSize*2;
    
    vVerts[6] = blockX + blockSize*2;
    vVerts[7] = blockY;
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    
    squareBatch.CopyVertexData3f(vVerts);
    
    /**
     *  @brief 告诉GLUT需要更新显示内容
     */
    glutPostRedisplay();
}


// Respond to arrow keys by moving the camera frame of reference
void BounceFunction(void)
{
    static GLfloat xDir = 1.0f;
    static GLfloat yDir = 1.0f;
    
    GLfloat stepSize = 0.005f;
    
    GLfloat blockX = vVerts[0];   // Upper left X
    GLfloat blockY = vVerts[7];  // Upper left Y
    
    blockY += stepSize * yDir;
    blockX += stepSize * xDir;
    
    // Collision detection
    if(blockX < -1.0f) { blockX = -1.0f; xDir *= -1.0f; }
    if(blockX > (1.0f - blockSize * 2)) { blockX = 1.0f - blockSize * 2; xDir *= -1.0f; }
    if(blockY < -1.0f + blockSize * 2)  { blockY = -1.0f + blockSize * 2; yDir *= -1.0f; }
    if(blockY > 1.0f) { blockY = 1.0f; yDir *= -1.0f; }
    
    // Recalculate vertex positions
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize*2;
    
    vVerts[3] = blockX + blockSize*2;
    vVerts[4] = blockY - blockSize*2;
    
    vVerts[6] = blockX + blockSize*2;
    vVerts[7] = blockY;
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    
    squareBatch.CopyVertexData3f(vVerts);
}


///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    squareBatch.Draw();
    
    // Flush drawing commands
    glutSwapBuffers();
    
    BounceFunction();
    glutPostRedisplay();
}



///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}
