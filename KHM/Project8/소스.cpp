#include <GL/freeglut.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include"float3.h"
#include"camera.h"
#include <iostream>
#define RADIAN PI/180
#define PI 3.141592
using namespace std;

static double prev_mx = 0;
static double prev_my = 0;

namespace KHM 
{
	int MODE_OF_VIEW = 3; // 1 = 1인칭(작살), 3 = 3인칭 
	struct BOAT 
	{
		float HULL_RADIAN = 0;
		float MOVE_RADIAN = 0;
		float x = 0;
		float y = 0;
		float z = 0;
	};
	BOAT Boat;

	struct HARPOON
	{
		float radian = 0;
		float x = 0;
		float y = 100;
		float z = 0;
		float t = 0;
		bool is_fired = false;
	};
	HARPOON Harpoon;
}

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

Camera<float3> m_camera{ 600.f / 800.f };

void Keyboard(unsigned char key, int x, int y);
void Timerfunction(int value);

double Eye_x = 0.0;
double Eye_y = 0.0;
double Eye_z = 900.0;

double Temp_x = 0;
double Temp_y = 0;
double Temp_z = 0;

double Camera_x = 0;
double Camera_y = 0;
double Camera_z = 900.0;



GLdouble rotateWC[16]
////={1,0,0,0,
////  0,1,0,0,
////  0,0,1,0,
////  0,0,0,1};
= { 1,0,0,0,
  0,1,0,0,
  0,0,1,0,
  0,0,0,1 };

//GLdouble rotateWC[9]
//= { 1,0,0,
//0,1,0,
//0,0,1 };
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		glutPostRedisplay();
	}
}
void Motion(int x, int y) 
{

	if (x > prev_mx) 
	{
		m_camera.Rotate(0.05, 0);
		if (KHM::MODE_OF_VIEW == 1)
		{
			KHM::Harpoon.radian -= 2.86472;
		}
	}
	else if (x < prev_mx) 
	{
		m_camera.Rotate(-0.05, 0);
		if (KHM::MODE_OF_VIEW == 1)
		{
			KHM::Harpoon.radian += 2.86472;
		}
	}

	if (y > prev_my)
	{
		m_camera.Rotate(0, -0.05);
	}
	else if (y < prev_my) 
	{
		m_camera.Rotate(0, 0.05);
	}
	glutPostRedisplay();
	prev_mx = x;
	prev_my = y;
}

void MAKE_COORD()
{
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-400, 0, 0);
	glVertex3f(400, 0, 0);
	glEnd();

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, -400, 0);
	glVertex3f(0, 400, 0);
	glEnd();

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, -400);
	glVertex3f(0, 0, 400);
	glEnd();

	glPushMatrix();
	glTranslatef(0, -100, 0);
	glRotatef(90, 1, 0, 0);
	glColor4f(0.25f, 0.85f, 0.92f, 0.0f);
	glRectf(-2500, -2500, 2500, 2500);
	glPopMatrix();
}

void MAKE_BOTTOM()
{
	glPushMatrix();
	glTranslated(0, -2000, 0);
	glColor4f(0.66f, 0.66f, 0.66f, 1.0f);
	glutSolidSphere(1500, 75, 75);
	glPopMatrix();
}
void MAKE_SEA()
{
	glPushMatrix();
	glTranslated(0, -2000, 0);
	glColor4f(0.25f, 0.85f, 0.92f, 0.3f);
	glutSolidSphere(2000, 100, 100);
	glPopMatrix();
}
void MAKE_BOAT()
{
	glPushMatrix();
	glTranslated(KHM::Boat.x, KHM::Boat.y, KHM::Boat.z);
	glRotated(KHM::Boat.HULL_RADIAN, 0, 1, 0);
	glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
	glScalef(1.0f, 1.0f, 2.0f);
	glutSolidCube(100);
	glPopMatrix();
}

void MAKE_HARPOON()
{
	glPushMatrix();
	//glTranslated(0, 100, 0);
	//glTranslated(KHM::Boat.x, KHM::Boat.y, KHM::Boat.z);
	glTranslated(KHM::Harpoon.x, KHM::Harpoon.y, KHM::Harpoon.z);
	glRotated(KHM::Harpoon.radian, 0, 1, 0);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glScalef(0.5f, 0.5f, 2.0f);
	glutSolidCube(30);
	glPopMatrix();
}
void main(int argc, char *argv[])
{
	m_camera.Initialize(float3{ KHM::Boat.x,KHM::Boat.y,KHM::Boat.z }, 900, 1, 9999, 90);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정

	glutCreateWindow("실습16"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);                  // 키보드 입력 콜백 함수
	glutTimerFunc(10, Timerfunction, 1);         // 타이머 함수 설정
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glutMainLoop();
}
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//////////////////////////////////////////////
	m_camera.LookAt();


	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();


	//MAKE_COORD();
	MAKE_BOTTOM();
	MAKE_SEA();
	MAKE_BOAT();
	MAKE_HARPOON();

	glFlush(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, w / h, 1.0, 9999.0);
	//glTranslatef(0.0, 0.0, -900.0);
	gluLookAt(Eye_x, Eye_y, Eye_z, Camera_x, Camera_y, Camera_z - 900, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();



	//glOrtho(-400, 400, -300, 300, -400.0, 400.0);
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		//KHM::Boat.x += sin(KHM::Boat.MOVE_RADIAN) * 3;
		//KHM::Boat.z += cos(KHM::Boat.MOVE_RADIAN) * 3;

		KHM::Boat.x += cos(KHM::Boat.HULL_RADIAN) * 3;
		KHM::Boat.y -= sin(KHM::Boat.HULL_RADIAN) * 3;
		KHM::Boat.z += cos(KHM::Boat.HULL_RADIAN) * 3;
		//KHM::Harpoon.z = KHM::Boat.z;
		//KHM::Harpoon.x = KHM::Boat.x;
		cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
	}
	//X = x0 + 거리 * cos(각도 Z) *   sin(각도 Y)

	//Y = y0 + 거리 * sin(Anglez)

	//Z = z0 + 거리 * cos(각도 Z) *   cos(각도 Y)
	if (key == 'a')
	{
		KHM::Boat.HULL_RADIAN++;
		KHM::Harpoon.radian++;

		cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
	}
	if (key == 's')
	{
		KHM::Boat.z -= cos(KHM::Boat.MOVE_RADIAN) * 3;
		KHM::Boat.x -= sin(KHM::Boat.MOVE_RADIAN) * 3;
		KHM::Harpoon.z = KHM::Boat.z;
		KHM::Harpoon.x = KHM::Boat.x;
		cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
	}
	if (key == 'd')
	{
		KHM::Boat.HULL_RADIAN--;
		KHM::Harpoon.radian--;
		cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
	}
	switch (key)
	{
	case 'q':
		exit(1);
		break;
	case 'i':
		glPushMatrix();
		KHM::Harpoon.y = 100;
		KHM::Harpoon.z = KHM::Boat.z;
		KHM::Harpoon.x = KHM::Boat.x;
		KHM::Harpoon.t = 0;
		KHM::Harpoon.is_fired = false;

		glPopMatrix();
		break;
	//case 'w':
	//	//BOAT_z+=3;
	//	KHM::Boat.z += cos(KHM::Boat.MOVE_RADIAN) * 3;
	//	KHM::Boat.x += sin(KHM::Boat.MOVE_RADIAN) * 3;
	//	cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
	//	break;
	//case 'a':
	//	KHM::Boat.HULL_RADIAN++;
	//	cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
	//	break;
	//case 's':
	//	//BOAT_z-=3;
	//	KHM::Boat.z -= cos(KHM::Boat.MOVE_RADIAN) * 3;
	//	KHM::Boat.x -= sin(KHM::Boat.MOVE_RADIAN) * 3;
	//	cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
	//	break;
	//case 'd':
	//	KHM::Boat.HULL_RADIAN--;
	//	cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
	//	break;
	case 'f':
		//KHM::Harpoon.x;
		KHM::Harpoon.is_fired = true;
		cout << KHM::Boat.x << " " << KHM::Boat.y << " " << KHM::Boat.z << " " << KHM::Boat.HULL_RADIAN << " " << KHM::Boat.MOVE_RADIAN << endl;
		break;
	case '1':
		m_camera.Initialize_radian();
		prev_mx = 0;
		prev_my = 0;
		KHM::MODE_OF_VIEW = 1;
		cout << KHM::MODE_OF_VIEW << endl;
		break;
	case '3':
		KHM::MODE_OF_VIEW = 3;
		cout << KHM::MODE_OF_VIEW << endl;
		break;
	}

	Reshape(800, 600);
	glutPostRedisplay();
}

void Timerfunction(int value)         // 이 함수는 한 번만 실행되므로 지속적인 애니메이션을 위해서는
{   
	// 타이머 함수 내에 타이머를 다시 호출해야 한다.
	if (KHM::Boat.HULL_RADIAN > 360)
	{
		KHM::Boat.HULL_RADIAN = KHM::Boat.HULL_RADIAN - 360;
	}
	KHM::Boat.MOVE_RADIAN = KHM::Boat.HULL_RADIAN;
	KHM::Boat.MOVE_RADIAN = KHM::Boat.MOVE_RADIAN / 180 * PI;
	if (KHM::MODE_OF_VIEW == 3)
	{
		m_camera.Initialize(float3{ KHM::Boat.x,KHM::Boat.y,KHM::Boat.z }, 900, 1, 9999, 90);
	}
	else if (KHM::MODE_OF_VIEW == 1)
	{
		m_camera.Initialize(float3{ KHM::Harpoon.x,KHM::Harpoon.y,KHM::Harpoon.z }, 30, 1, 9999, 90);
		//m_camera.Initialize(float3{ KHM::Harpoon.x,KHM::Boat.y + 100,KHM::Harpoon.z }, 900, 1, 9999, 90);
	}
	if (KHM::Harpoon.is_fired == true)
	{
		if (KHM::Harpoon.y > 0)
		{
			KHM::Harpoon.y -= 0.98;
		}
		KHM::Harpoon.z += 50 * (1 - KHM::Harpoon.t);
		if (KHM::Harpoon.t < 1)
		{
			KHM::Harpoon.t += 0.01;
			//KHM::Harpoon.x += sin(KHM::Boat.MOVE_RADIAN) * 3;
		}
	}
	glutPostRedisplay();                  // 화면 재출력
	glutTimerFunc(10, Timerfunction, 1);      // 타이머함수 재설정
}