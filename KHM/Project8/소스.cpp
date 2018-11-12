#include <GL/freeglut.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include"float3.h"
#include"camera.h"
#include <iostream>
#define PI 3.141592
#define RADIAN PI/180
#define WORLD_SCALE 4000
enum Direction{LEFT = -1 , KEEP, RIGHT};
using namespace std;

static double prev_mx = 0;
static double prev_my = 0;


namespace KHM 
{
	int MODE_OF_VIEW = 3; // 1 = 1인칭(작살), 3 = 3인칭 
	struct BOAT 
	{
		bool is_forward = false;
		int direction = KEEP;
		bool is_breaking = false;
		float HULL_RADIAN = 0;
		float MOVE_RADIAN = 0;
		float speed = 0.0;
		float velocity = 0.0002;
	};
	BOAT Boat;

	struct HARPOON
	{
		float HULL_RADIAN = 0;
		float x = 0;
		float y = 0;
		float z = 0;
		float t = 0;
		bool is_hit = false; 
		bool is_fired = false;
	};
	struct Node
	{
		HARPOON Harpoon;
		Node * next;
	};
	Node * Head;
	void Insert()
	{
		Node *New = new Node;
		
		New->Harpoon.x = 0;
		New->Harpoon.y = 0;
		New->Harpoon.z = 0;
		New->Harpoon.HULL_RADIAN = 0;
		New->Harpoon.is_hit = false;
		New->Harpoon.is_fired = false;
		New->next = nullptr;
		Node *curr = Head;
		while (Head->next != nullptr)
		{
			curr = curr->next;
		}
		curr->next = New;
	}
	void shot_Harpoon()
	{
		Node *curr = Head;
		while (Head->next != nullptr)
		{
			curr = curr->next;
		}
		curr->Harpoon.is_fired = true;
	}
}

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

Camera<float3> m_camera{ 600.f / 800.f };

void Keyboard(unsigned char key, int x, int y);
void UpKeyboard(unsigned char key, int x, int y);

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
= { 1,0,0,0,
  0,1,0,0,
  0,0,1,0,
  0,0,0,1 };


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
			KHM::Harpoon.HULL_RADIAN -= 2.86472;
		}
	}
	else if (x < prev_mx) 
	{
		m_camera.Rotate(-0.05, 0);
		if (KHM::MODE_OF_VIEW == 1)
		{
			KHM::Harpoon.HULL_RADIAN += 2.86472;
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

void draw_COORD()
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

void draw_BOTTOM()
{
	glPushMatrix();
	glColor4f(0.66f, 0.66f, 0.66f, 1.0f);
	glutWireSphere(1500, 75, 75);
	glPopMatrix();
}
void draw_SEA()
{
	glPushMatrix();
	glColor4f(0.25f, 0.85f, 0.92f, 0.3f);
	glutWireSphere(WORLD_SCALE, 100, 100);
	glPopMatrix();
}
void draw_BOAT()
{
	glPushMatrix();
	//glTranslated(KHM::Boat.x, KHM::Boat.y, KHM::Boat.z);
	//glRotated(KHM::Boat.phi, 0, 1, 0); 
	glRotated(KHM::Boat.HULL_RADIAN , 0, 1, 0); //배 방향
	glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
	glScalef(1.0f, 1.0f, 2.0f);
	glutSolidCube(100);
	glPopMatrix();
}

void draw_HARPOON()
{
	glPushMatrix();
	//glTranslated(KHM::Harpoon.x, KHM::Harpoon.y + 100, KHM::Harpoon.z);

	glColor3f(1.0, 0.0, 1.0);
	/*********************작살 봉*********************/
	glPushMatrix(); {
		glScalef(1.0, 1.0, 50.0);           //봉길이 50
		glutSolidTorus(0.5, 1, 20, 20);
	}glPopMatrix();
	/*********************작살 촉*********************/
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix(); {
		glTranslatef(0, 0, 25);
		glScalef(1.0, 1.0, 1.0);
		glutSolidCone(3, 5, 10, 10);
		glTranslatef(0, 0, 3);
		glutSolidCone(3, 5, 10, 10);
		glTranslatef(0, 0, 3);
		glutSolidCone(3, 5, 10, 10);
	}glPopMatrix();

	glPopMatrix();
}

void initialize()
{
	KHM::Head->next = nullptr;
}
void main(int argc, char *argv[])
{
	initialize();
	m_camera.Initialize(float3{ 0,0,0 }, 1200, 1, 9999, 90);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("포경"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);                  // 키보드 입력 콜백 함수
	glutKeyboardUpFunc(UpKeyboard);                  // 키보드 입력 콜백 함수
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

	//draw_COORD();
	draw_BOAT();
	glPushMatrix();
	glTranslated(0, -WORLD_SCALE, 0);
	glMultMatrixd(rotateWC);

	glPushMatrix();
	glTranslated(0, WORLD_SCALE, 0);
	glutSolidCube(100);
	glPopMatrix();

	//draw_HARPOON();
	draw_BOTTOM();
	draw_SEA();
	glPopMatrix();
	glFlush(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();



	//glOrtho(-400, 400, -300, 300, -400.0, 400.0);
}

void Keyboard(unsigned char key, int x, int y)
{
	int mod = 0;

	switch (key)
	{
	case 'q':
		exit(1);
		break;
	case 'i':
		glPushMatrix();
		glPopMatrix();
		break;
	case 'w':
		KHM::Boat.is_forward = true;
		mod = glutGetModifiers();
		if (mod == 'a')
		{
			glPushMatrix();
			glRotatef(-KHM::Boat.speed, 0.0, 1.0, 0.0);
			glMultMatrixd(rotateWC);
			glGetDoublev(GL_MODELVIEW_MATRIX, rotateWC);
			glPopMatrix();
		}
		else if (mod == 'd')
		{
			glPushMatrix();
			glRotatef(KHM::Boat.speed, 0.0, 1.0, 0.0);
			glMultMatrixd(rotateWC);
			glGetDoublev(GL_MODELVIEW_MATRIX, rotateWC);
			glPopMatrix();
		}		
		
		break;
	case 'a':
		KHM::Boat.direction = LEFT;
		mod = glutGetModifiers();
		if (mod == 'w')
		{
			KHM::Boat.is_forward = true;
		}
		break;
	case 's':
		if (KHM::Boat.speed > 0.0)
		{
			KHM::Boat.is_breaking = true;
		}
		break;
	case 'd':
		KHM::Boat.direction = RIGHT;
		mod = glutGetModifiers();
		if (mod == 'w')
		{
			KHM::Boat.is_forward = true;
		}
		break;
	case 'f':
		shot_Harpoon();
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

void UpKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		KHM::Boat.is_forward = false;
		break;
	case 'a':
		KHM::Boat.direction = KEEP;
		break;
	case 'd':
		KHM::Boat.direction = KEEP;
		break;
	case 's':
		KHM::Boat.is_breaking = false;
		break;
	}
}

void Timerfunction(int value) 
{   
	if (KHM::Boat.HULL_RADIAN > 360)
	{
		KHM::Boat.HULL_RADIAN = KHM::Boat.HULL_RADIAN - 360;
	}
	KHM::Boat.MOVE_RADIAN = KHM::Boat.HULL_RADIAN;
	KHM::Boat.MOVE_RADIAN = KHM::Boat.MOVE_RADIAN * RADIAN;
	if (KHM::MODE_OF_VIEW == 3)
	{
		//m_camera.Initialize(float3{ KHM::Boat.x,KHM::Boat.y,KHM::Boat.z }, 1800, 1, 9999, 90);
	}
	else if (KHM::MODE_OF_VIEW == 1)
	{
		m_camera.Initialize(float3{ KHM::Harpoon.x,KHM::Harpoon.y,KHM::Harpoon.z }, 30, 1, 9999, 90);
	}
	curr
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
	glPushMatrix(); {
		if (KHM::Boat.is_forward == true)
		{
			if (KHM::Boat.speed < 0.1)
			{
				KHM::Boat.speed += KHM::Boat.velocity;
			}
		}
		else
		{
			if (KHM::Boat.speed > 0.0)
			{
				KHM::Boat.speed -= KHM::Boat.velocity * 2;
				if (KHM::Boat.is_breaking == true)
				{
					KHM::Boat.speed -= KHM::Boat.velocity * 3;
				}
				if (KHM::Boat.speed <= 0)
				{
					KHM::Boat.speed = 0;
				}
			}

		}
		glRotatef(-KHM::Boat.speed, 1.0, 0.0, 0.0);
		glMultMatrixd(rotateWC);
		glGetDoublev(GL_MODELVIEW_MATRIX, rotateWC);
	}glPopMatrix();
	glPushMatrix(); {
		if (KHM::Boat.direction == LEFT)
		{
			glRotatef(-(KHM::Boat.speed * 3 + 0.3), 0.0, 1.0, 0.0);
		}
		else if (KHM::Boat.direction == RIGHT)
		{
			glRotatef((KHM::Boat.speed * 3 + 0.3), 0.0, 1.0, 0.0);

		}
		glMultMatrixd(rotateWC);
		glGetDoublev(GL_MODELVIEW_MATRIX, rotateWC);
	}glPopMatrix();
	glutPostRedisplay();                  // 화면 재출력
	glutTimerFunc(10, Timerfunction, 1);      // 타이머함수 재설정
}