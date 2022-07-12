#include "ObjLoad.h"
#include "Background.hpp"
#pragma comment(lib, "glew32.lib")
#include <GL/glew.h>
#include <gl/glut.h>
using namespace std;

extern float m;
extern float frotate;
extern int flag;
extern float pointx;
extern float pointz;
extern float pointy;
extern float theta;
GLuint vShader, fShader;
GLuint programHandle;

//Load a tower, draw it in void CreateCar();
ObjLoader tower = ObjLoader("./obj/Tower1.obj");

//Create a car
void CreateCar(ObjLoader obj_body, ObjLoader obj_glass, ObjLoader obj_other) {
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);


	//Create a tower
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glScalef(0.005, 0.005, 0.005);
	Material_tower();
	tower.Draw();
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-pointx, pointy, pointz);
	glTranslatef(0.0, -3.3, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glRotatef(frotate, 0, 1, 0);
	glScalef(0.2, 0.2, 0.2);

	Material_body();
	obj_body.Draw();
	Material_glass();
	obj_glass.Draw();
	Material_other();
	obj_other.Draw();


	glPopMatrix();

	//GLfloat mat[] = { 0.0, 0.0, 0.0, 1.0 };
	//GLfloat white[] = { 0.0, 1.0, 1.0, 1.0 }; //环境反射的颜色参数
	//GLfloat moon[] = { 0.5, 0.5, 0.5, 0.5 };
	//GLfloat black[] = { 0.0, 0.0, 0.0, 0.0 };
	//GLfloat light_pos1[] = { -pointx,0,pointz,1 };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos1); //设置第0号光源的光照位置
	//glLightfv(GL_LIGHT0, GL_AMBIENT, white); //使用环境光Ambient：即被环境漫反射后的光
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, moon);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, moon);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_LIGHT1); //开启第0号光源
}


ObjLoader::ObjLoader(string filename)
{
	cout << "Objloader initializing" << endl;
	string line;
	fstream f;
	f.open(filename, ios::in);
	if (!f.is_open()) {
		cout << "Fail to open obj file" << endl;
	}

	while (!f.eof()) {
		getline(f, line);
		vector<string>parameters;
		string tailMark = " ";
		string ans = "";

		line = line.append(tailMark);
		for (int i = 0; i < line.length(); i++) {
			char ch = line[i];
			if (ch != ' ') {
				ans += ch;
			}
			else {
				parameters.push_back(ans);
				ans = "";
			}
		}

		if (parameters[0] == "v") {   //If it's vertex
			vector<GLfloat>Point;
			for (int i = 1; i < 4; i++) {   //push in to vector
				GLfloat xyz = atof(parameters[i].c_str());
				Point.push_back(xyz);
			}
			vSets.push_back(Point);
		}

		else if (parameters[0] == "f") {   //If it's surface
			vector<GLint>vIndexSets;
			for (int i = 1; i < 4; i++) {
				string x = parameters[i];
				string ans = "";
				for (int j = 0; j < x.length(); j++) {
					char ch = x[j];
					if (ch != '/') {
						ans += ch;
					}
					else {
						break;
					}
				}
				GLint index = atof(ans.c_str());
				index = index--;				//obj index start from index
				vIndexSets.push_back(index);	//push the points of a surface
			}
			fSets.push_back(vIndexSets);		//push the surface 
		}
	}
	f.close();

}

void ObjLoader::Draw() {
	//XJH test for material
	//color material 4th para?
	//glEnable(GL_COLOR_MATERIAL);
	//PopMaterial();
	//glUseProgram(programHandle);
	//glUniform3f(glGetUniformLocation(programHandle, "lightColor"), 1.0f, 1.0f, 1.0f);
	//glUniform3f(glGetUniformLocation(programHandle, "material.ambient"), 0.18f, 0.02f, 0.02f);
	//glUniform3f(glGetUniformLocation(programHandle, "material.diffuse"), 0.62f, 0.04f, 0.04f);
	//glUniform3f(glGetUniformLocation(programHandle, "material.specular"), 0.73f, 0.63f, 0.63f);
	//glUniform3f(glGetUniformLocation(programHandle, "material.shininess"), 77.0f, 77.0f, 77.0f);
	//glUniform3f(glGetUniformLocation(programHandle, "light.ambient"), 1.0f, 1.0f, 1.0f);
	//glUniform3f(glGetUniformLocation(programHandle, "light.diffuse"), 1.0f, 1.0f, 1.0f);
	//glUniform3f(glGetUniformLocation(programHandle, "light.specular"), 1.0f, 1.0f, 1.0f);
	//glUniform3f(glGetUniformLocation(programHandle, "lightColor"), 1.0f, 1.0f, 1.0f);

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < fSets.size(); i++) {
		GLfloat VN[3];
		//三个顶点
		GLfloat SV1[3];
		GLfloat SV2[3];
		GLfloat SV3[3];

		if ((fSets[i]).size() != 3) {
			cout << "the fSetsets_Size is not correct" << endl;
		}
		else {
			GLint firstVertexIndex = (fSets[i])[0];
			GLint secondVertexIndex = (fSets[i])[1];
			GLint thirdVertexIndex = (fSets[i])[2];

			SV1[0] = (vSets[firstVertexIndex])[0];   //读完画一个三角形
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];


			GLfloat vec1[3], vec2[3], vec3[3];
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			glNormal3f(VN[0], VN[1], VN[2]);

			glVertex3f(SV1[0], SV1[1], SV1[2]); 
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
		}
	}
	glEnd();
	//glUseProgram(0);
	
	Material_reset();;
	glColor3f(1.0, 1.0, 1.0);
}

void CarInit() {
	//初始化glew扩展库
	//GLenum err = glewInit();
	//if (GLEW_OK != err)
	//{
	//	cout << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
	//}
	initShader("./obj/ivory.vert", "./obj/Shader.frag");
}


void Material_body() {
	float Amb[] = { 0.13,0.0,0.0,0.3 };
	float Dif[] = { 0.8,0.2,0.2,1.0 };
	float Spe[] = { 0.3,0.3,0.3,0.1 };
	float Shine[] = { 0.1 };
	float Emi[] = { 0.0,0.0,0.0,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spe);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shine);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emi);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Material_glass() {
	float Amb[] = { 0.1,0.1,0.1,0.3 };
	float Dif[] = { 0.1,0.1,0.1,0.5 };
	float Spe[] = { 0.7,0.7,0.7,0.8 };
	float Shine[] = { 10.0 };
	float Emi[] = { 0.0,0.0,0.0,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spe);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shine);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emi);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Material_other() {
	float Amb[] = { 0.0,0.0,0.0,0.3 };
	float Dif[] = { 0.1,0.1,0.1,1.0 };
	float Spe[] = { 0.1,0.1,0.1,0.01 };
	float Shine[] = { 1.0 };
	float Emi[] = { 0.0,0.0,0.0,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spe);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shine);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emi);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Material_tower() {
	float Amb[] = { 0.1,0.1,0.1,0.3 };
	float Dif[] = { 0.6,0.6,0.6,1.0 };
	float Spe[] = { 0.1,0.1,0.1,0.1 };
	float Shine[] = { 1.0 };
	float Emi[] = { 0.1,0.1,0.1,0.1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spe);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shine);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emi);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Material_reset() {
	float Amb[] = { 0.2,0.2,0.2,1.0 };
	float Dif[] = { 0.8,0.8,0.8,1.0 };
	float Spe[] = { 0.0,0.0,0.0,1.0 };
	float Shine[] = { 0.0 };
	float Emi[] = { 0.0,0.0,0.0,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spe);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shine);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emi);
	glBlendFunc(GL_ONE, GL_ZERO);
}


char* textFileRead(char* fn)
{
	FILE* fp;
	char* content = NULL;
	int count = 0;
	if (fn != NULL)
	{
		fopen_s(&fp, fn, "r");
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0)
			{
				content = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

void initShader(const char* VShaderFile, const char* FShaderFile)
{
	//1、查看显卡、GLSL和OpenGL的信息    
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << "显卡供应商   : " << vendor << endl;
	cout << "显卡型号     : " << renderer << endl;
	cout << "OpenGL版本   : " << version << endl;
	cout << "GLSL版本     : " << glslVersion << endl;
	//2、编译着色器    
	//创建着色器对象：顶点着色器    
	vShader = glCreateShader(GL_VERTEX_SHADER);
	//错误检测    
	if (0 == vShader)
	{
		cerr << "ERROR : Create vertex shader failed" << endl;
		exit(1);
	}
	//把着色器源代码和着色器对象相关联
	int testcount = 0;
	cout << "point " << testcount++ << endl;
	const char* vShaderCode = textFileRead((char*)VShaderFile);
	cout << "point " << testcount++ << endl;
	const char* vCodeArray[1] = { vShaderCode };
	cout << "point " << testcount++ << endl;
	//将字符数组绑定到对应的着色器对象上  
	glShaderSource(vShader, 1, vCodeArray, NULL);
	cout << "point " << testcount++ << endl;
	//编译着色器对象    
	glCompileShader(vShader);
	cout << "point " << testcount++ << endl;
	//检查编译是否成功    
	GLint compileResult;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度    
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出    
			glGetShaderInfoLog(vShader, logLen, &written, log);
			cerr << "vertex shader compile log : " << endl;
			cerr << log << endl;
			free(log);//释放空间    
		}
	}
	cout << "point " << testcount++ << endl;
	//创建着色器对象：片断着色器    
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//错误检测    
	if (0 == fShader)
	{
		cerr << "ERROR : Create fragment shader failed" << endl;
		exit(1);
	}

	//把着色器源代码和着色器对象相关联    
	cout << "point " << testcount++ << endl;
	const char* fShaderCode = textFileRead((char*)FShaderFile);
	cout << "point " << testcount++ << endl;
	const char* fCodeArray[1] = { fShaderCode };
	glShaderSource(fShader, 1, fCodeArray, NULL);

	//编译着色器对象    
	glCompileShader(fShader);

	//检查编译是否成功    
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度    
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出    
			glGetShaderInfoLog(fShader, logLen, &written, log);
			cerr << "fragment shader compile log : " << endl;
			cerr << log << endl;
			free(log);//释放空间    
		}
	}
	//3、链接着色器对象    
	//创建着色器程序    
	programHandle = glCreateProgram();
	if (!programHandle)
	{
		cerr << "ERROR : create program failed" << endl;
		exit(1);
	}
	//将着色器程序链接到所创建的程序中    
	glAttachShader(programHandle, vShader);
	glAttachShader(programHandle, fShader);
	//将这些对象链接成一个可执行程序    
	glLinkProgram(programHandle);
	//查询链接的结果    
	GLint linkStatus;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
	if (GL_FALSE == linkStatus)
	{
		cerr << "ERROR : link shader program failed" << endl;
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
			&logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen,
				&written, log);
			cerr << "Program log : " << endl;
			cerr << log << endl;
		}
	}
}
