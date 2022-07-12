//
//  Background.hpp
//  CGProject
//
//  Created by LI Hongyi on 2019/12/15.
//  Copyright © 2019 LI Hongyi. All rights reserved.
//

#ifndef Background_hpp
#define Background_hpp

#include <cmath>
#include <iostream>
#if defined(WIN32)
#  include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "glew32.lib")
#pragma warning(suppress : 4996)
#include <GL/glew.h>
#define NDEBUG
#include "GL\freeglut.h"
//#include <SOIL.h>
#include"SOIL.h"
#include <string.h>

#pragma warning(suppress : 4996)
#define BITMAP_ID 0x4D42
void Binit();
GLuint load_texture(const char* file_name);
void CreateBackground(void);
void SetObt(int ofx, int ofy);
void grab(int WindowWidth, int WindowHeight);
#endif /* Background_hpp */


