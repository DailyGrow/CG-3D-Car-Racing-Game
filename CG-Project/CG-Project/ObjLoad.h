#ifndef OBJLOAD_H
#define OBJLOAD_H

#include <string>
#define NDEBUG
//#include "GL\freeglut.h"
#include <GL/glew.h>
#include <gl/glut.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


//Load a obj file
class ObjLoader {
public:
	ObjLoader(string filename);
	void Draw(); 
private:
	vector<vector<GLfloat>>vSets;		//��Ŷ���(x,y,z)����
	vector<vector<GLint>>fSets;			//������������������
};

//Create a car
void CreateCar(ObjLoader obj_body, ObjLoader obj_glass, ObjLoader obj_other);

//Initialize car shader
void CarInit();

//Material for each part
void Material_body();
void Material_glass();
void Material_other();
void Material_tower();
void Material_reset();

char* textFileRead(char* fn);
void initShader(const char* VShaderFile, const char* FShaderFile);

#endif