//
//  main.cpp
//  MatrixTransition
//
//  Created by 先锋电子技术 on 16/7/18.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>

#include <math.h>
#include <stdio.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

int main(int argc, const char * argv[]) {
    // insert code here...
    
 
//    GLFrame
    return 0;
}

///////////////////////////////////////////////////////////////////////
// Just assemble the matrix
void GetMatrix(M3DMatrix44f matrix, bool bRotationOnly = false)
{
    M3DVector3f vOrigin;	// Where am I?
    M3DVector3f vForward;	// Where am I going?
    M3DVector3f vUp;		// Which way is up?
 
    // Calculate the right side (x) vector, drop it right into the matrix
    // 计算x，并把它放在矩阵中的右边
    M3DVector3f vXAxis;
    m3dCrossProduct3(vXAxis, vUp, vForward);
    
    // Set matrix column does not fill in the fourth value...
    // 设置矩阵列，并没有填冲第4个值
    m3dSetMatrixColumn44(matrix, vXAxis, 0);
    matrix[3] = 0.0f;
    
    // Y Column
    m3dSetMatrixColumn44(matrix, vUp, 1);
    matrix[7] = 0.0f;
    
    // Z Column
    m3dSetMatrixColumn44(matrix, vForward, 2);
    matrix[11] = 0.0f;
    
    // Translation (already done)
    if(bRotationOnly == true)
    {
        matrix[12] = 0.0f;
        matrix[13] = 0.0f;
        matrix[14] = 0.0f;
    }
    else
        m3dSetMatrixColumn44(matrix, vOrigin, 3);
    
    matrix[15] = 1.0f;
}
