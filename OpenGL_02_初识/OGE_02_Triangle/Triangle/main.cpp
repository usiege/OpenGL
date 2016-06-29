//
//  main.cpp
//  OGE_03_Triangle
//
//  Created by 先锋电子技术 on 16/6/29.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include <iostream>
#include "Triangle.hpp"

int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    //设置当前工作目录，将当前工作文件夹改为程序捆绑包中的/Resource文件夹
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    //双缓冲，RGBA颜色，深度缓冲，模板缓冲
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangle");
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    
    GLenum err = glewInit();
    if ( GLEW_OK != err) {
        fprintf(stderr, "GLEW Error:%s\n",glewGetErrorString(err));
        return 1;
    }

    setupRC();
    
    glutMainLoop();
    return 0;
}
