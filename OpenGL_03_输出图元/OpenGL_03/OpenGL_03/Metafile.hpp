//
//  Metafile.hpp
//  OpenGL_03
//
//  Created by 先锋电子技术 on 16/6/23.
//  Copyright © 2016年 uwuneng. All rights reserved.
//

#ifndef Metafile_hpp
#define Metafile_hpp

/// opengl 图元
#include <stdio.h>

//画直线
void drawLine(void);
//画折线



class Metafile {

typedef void (*Func)(void);
public:
    //画直线
    void drawLine(void){
        
    };
    //画折线
    void call(Func fun){  // 类内部接口
//        (this.*fun)();
        fun();
    }
};



#endif /* Metafile_hpp */
