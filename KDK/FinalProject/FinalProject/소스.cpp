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
#define DISTANCE 500.0
enum Direction { LEFT = -1, KEEP, RIGHT };
using namespace std;

static double prev_mx = 0;
static double prev_my = 0;


namespace KHM
{
	int MODE_OF_VIEW = 3; // 1 = 1인칭(작살), 3 = 3인칭 
	float temp_distance = 500;
	float temp_x_angle = 0;
	float temp_y_angle = 0;
	struct BOAT
	{
		bool is_forward = false;
		int direction = KEEP;
		bool is_breaking = false;
		float speed = 0.0;
		float velocity = 0.0002;
	};
	BOAT Boat;

	struct HARPOON
	{
		float y_angle = 0;
		float x_angle = 0;
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
	void Insert_Harpoon()
	{
		Node *New = new Node;
		New->Harpoon.x = 0;
		New->Harpoon.y = 150;
		New->Harpoon.z = 0;
		New->Harpoon.x_angle = KHM::temp_x_angle;
		New->Harpoon.y_angle = KHM::temp_y_angle;
		New->Harpoon.is_hit = false;
		New->Harpoon.is_fired = false;

		New->next = Head->next;
		Head->next = New;
	}
	void Delete_Harpoon(Node * key)
	{
		Node*curr = Head->next;
		Node*prev = Head;
		while (curr != nullptr)
		{
			if (key == curr)
			{
				prev->next = curr->next;
				delete curr;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	void shot_Harpoon()
	{
		Node *curr = Head->next;
		KHM::temp_x_angle = curr->Harpoon.x_angle;
		KHM::temp_y_angle = curr->Harpoon.y_angle;
		curr->Harpoon.is_fired = true;
		Insert_Harpoon();
	}
	void move_Harpoon()
	{
		Node* curr = Head->next;
		while (curr != nullptr)
		{
			if (curr->Harpoon.is_fired)
			{
				if (curr->Harpoon.y > 0)
				{
					curr->Harpoon.y -= 0.98;
				}
				curr->Harpoon.z += 50 * (1 - curr->Harpoon.t);
				if (curr->Harpoon.t < 1)
				{
					curr->Harpoon.t += 0.01;
					//KHM::Harpoon.x += sin(KHM::Boat.MOVE_RADIAN) * 3;
				}


				//여기 어딘가에 타겟 맞췄을 때 is_hit  true로 바꾸는거
				//맵의 범위를 넘었을 때 삭제
			}

			curr = curr->next;
		}
	}
	void draw_loaded_Harpoon()
	{
		Node* curr = Head->next;
		glPushMatrix(); {
			glTranslatef(curr->Harpoon.x, curr->Harpoon.y, curr->Harpoon.z);
			glRotatef(curr->Harpoon.y_angle, 0.0, 1.0, 0.0);
			glRotatef(curr->Harpoon.x_angle, 1.0, 0.0, 0.0);
			glScalef(1.0, 1.0, 1.0);

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
		}glPopMatrix();
	}
	void draw_moving_Harpoon()
	{
		Node* curr = Head->next->next;
		while (curr != nullptr)
		{
			glPushMatrix(); {
				glRotatef(curr->Harpoon.y_angle, 0.0, 1.0, 0.0);
				glRotatef(curr->Harpoon.x_angle, 1.0, 0.0, 0.0);
				glPushMatrix(); {
					//glRotated(curr->Harpoon.y_angle, 0, 1, 0);
					glTranslatef(curr->Harpoon.x, curr->Harpoon.y, curr->Harpoon.z);

					glScalef(1.0, 1.0, 1.0);

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
				}glPopMatrix();
			}glPopMatrix();
			curr = curr->next;
		}
	}
}

void drawCylinder(GLfloat radius, GLfloat h)
{
	/* function createCyliner()
	원기둥의 중심 x,y,z좌표, 반지름, 높이를 받아 원기둥을 생성하는 함수(+z방향으로 원에서 늘어남)
	centerx : 원기둥 원의 중심 x좌표
	centery : 원기둥 원의 중심 y좌표
	centerz : 원기둥 원의 중심 z좌표
	radius : 원기둥의 반지름
	h : 원기둥의 높이
	*/
	GLfloat x, y, angle, centerx = 0, centery = 0, centerz = 0;

	glBegin(GL_TRIANGLE_FAN);           //원기둥의 윗면
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(centerx, centery, centerz);

	for (angle = 0.0f; angle < (2.0f*PI); angle += (PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(x, y, centerz);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);            //원기둥의 옆면
	for (angle = 0.0f; angle < (2.0f*PI); angle += (PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(sin(angle), cos(angle), 0.0f);
		glVertex3f(x, y, centerz);
		glVertex3f(x, y, centerz + h);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);           //원기둥의 밑면
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(centerx, centery, centerz + h);
	for (angle = (2.0f*PI); angle > 0.0f; angle -= (PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(x, y, centerz + h);
	}
	glEnd();
}

void draw_Harpoon_Gun(float x, float y, float z, float x_angle, float y_angle)
{

	glPushMatrix(); {
		glTranslatef(x, y, z);
		glRotatef(y_angle, 0.0, 1.0, 0.0);// 전체 y축 회전
		glTranslatef(0, 0, -10);
		glColor3f(1.0, 0.0, 0.0);
		/*********************Y자 베이스 *********************/
		glPushMatrix(); {
			glTranslatef(0, -45, 0);
			glPushMatrix(); {
				glRotatef(90, 1.0, 0.0, 0.0);
				drawCylinder(15, 10);//바닥 원기둥
			}glPopMatrix();
			/***************y자 몸통*******************/
			glPushMatrix(); {
				glColor3f(1.0, 0.0, 0.0);
				glTranslatef(0, 5, 0);
				glPushMatrix(); {
					glScalef(1.5, 2.0, 1.0);
					glColor3f(1.0, 0.0, 0.0);
					glutSolidCube(10);
					glColor3f(1.0, 1.0, 0.0);
					glutWireCube(10);
				}glPopMatrix();
				glPushMatrix(); {
					glPushMatrix(); {//    /
						glTranslatef(3, 0, 0);
						glRotatef(70, 0.0, 0.0, 1.0);
						glTranslatef(10, 0, 0);
						glScalef(2.0, 0.5, 1.0);

						glColor3f(1.0, 0.0, 0.0);
						glutSolidCube(10);
						glColor3f(1.0, 1.0, 0.0);
						glutWireCube(10);
					}glPopMatrix();
					glPushMatrix(); {
						glTranslatef(10, 28, 0);
						glScalef(0.5, 2.0, 1.0);

						glColor3f(1.0, 0.0, 0.0);
						glutSolidCube(10);
						glColor3f(1.0, 1.0, 0.0);
						glutWireCube(10);
					}glPopMatrix();
				}glPopMatrix();
				glPushMatrix(); {/*     \                                                      */
					glPushMatrix(); {//    /
						glTranslatef(-3, 0, 0);
						glRotatef(-70, 0.0, 0.0, 1.0);
						glTranslatef(-10, 0, 0);
						glScalef(2.0, 0.5, 1.0);

						glColor3f(1.0, 0.0, 0.0);
						glutSolidCube(10);
						glColor3f(1.0, 1.0, 0.0);
						glutWireCube(10);
					}glPopMatrix();
					glPushMatrix(); {
						glTranslatef(-10, 28, 0);
						glScalef(0.5, 2.0, 1.0);

						glColor3f(1.0, 0.0, 0.0);
						glutSolidCube(10);
						glColor3f(1.0, 1.0, 0.0);
						glutWireCube(10);
					}glPopMatrix();
				}glPopMatrix();
			}glPopMatrix();
		}glPopMatrix();
		/*******************//*포신*//********************/
		glRotatef(x_angle, 1.0, 0.0, 0.0);   //포신의 x축 회전

		glColor3f(1.0, 0.0, 1.0);
		glPushMatrix(); {
			glTranslatef(0, 0, 10);
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
		}glPopMatrix();
		glTranslatef(0, 0, 8);
		glColor3f(1.0, 1.0, 0.8);
		glPushMatrix(); {
			/**********************조준쇠******************/
			glPushMatrix(); {
				glColor3f(0.5, 0.5, 0.5);
				glTranslatef(0, 15, -5);
				glPushMatrix(); {
					glColor3f(0.3, 0.3, 0.3);
					glTranslatef(0, -3, -15);
					glScalef(1.0, 5, 1);
					glutSolidCube(2);
				}glPopMatrix();
				glPushMatrix(); {
					glColor3f(0.3, 0.3, 0.3);
					glTranslatef(0, -4, 10);
					glScalef(1.0, 4, 1);
					glutSolidCube(2);
				}glPopMatrix();
				glColor3f(0.5, 0.5, 0.5);
				glRotatef(5, 1.0, 0.0, 0.0);
				glScalef(1.0, 2, 40);
				glutSolidCube(1);
			}glPopMatrix();
			/*********************네모몸통*******************************/
			glPushMatrix(); {
				glColor3f(1.0, 1.0, 0.7);
				glTranslatef(0, -6, -15);
				glPushMatrix(); {
					glRotatef(35, 1.0, 0.0, 0.0);
					glScalef(1.0, 1.0, 1.5);
					glRotatef(-45, 1.0, 0.0, 0.0);
					glScalef(1.0, 1.5, 1.2);
					glutSolidCube(16);
				}glPopMatrix();

				/*************손잡이*****************/
				glPushMatrix(); {
					glColor3f(0.4, 0.4, 0.4);
					glTranslatef(0, 0, -25);
					glPushMatrix(); {
						glTranslatef(0, 0, -10);
						drawCylinder(2, 10);
					}glPopMatrix();
					glScalef(4, 5, 3);
					glutSolidCube(1);
				}glPopMatrix();
				//
				glPushMatrix(); {
					glColor3f(0.5, 0.5, 0.5);
					glTranslatef(8, 0, -10);
					glPushMatrix(); {
						glScalef(1, 6, 15);
						glutSolidCube(1);
					}glPopMatrix();
					glPushMatrix(); {
						glColor3f(0.3, 0.3, 0.3);
						glTranslatef(0, 0, -7);
						glRotatef(40, 0.0, 1.0, 0.0);
						glTranslatef(0, 0, -5);
						glScalef(1, 4, 10);
						glutSolidCube(1);
					}glPopMatrix();
				}glPopMatrix();
				glPushMatrix(); {
					glColor3f(0.5, 0.5, 0.5);
					glTranslatef(-8, 0, -10);
					glPushMatrix(); {
						glScalef(1, 6, 15);
						glutSolidCube(1);
					}glPopMatrix();
					glPushMatrix(); {
						glColor3f(0.3, 0.3, 0.3);
						glTranslatef(0, 0, -7);
						glRotatef(-40, 0.0, 1.0, 0.0);
						glTranslatef(0, 0, -5);
						glScalef(1, 4, 10);
						glutSolidCube(1);
					}glPopMatrix();
				}glPopMatrix();
			}glPopMatrix();
			/********************** 포신 원통**************************/
			glPushMatrix(); {
				glTranslatef(0, 0, -20);
				glColor3f(1.0, 0.8, 0.8);
				drawCylinder(8, 30);

			}glPopMatrix();
			glPushMatrix(); {
				glTranslatef(0, -14, -13);
				glColor3f(1.0, 0.8, 0.8);
				drawCylinder(6, 25);
				glColor3f(0.8, 0.8, 1.0);
				glTranslatef(0, 0, 20);
				drawCylinder(4, 10);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(1.0, 1.0, 0.8);
				glTranslatef(0, 0, 15);
				glScalef(1.0, 1.0, 10.0);
				glutSolidTorus(1, 4, 20, 20);
			}glPopMatrix();
		}glPopMatrix();
	}glPopMatrix();
}

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

Camera<float3> m_camera{ 768.f / 1024.f };

void Keyboard(unsigned char key, int x, int y);
void UpKeyboard(unsigned char key, int x, int y);

void Timerfunction(int value);

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
void mouseWheel(int button, int dir, int x, int y)
{
	static float distance = KHM::temp_distance;
	KHM::temp_distance = distance;
	if (dir > 0)
	{
		m_camera.SetDistance(distance -= 10);
	}
	else if (distance < 500)
	{
		m_camera.SetDistance(distance += 10);
	}
	return;
}
void Motion(int x, int y)
{

	if (x > prev_mx)
	{
		if (KHM::MODE_OF_VIEW == 1)
		{
			if (KHM::Head->next->Harpoon.y_angle > -28.6472)
			{
				KHM::Head->next->Harpoon.y_angle -= 2.86472;
			}
		}
		else
		{
			m_camera.Rotate(0.05, 0);
		}
	}
	else if (x < prev_mx)
	{
		if (KHM::MODE_OF_VIEW == 1)
		{
			if (KHM::Head->next->Harpoon.y_angle < 28.6472)
			{
				KHM::Head->next->Harpoon.y_angle += 2.86472;
			}
		}
		else
		{
			m_camera.Rotate(-0.05, 0);
		}
	}

	if (y > prev_my)
	{
		if (KHM::MODE_OF_VIEW == 1)
		{
			if (KHM::Head->next->Harpoon.x_angle < 28.6472)
			{
				KHM::Head->next->Harpoon.x_angle += 2.86472;
			}
		}
		else
		{
			m_camera.Rotate(0, -0.05);
		}
	}
	else if (y < prev_my)
	{
		if (KHM::MODE_OF_VIEW == 1)
		{
			if (KHM::Head->next->Harpoon.x_angle > -28.6472)
			{
				KHM::Head->next->Harpoon.x_angle -= 2.86472;
			}
		}
		else
		{
			m_camera.Rotate(0, 0.05);
		}
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
	glutWireSphere(WORLD_SCALE - 500, 75, 75);
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
	glPushMatrix(); {
		glPushMatrix(); {//와이드 공간
			glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
			glScalef(1.0f, 1.0f, 2.0f);
			glutWireCube(100);
		}glPopMatrix();
		float hight= 120;
		// 3차원 상의 제어점 설정
		GLfloat left_ctrlpoints[3][4][3] = {
		{{0.0, hight, 100.0},{50.0, hight+30, 40.0},{50.0, 50.0, 40.0},{50.0, 50.0, -100.0}},
		{{0.0, 0.0, 90.0},{40.0, -25.0, 40.0},{40.0, -25.0, 0.0},{50.0, -25.0, -100.0}},
		{{0.0, -50.0, 60.0},{0.0, -50.0, 40.0},{0.0, -50.0, 0.0},{0.0, -50.0, -100.0}}
		};
		GLfloat right_ctrlpoints[3][4][3] = {
		{{0.0, hight, 100.0},{-50.0, hight+30, 40.0},{-50.0, 50.0, 40.0},{-50.0, 50.0, -100.0}},
		{{0.0, 0.0, 90.0},{-40.0, -25.0, 40.0},{-40.0, -25.0, 0.0},{-50.0, -25.0, -100.0}},
		{{0.0, -50.0, 60.0},{0.0, -50.0, 40.0},{0.0, -50.0, 0.0},{0.0, -50.0, -100.0}}
		};
		GLfloat deck_ctrlpoints[3][4][3] = {
			{{0.0, hight-10, 100.0},{-50.0, hight+20, 40.0},{-50.0, 40.0, 40.0},{-50.0, 40.0, -100.0}},
			{{0.0, hight - 10, 100.0},{0.0, hight, 40.0},{0.0, 40.0, 40.0},{0.0, 40.0, -100.0}},
			{{0.0, hight - 10, 100.0},{50.0, hight , 40.0},{50.0, 40.0, 40.0},{50.0, 40.0, -100.0}}
		};
		GLfloat back_ctrlpoints[3][3][3] = {
			{{50.0, 50.0, -100.0},{50.0, -25.0, -100.0},{0.0, -50.0, -100.0}},
			{{0.0, 50.0, -100.0},{0.0, -25.0, -100.0},{0.0, -50, -100.0}},
			{{-50.0, 50.0, -100.0},{-50.0, -25.0, -100.0},{0.0, -50.0, -100.0}}
		};
		glTranslated(0, 0, 0);
		glPushMatrix(); {
			// 곡면 제어점 설정
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &back_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// 그리드를 이용한 곡면 드로잉
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// 선을 이용하여 그리드 연결
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glPointSize(5.0); glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_POINTS);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					glVertex3fv(back_ctrlpoints[i][j]);
			glEnd();
		}glPopMatrix();
		glPushMatrix(); {
			// 곡면 제어점 설정
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &deck_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// 그리드를 이용한 곡면 드로잉
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// 선을 이용하여 그리드 연결
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glPointSize(5.0); glColor3f(1.0, 0.0, 1.0);
			glBegin(GL_POINTS);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 4; j++)
					glVertex3fv(deck_ctrlpoints[i][j]);
			glEnd();
		}glPopMatrix();

		glPushMatrix(); {
			// 곡면 제어점 설정
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &left_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// 그리드를 이용한 곡면 드로잉
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// 선을 이용하여 그리드 연결
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glPointSize(5.0); glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POINTS);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 4; j++)
					glVertex3fv(left_ctrlpoints[i][j]);
			glEnd();
		}glPopMatrix();
		glPushMatrix(); {
			// 곡면 제어점 설정
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &right_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// 그리드를 이용한 곡면 드로잉
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// 선을 이용하여 그리드 연결
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glPointSize(5.0); glColor3f(0.0, 1.0, 1.0);
			glBegin(GL_POINTS);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 4; j++)
					glVertex3fv(right_ctrlpoints[i][j]);
			glEnd();
		}glPopMatrix();


	}glPopMatrix();
}

void initialize()
{
	KHM::Head = new KHM::Node;
	KHM::Head->next = nullptr;
	KHM::Insert_Harpoon();
}
void main(int argc, char *argv[])
{
	initialize();
	m_camera.Initialize(float3{ 0,0,0 }, DISTANCE, 1, 9999, 90);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(1024, 768); // 윈도우의 크기 지정
	glutCreateWindow("포경"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMouseWheelFunc(mouseWheel);
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

	draw_BOAT();
	draw_Harpoon_Gun(0, 150, 0, KHM::Head->next->Harpoon.x_angle, KHM::Head->next->Harpoon.y_angle);
	glPushMatrix(); {
		glTranslated(0, +150, 0);
		glMultMatrixd(rotateWC);
		glTranslated(0, -150, 0);
		KHM::draw_moving_Harpoon();
	}glPopMatrix();
	glPushMatrix(); {
		glTranslated(0, -WORLD_SCALE, 0);
		glMultMatrixd(rotateWC);

		glPushMatrix(); {
			glTranslated(0, WORLD_SCALE, 0);
		}glPopMatrix();


		draw_BOTTOM();
		draw_SEA();
	}glPopMatrix();
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
		if (KHM::MODE_OF_VIEW == 1)
		{
			KHM::shot_Harpoon();
		}
		break;
	case '1':
		m_camera.Initialize_radian();
		prev_mx = 0;
		prev_my = 0;
		KHM::MODE_OF_VIEW = 1;
		cout << KHM::MODE_OF_VIEW << endl;
		break;
	case '3':
		KHM::Head->next->Harpoon.x_angle = 0;
		KHM::Head->next->Harpoon.y_angle = 0;
		m_camera.Initialize(float3{ 0,0,0 }, KHM::temp_distance, 1, 9999, 90);
		KHM::MODE_OF_VIEW = 3;
		cout << KHM::MODE_OF_VIEW << endl;
		break;
	}

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
	//if (KHM::MODE_OF_VIEW == 3)
	//{
	//	//m_camera.Initialize(float3{ KHM::Harpoon.x,KHM::Boat.y,KHM::Boat.z }, DISTANCE, 1, 9999, 90);
	//}
	if (KHM::MODE_OF_VIEW == 1)
	{
		m_camera.Initialize(float3{ 0,185,0 }, 30, 1, 9999, 90);
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
			glRotatef(-(KHM::Boat.speed * 5 + 0.1), 0.0, 1.0, 0.0);
		}
		else if (KHM::Boat.direction == RIGHT)
		{
			glRotatef((KHM::Boat.speed * 5 + 0.1), 0.0, 1.0, 0.0);

		}
		glMultMatrixd(rotateWC);
		glGetDoublev(GL_MODELVIEW_MATRIX, rotateWC);
	}glPopMatrix();

	glPushMatrix(); {
		KHM::move_Harpoon();
	}glPopMatrix();
	glutPostRedisplay();                  // 화면 재출력
	glutTimerFunc(10, Timerfunction, 1);      // 타이머함수 재설정
}