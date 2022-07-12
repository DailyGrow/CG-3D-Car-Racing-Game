
#include "Background.hpp"
#include "ObjLoad.h"
#pragma warning(suppress : 4996)

#define PI 3.1415926

string ObjPath = "./obj/";
ObjLoader obj_body  = ObjLoader(ObjPath+"car_body.obj");
ObjLoader obj_glass = ObjLoader(ObjPath + "car_glass.obj");
ObjLoader obj_other = ObjLoader(ObjPath + "car_other.obj");


float m = 0;
float n = 0;
float frotate = 0.0f;
float right = 0.0f;
int flag = 1;
int W = 0;
int A = 0;
float pointx = 0;
float pointz = 90;
float pointy = 7.0;
float theta = 0.0f;
bool dPressed = false, aPressed = false, wPressed = false, sPressed = false, zPressed = false;
float camera_position[3];
const float PIOVER180 = 0.0174532925f;
float fov = 45.0f;
int wHeight;
int wWidth;
enum VIEW{
	view1_normal=0, view2_god, view3_audi, view4_first
} car_view;
unsigned int car_view_cnt = 0;


void setup()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);//开启光照
	glEnable(GL_DEPTH_TEST); //打开深度测试
	glEnable(GL_TEXTURE_2D);	//启用二维纹理

	GLfloat light0_ambient[] = { 1, 1, 1, 1 };	//环境光
	GLfloat light0_diffuse[] = { 1, 1, 1, 1 };	//漫反射
	GLfloat light0_position[] = { 0, 0, 0, 1 };	//光源的位置

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position); //设置第0号光源的光照位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); //使用环境光Ambient：即被环境漫反射后的光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); //使用漫反射

	glEnable(GL_LIGHT0); //打开光源

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
	}
	else
		cout << "Init glew success "<< endl;
	
	
}
void gettime()
{
	static int frame = 0, time = 0, timebase = 0, tt = 0;
	static char buffer[256];
	char mode[64];
	time = glutGet(GLUT_ELAPSED_TIME);
	strcpy_s(mode, "race!");
	//tt = (time - timebase / 1000);
	tt = tt + 1;
	sprintf_s(buffer, "time: %d s", tt);


	char* c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0, 480, 0, 480, -1, 1);    // 位置正投影
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(220, 440);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);

}

float tmp_angle = 0;
float dist = 40.0f;
void display()
{
	glClearColor(0.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();									// Reset The Current Modelview Matrix恢复初始坐标系

	/* Observation */
	camera_position[1] = pointy;
	camera_position[0] = -pointx;
	camera_position[2] = pointz;
	switch (car_view_cnt)
	{
	case view1_normal: {
		gluLookAt(camera_position[0] + cos(frotate * PIOVER180) * dist,
			camera_position[1] + 10.0f,
			camera_position[2] - sin(frotate * PIOVER180) * dist,
			-pointx, pointy, pointz,
			0.0f, 1.0f, 0.0f);
		break;
	}
	case view2_god: {
		gluLookAt(camera_position[0]+1, camera_position[1] + 200.0f, camera_position[2]+1,
			-pointx, pointy, pointz,
			0.0f, 1.0f, 0.0f);
		break;
	}
	case view3_audi: {
		gluLookAt(10, 200, 400, 0, 0, 0, 0, 1, 0);
		break;
	}
	case view4_first: {
		gluLookAt(camera_position[0] + cos(frotate * PIOVER180) * 20.0f,
			camera_position[1] + 5.0f,
			camera_position[2] - sin(frotate * PIOVER180) * 20.0f,
			-pointx, pointy, pointz,
			0.0f, 1.0f, 0.0f);
		break;
	}

	default:
		gluLookAt(10, 200, 400, 0, 0, 0, 0, 1, 0);
		break;
	}
	
	
	//std::cout << pointx << " " << pointy << " " << pointz << std::endl;

	//std::cout << camera_position[0] << " " << camera_position[1] << " " << camera_position[2] << std::endl;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	
	/* The function is used to draw the background. */
	
	//增加光源的函数，可注释掉
	glEnable(GL_DEPTH_TEST); //使用深度测试
	//glEnable(GL_LIGHTING); //开启光照
	GLfloat mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat white[] = { 1.0,1.0,1.0,1.0 };
	GLfloat yellow[] = { 1.0, 1.0, 0.7, 1.0 }; //环境反射的颜色参数
	GLfloat blue[] = { 1.0, 0.3, 0.7, 1.0 };
	GLfloat moon[] = { 0.5, 0.5, 0.5, 0.5 };
	GLfloat black[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat light_pos0[] = { 0,20,0,1 }; //点光源，第四个参数为1，前三个参数代表光源位置；如果是平行光源，第四个参数是0，前三个参数表示方向。
	GLfloat light_pos1[] = { -10,0,-20,0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0); //设置第0号光源的光照位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, yellow); //使用环境光Ambient：即被环境漫反射后的光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, moon);
	glLightfv(GL_LIGHT0, GL_SPECULAR, moon);

	glEnable(GL_LIGHT0); //开启第0号光源

	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1); //设置第0号光源的光照位置
	glLightfv(GL_LIGHT1, GL_AMBIENT, yellow);

	glEnable(GL_LIGHT1); //开启第1号光源
	
	CreateBackground();
	CreateCar(obj_body, obj_glass, obj_other);

	gettime();
	glutSwapBuffers();
}


void updateView(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);  // select the projection matrix
	glLoadIdentity();

	float wh_ratio = (GLfloat)width / (GLfloat)height;
	gluPerspective(fov, wh_ratio, 0.1, 10000.0);
	glMatrixMode(GL_MODELVIEW);
}


void reshape(int w, int h)
{
	if (h == 0) h = 1;
	wHeight = h;
	wWidth = w;
	updateView(w, h);
}

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'w': {//m = m - 8 * flag;   
		wPressed = true;
		break;
		}
	case 's': {//m = m + 8 * flag;
		sPressed = true;
		break;
		}
	case 'a': {
		aPressed = true;
		break;		
		} //a键左转
	case 'd': {	
		dPressed = true;
		break;
		} //d键右转
	case 'z': {
		zPressed = true;
	}
	case 'v': { // change the view
		car_view_cnt = ++car_view_cnt % 4;
		break;
	}
	case 'r': {
		tmp_angle += 10;
		break;
	}
	case 't': {
		tmp_angle -= 10;
		break;
	}
	case 'c': {
		grab(900, 500);
		break;
	}

	}
	updateView(wWidth, wHeight);
}


void keyUp(unsigned char key, int x, int y)
{
	if (key == 'w')
		wPressed = false;
	if (key == 's')
		sPressed = false;
	if (key == 'a')
		aPressed = false;
	if (key == 'd')
		dPressed = false;
	if (key == 'z')
		zPressed = false;
}


void idle()
{
	if (wPressed) {
		pointx = pointx + 8 * cos(3.14159*frotate / 180.0f);
		pointz = pointz + 8 * sin(3.14169*frotate / 180.0f);
	}
	if (sPressed) {
		pointx = pointx - 8 * cos(3.14159*frotate / 180.0f);
		pointz = pointz - 8 * sin(3.14159*frotate / 180.0f);
	}
	if (aPressed) {
		frotate = frotate + 5.0f;
	}
	if (dPressed) {
		frotate = frotate - 5.0f;
	}
	if (zPressed) {
		pointx = pointx + 18 * cos(3.14159 * frotate / 180.0f);  //z键加速，只能向前加速
		pointz = pointz + 18 * sin(3.14169 * frotate / 180.0f);
	}

	// check collsion
	float xbound = 325 * 25 / 34;
	float ybound = 180 * 125 / 190;

	float xin = 20 * 325 / 34;
	float yin = 65 * 180 / 190;

	float L = 1 * 325 / 34;

	float cx = -15 * 325 / 34;
	float cy = -1.5 * 325 / 34;

	float Ro = 10 * 325 / 34;
	float Ri = 5 * 325 / 34;
	
	if (pointx >= xbound)
		pointx = xbound;
	if (pointx <= -xbound)
		pointx = -xbound;
	if (pointz >= ybound)
		pointz = ybound;
	if (pointz <= -ybound)
		pointz = -ybound;
	if (pointx < cx && pointz < cy && (pointx - cx) * (pointx - cx) + (pointz - cy) * (pointz - cy) >= Ro*Ro)
	{
		pointx = pointx * Ro / sqrt((pointx - cx) * (pointx - cx) + (pointz - cy) * (pointz - cy));
		pointz = pointz * Ro / sqrt((pointx - cx) * (pointx - cx) + (pointz - cy) * (pointz - cy));
	}
	if (pointx < cx && pointz < cy && (pointx - cx) * (pointx - cx) + (pointz - cy) * (pointz - cy) <= Ri * Ri)
	{
		pointx = pointx * Ri / sqrt((pointx - cx) * (pointx - cx) + (pointz - cy) * (pointz - cy));
		pointz = pointz * Ri / sqrt((pointx - cx) * (pointx - cx) + (pointz - cy) * (pointz - cy));
	}
	
	if (pointx > xin && pointz > -yin && pointz < yin && pointx <= xin+L)
	{
		pointx = xin + L;
	} 
	else if (pointz > yin && pointx > -xin && pointx < xin && pointz <= yin + L)
	{
		pointz = yin + L;
	}
	else if (pointz < -yin && pointx > cx && pointx < xin && pointz >= -(yin + L))
	{
		pointz = -(yin + L);
	}
	else if (pointx < -xin && pointz > cy && pointz < yin && pointx >= -(xin + L))
	{
		pointx = -(xin + L);
	}
	glutPostRedisplay();
}

void wheelFunc(int wheel, int direction, int x, int y)
{
	if (direction > 0) {
		fov += 0.5;
	}
	else {
		fov -= 0.5;
	}
	if (fov > 89.0f) fov = 89.0f;
	if (fov < 0.1f) fov = 0.1f;
	updateView(wWidth, wHeight);
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(900, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("car racing");

	setup();
	glutIgnoreKeyRepeat(0);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutMouseWheelFunc(wheelFunc);
	

	Binit();
	glutIdleFunc(idle);
	glutDisplayFunc(display);

	glutMainLoop();
}

