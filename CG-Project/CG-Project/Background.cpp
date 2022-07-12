//
//  Background.cpp
//  CGProject
//
//  Created by LI Hongyi on 2019/12/15.
//  Copyright © 2019 LI Hongyi. All rights reserved.
//
#pragma warning(suppress : 4996)
#include "Background.hpp"


#define BMP_Header_Length 54

int depth = 10;

GLuint texGround;
GLuint texBound;
GLuint texOb;
GLuint WriOb;
GLuint texGrass;
GLuint texBuild;

int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}
void grab(int WindowWidth, int WindowHeight)

{

	FILE* pDummyFile;  //指向另一bmp文件，用于复制它的文件头和信息头数据

	FILE* pWritingFile;  //指向要保存截图的bmp文件

	GLubyte* pPixelData;    //指向新的空的内存，用于保存截图bmp文件数据

	GLubyte  BMP_Header[BMP_Header_Length];

	GLint    i, j;

	GLint    PixelDataLength;   //BMP文件数据总长度



	// 计算像素数据的实际长度

	i = WindowWidth * 3;   // 得到每一行的像素数据长度

	while (i % 4 != 0)      // 补充数据，直到i是的倍数

		++i;

	PixelDataLength = i * WindowHeight;  //补齐后的总位数



	// 分配内存和打开文件

	pPixelData = (GLubyte*)malloc(PixelDataLength);

	if (pPixelData == 0)

		exit(0);

	errno_t err1,err2;
	err1 = fopen_s(&pDummyFile, "ROAD.bmp", "rb");
	err2 = fopen_s(&pWritingFile, "record.bmp", "wb");

	std::cout << err1 << " " << err2 << std::endl;

	//把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据
	
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //读取文件头和信息头，占据54字节

	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);

	fseek(pWritingFile, 0x0012, SEEK_SET); //移动到0X0012处，指向图像宽度所在内存

	i = WindowWidth;

	j = WindowHeight;

	fwrite(&i, sizeof(i), 1, pWritingFile);

	fwrite(&j, sizeof(j), 1, pWritingFile);

	

	// 读取当前画板上图像的像素数据

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //设置4位对齐方式

	glReadPixels(0, 0, WindowWidth, WindowHeight,

		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);



	// 写入像素数据

	fseek(pWritingFile, 0, SEEK_END);

	//把完整的BMP文件数据写入pWritingFile

	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);



	// 释放内存和关闭文件

	fclose(pDummyFile);

	fclose(pWritingFile);

	free(pPixelData);
	
}

GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// 打开文件，如果失败，返回
	FILE* pFile;
	errno_t err;
	err = fopen_s(&pFile, file_name, "rb");

	if (pFile == 0)
		return 0;

	// 读取文件中图象的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// 读取像素数据
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);

		if (!power_of_two(width) || !power_of_two(height) || width > max || height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// 计算每行需要的字节数和总字节
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// 分配内存
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// 进行像素缩放

			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}

	}

	// 分配一个新的纹理编号

	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR, GL_UNSIGNED_BYTE, pixels);
	;
	glBindTexture(GL_TEXTURE_2D, 0);

	free(pixels);
	return texture_ID;
}

void Binit()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);    // 启用纹理
	glEnable(GL_BLEND);
	texGrass = load_texture("RGrass.bmp");
	texGround = load_texture("STONE.bmp");  //加载
	texBound = load_texture("ROAD.bmp");
	texOb = load_texture("BLOCK.bmp");
	WriOb = load_texture("Crack.bmp");
	texBuild = load_texture("Start.bmp");
	printf("Texture loading successfully\n");
	/*Color Indentify (Can be modified)*/
	glClearColor(0.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
}

void CreateBackground(void) {

	//Color of background
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	//glLoadIdentity();
	glScalef(0.1, 0.1, 0.1);
	// divided by 10, then the key coords is like that:
	//  before /10 ; xz plane
	//                  (edge: 1250
	//                    1250
	//                      |
	//                     650
	//  -2500 - -2000                2000 - 2500  (edge:2500
	//                    -650
	//                      |
	//                   -1250
	glRotatef(-90, 1, 0, 0);

	/*Sky*/

	/*草地*/
	int dir[4][2] = { {1,1},{1,0},{0,0},{0,1} };

	
	int Step = 20;
	for (size_t i = 0; i < 6800 / Step; i++)
	{
		for (size_t j = 0; j < 3800 / Step; j++)
		{
			int X = -3400 + Step * i;
			int Y = -1900 + Step * j;
			bool Round = true;
			if (X > 1500 && Y > 150 && (X-1500)*(X-1500)+(Y-150)*(Y-150)>500*500 && (X - 1500) * (X - 1500) + (Y - 150) * (Y - 150) < 1000 * 1000)
			{
				Round = false;
			}
			if (X < 1900 && X > -1900 && Y < 600 && Y > -600 && Round)
			{
				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texGrass);
			}
			else {
				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texGround);
			}
			glPushMatrix();
			glBegin(GL_POLYGON);
			//glColor3f(0.13, 0.54, 0.13);
			
			glTexCoord2iv(dir[2]);
			glVertex3d(X, Y, -depth);
			glTexCoord2iv(dir[1]);
			glVertex3d(X + Step, Y, -depth);
			glTexCoord2iv(dir[0]);
			glVertex3d(X + Step, Y + Step, -depth);
			glTexCoord2iv(dir[3]);
			glVertex3d(X, Y + Step, -depth);
			glEnd();
			glPopMatrix();

			glDisable(GL_TEXTURE_2D);
		}
	}
	

	/*赛道 -----*/
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texBound);

	Step = 100;
	for (size_t i = 0; i < 3500 / Step; i++)
	{
		for (size_t j = 0; j < 500 / Step; j++)
		{
			int X = -2000 + Step * i;
			int Y = 700 + Step * j;
			
			glPushMatrix();
			glBegin(GL_POLYGON);
			glTexCoord2iv(dir[2]);
			glVertex3d(X, Y, 0);
			glTexCoord2iv(dir[1]);
			glVertex3d(X + Step, Y, 0);
			glTexCoord2iv(dir[0]);
			glVertex3d(X + Step, Y + Step, 0);
			glTexCoord2iv(dir[3]);
			glVertex3d(X, Y + Step, 0);
			glEnd();
			glPopMatrix();
		}
	}
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	for (size_t i = 0; i < 4000 / Step; i++)
	{
		for (size_t j = 0; j < 500 / Step; j++)
		{
			int X = -2000 + Step * i;
			int Y = -1200 + Step * j;
			if (X < -300 && X > -600)
			{
				glBindTexture(GL_TEXTURE_2D, texBuild);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, texBound);
			}
			glPushMatrix();
			glBegin(GL_POLYGON);
			glTexCoord2iv(dir[2]);
			glVertex3d(X, Y, 0);
			glTexCoord2iv(dir[1]);
			glVertex3d(X + Step, Y, 0);
			glTexCoord2iv(dir[0]);
			glVertex3d(X + Step, Y + Step, 0);
			glTexCoord2iv(dir[3]);
			glVertex3d(X, Y + Step, 0);
			glEnd();
			glPopMatrix();
		}
	}
	glDisable(GL_TEXTURE_2D);

	/*赛道 |||||||||*/
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texBound);
	Step = 25;
	for (size_t i = 0; i < 500 / Step; i++)
	{
		for (size_t j = 0; j < 2400 / Step; j++)
		{
			glPushMatrix();
			glBegin(GL_POLYGON);
			int X = -2500 + Step * i;
			int Y = -1200 + Step * j;
			glTexCoord2iv(dir[2]);
			glVertex3d(X, Y, 0);
			glTexCoord2iv(dir[1]);
			glVertex3d(X + Step, Y, 0);
			glTexCoord2iv(dir[0]);
			glVertex3d(X + Step, Y + Step, 0);
			glTexCoord2iv(dir[3]);
			glVertex3d(X, Y + Step, 0);
			glEnd();
			glPopMatrix();
		}
	}
	for (size_t i = 0; i < 500 / Step; i++)
	{
		for (size_t j = 0; j < 1400 / Step; j++)
		{
			glPushMatrix();
			glBegin(GL_POLYGON);
			int X = 2000 + Step * i;
			int Y = -1200 + Step * j;
			glTexCoord2iv(dir[2]);
			glVertex3d(X, Y, 0);
			glTexCoord2iv(dir[1]);
			glVertex3d(X + Step, Y, 0);
			glTexCoord2iv(dir[0]);
			glVertex3d(X + Step, Y + Step, 0);
			glTexCoord2iv(dir[3]);
			glVertex3d(X, Y + Step, 0);
			glEnd();
			glPopMatrix();
		}
	}

	/*弯道*/
	for (size_t i = 0; i < 1000 / Step; i++)
	{
		for (size_t j = 0; j < 1000 / Step; j++) {
			
			int X = 1500 + Step * i;
			int Y = 150 + Step * j;
			int Rsq = (Step * i) * (Step * i) + (Step * j) * (Step * j);
			if (Rsq < 1000*1000 && Rsq > 500*500)
			{
				glPushMatrix();
				glBegin(GL_POLYGON);
				glTexCoord2iv(dir[2]);
				glVertex3d(X, Y, 0);
				glTexCoord2iv(dir[1]);
				glVertex3d(X + Step, Y, 0);
				glTexCoord2iv(dir[0]);
				glVertex3d(X + Step, Y + Step, 0);
				glTexCoord2iv(dir[3]);
				glVertex3d(X, Y + Step, 0);
				glEnd();
				glPopMatrix();
			}
			else
			{
				if (Rsq >= 1000*1000 && Step * (i - 1) * Step * (i - 1) + (Step * j) * (Step * j) < 1000 * 1000)
				{
					SetObt(X + 50, Y);
				}
				else if (Rsq <= 500 * 500 && Step * (i + 1) * Step * (i + 1) + (Step * j) * (Step * j) > 500 * 500)
				{
					SetObt(X - 50, Y);
				}
				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texBound);
				//glEnable(GL_BLEND);
			}
		}
	}
	glDisable(GL_TEXTURE_2D);

	/*glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texGround);
	glDisable(GL_BLEND);

	float ctrlpoints[4][4][3] = {
		{{1000.0,2700.0,0.0},//p00***********************   
		{1050.0,1800.0,200.0},//p01   
		{1100.0,1600.0,400.0},//p02   
		{1550.0,1000.0,0.0}},//p03***************************   
		{{1800.0,2000.0,500.0},//p10   
		{1900.0,1300.0,800.0},//p11   
		{2000.0,1100.0,600.0},//p12   
		{2400.0,700.0,400.0}},//p13   
		{{3100.0,2000.0,600.0},//p20   
		{3200.0,1300.0,200.0},//p21   
		{3300.0,1100.0,700.0},//p22   
		{3700.0,700.0,400.0}},//p23   
		{{4200.0,2700.0,0.0},//p30********************************   
		{4300.0,1800.0,400.0},//p31   
		{4400.0,1600.0,600.0},//p32   
		{4900.0,1000.0,0.0}}//p33***********************   
	};

	glPushMatrix();
	//绘制控制点与控制线
	glScaled(1.0, 0.5, 1.0);
	glTranslatef(-3100, 0, 0);

	GLUnurbs* pNurb = gluNewNurbsRenderer();

	//绘制B样条控制曲面
	GLfloat knots[8] = { 0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0 }; //B样条控制向量
	glLineWidth(1.0f);
	glColor3f(0.0, 0.0, 0.0);

	//gluNurbsProperty(pNurb, GLU_SAMPLING_TOLERANCE, 25.0); //设置属性
	gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
	gluBeginSurface(pNurb);//开始绘制
	gluNurbsSurface(pNurb,
		8, knots,
		8, knots,
		4 * 3,
		3,
		&ctrlpoints[0][0][0],
		4, 4,
		GL_MAP2_VERTEX_3);

	gluEndSurface(pNurb); //结束绘制

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);*/


	for (int i = -2000; i < 2000; i+=100)
	{
		SetObt(i + 50, -700 + 50);
		if (i <= 1500)
		{
			SetObt(i + 50, 700 - 50);
		}
	}
	for (int i = -2500; i < 2500; i += 100)
	{
		SetObt(i + 50, -1200 - 50);
		if (i <= 1500)
		{
			SetObt(i + 50, 1200 + 50);
		}
		if (i >= 1500 && i <= 1700 )
		{
			SetObt(i + 50, 1200);
		}

	}
	for (int i = -1200; i <= 1300; i += 100)
	{
		if (i <= 400)
		{
			SetObt(2500 + 50, i - 50);
		}
		
		SetObt(-2500-50, i - 50);
	}
	for (int i = -600; i <= 700; i += 100)
	{
		if (i <= 300)
		{
			SetObt(2000 - 50, i - 50);
		}
		
		SetObt(-2000 + 50, i - 50);
	}
	glPopMatrix();
}

void SetObt(int ofx, int ofy) {

	glDisable(GL_BLEND);
	
	int dir[4][2] = { {1,1},{1,0},{0,0},{0,1} };
	int Step = 100;
	int Z = 15;
	int X = -50 + ofx;
	int Y = -50 + ofy; 
	
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texOb);
	glPushMatrix();

	//BASE
	
	glBegin(GL_POLYGON);
	glTexCoord2iv(dir[2]);
	glVertex3d(X, Y, Z);
	glTexCoord2iv(dir[1]);
	glVertex3d(X + Step, Y, Z);
	glTexCoord2iv(dir[0]);
	glVertex3d(X + Step, Y + Step, Z);
	glTexCoord2iv(dir[3]);
	glVertex3d(X, Y + Step, Z);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2iv(dir[2]);
	glVertex3d(X, Y, 0);
	glTexCoord2iv(dir[1]);
	glVertex3d(X, Y+Step, 0);
	glTexCoord2iv(dir[0]);
	glVertex3d(X, Y + Step, Z);
	glTexCoord2iv(dir[3]);
	glVertex3d(X, Y, Z);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2iv(dir[2]);
	glVertex3d(X+Step, Y, 0);
	glTexCoord2iv(dir[1]);
	glVertex3d(X + Step, Y + Step, 0);
	glTexCoord2iv(dir[0]);
	glVertex3d(X + Step, Y + Step, Z);
	glTexCoord2iv(dir[3]);
	glVertex3d(X + Step, Y, Z);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2iv(dir[2]);
	glVertex3d(X, Y + Step, 0);
	glTexCoord2iv(dir[1]);
	glVertex3d(X + Step, Y + Step, 0);
	glTexCoord2iv(dir[0]);
	glVertex3d(X + Step, Y + Step, Z);
	glTexCoord2iv(dir[3]);
	glVertex3d(X, Y + Step, Z);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2iv(dir[2]);
	glVertex3d(X ,Y, 0);
	glTexCoord2iv(dir[1]);
	glVertex3d(X + Step, Y, 0);
	glTexCoord2iv(dir[0]);
	glVertex3d(X + Step, Y, Z);
	glTexCoord2iv(dir[3]);
	glVertex3d(X, Y, Z);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
}