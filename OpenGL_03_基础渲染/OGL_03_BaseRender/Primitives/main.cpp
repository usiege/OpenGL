// Primitieves.cpp
// OpenGL SuperBible, Chapter 2
// Demonstrates the 7 Geometric Primitives
// Program by Richard S. Wright Jr.

#include <stdio.h>

//#include <GLTools.h>	// OpenGL toolkit
//#include <GLUT/GLUT.h>

#include "Primitieves.hpp"

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GL_POINTS");
    
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPressFunc);//普通按键
    glutSpecialFunc(SpecialKeys);  //特殊按键
    
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    SetupRC();
    
    glutMainLoop();
    return 0;
}
