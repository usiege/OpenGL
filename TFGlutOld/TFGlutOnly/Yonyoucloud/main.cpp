//
//  main.cpp
//  Yonyoucloud
//
//  Created by user on 2018/7/9.
//  Copyright © 2018年 usiege. All rights reserved.
//

#include <iostream>

#include <GLUT/glut.h>
#include <OpenGL/gl.h>

void renderScene();

int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,0);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("我不是药神，强烈推荐");
    
    glClearColor(0.0f, .5f, 0.0f, 0.0f);
    glutDisplayFunc(renderScene);
    glutMainLoop();
    
    return 0;
}

//void renderScene() {
//    static const GLfloat vertices[] = {
//        -1.0f, -1.0f, .0f,
//        1.0f, -1.0f, .0f,
//        .0f, 1.0f, .0f,
//    };
//
//}

#if 1
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    //清除帧缓存，使用在上面指定过的颜色
    glutSwapBuffers();
    //告诉 GLUT 在 backbuffer 和 frontbuffer 之间进行交换。
    //在通过帧回调函数的下一个循环中，我们将场景渲染到当前的 frontbuffer 中，而 backbuffer 将被显示。
    
    static const GLfloat vertices[] = {
        -1.0f, -1.0f, .0f,
        1.0f, -1.0f, .0f,
        .0f, 1.0f, .0f,
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
//    glDrawArrays(GL_POINTS, 0, 1);    //画点
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(0);
    
}
//顶点缓冲区对象
#endif
