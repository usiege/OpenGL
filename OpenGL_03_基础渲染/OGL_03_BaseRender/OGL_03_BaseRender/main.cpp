//
//  main.cpp
//  OGL_03_BaseRender
//
//  Created by 先锋电子技术 on 16/6/30.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#include <iostream>
#include <GLTools.h>
#include <GLUT/GLUT.h>

int a = 0;              //全局初始化区
char *p1;				//全局未初始化区
int main()
{
    int b;			    //栈
    char s[] = "abc";		//栈
    char *p2;			//栈
    char *p3 = "123456";	//123456\0在常量区，p3在栈上。
    static int c =0;		//全局（静态）初始化区
    p1 = (char *)malloc(10);
    p2 = (char *)malloc(20);	//分配得来得10和20字节的区域就在堆区。
    strcpy(p1,"123456");	//123456\0放在常量区，编译器可能会将它与p3所指向的"123456"优化成一个地方
}
