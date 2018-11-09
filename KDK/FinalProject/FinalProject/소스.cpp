#include <GL/freeglut.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include"float3.h"
#include"camera.h"
#define PI 3.14
#define RADIAN PI/180

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid mouseWheel(int, int, int, int);
Camera<float3> m_camera{ 600.f / 800.f };
namespace KDK {
	struct Whale {
		float x, y, z;
		int tail_moveVelocity;
		int moveTailCount;
		int Tail_Radian;
		int tail_moveRange[2];
		GLdouble rotate[16]
			= { 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};
	Whale whale;

}
GLdouble rotate[16]
= { 1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1 };
void init()
{
	m_camera.Initialize(float3{ 0,0,0 }, 300, 1, 1000, 90);
	KDK::whale.tail_moveVelocity = 3;
	KDK::whale.Tail_Radian = -45;
	KDK::whale.x = 60;
	KDK::whale.z = 90;
}
void Keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 'x':
		glPushMatrix();
		glRotatef(5, 1.0, 0.0, 0.0);
		glMultMatrixd(rotate);
		glGetDoublev(GL_MODELVIEW_MATRIX, rotate);
		glPopMatrix();
		break;
	default:
		break;
	}

}
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		glutPostRedisplay();
	}
}
void Motion(int x, int y) {
	static double prev_mx = 0;
	static double prev_my = 0;
	if (x > prev_mx) {
		m_camera.Rotate(0.05, 0);

	}
	else if (x < prev_mx) {
		m_camera.Rotate(-0.05, 0);

	}

	if (y > prev_my) {
		m_camera.Rotate(0, -0.05);

	}
	else if (y < prev_my) {
		m_camera.Rotate(0, 0.05);

	}
	glutPostRedisplay();
	prev_mx = x;
	prev_my = y;
}
void mouseWheel(int button, int dir, int x, int y)
{
	static int distance=300;
	if (dir > 0)
	{
		m_camera.SetDistance(distance-=10);
	}
	else
	{
		m_camera.SetDistance(distance+=10);
	}

	return;
}
void TimerFunction(int value)
{
	if (KDK::whale.tail_moveRange[0] <= 20 && KDK::whale.tail_moveRange[0] >= 0) {
		KDK::whale.tail_moveRange[0] += KDK::whale.tail_moveVelocity;
		KDK::whale.Tail_Radian += KDK::whale.tail_moveVelocity * 4;

	}
	else {
		KDK::whale.tail_moveRange[1] += KDK::whale.tail_moveVelocity;
		if (KDK::whale.tail_moveRange[1] > 30 || KDK::whale.tail_moveRange[1] < 0)
		{
			KDK::whale.tail_moveVelocity *= -1;
			if (KDK::whale.tail_moveVelocity > 0)
				KDK::whale.tail_moveRange[0] = 0;
			else
				KDK::whale.tail_moveRange[0] = 20;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1);
}
void main(int argc, char *argv[])
{
	init();

	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("실습"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutTimerFunc(50, TimerFunction, 1);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMouseWheelFunc(mouseWheel);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void drawRect(float size)
{
	glBegin(GL_QUADS);
	//윗면
	glVertex3f(-size / 2, size / 2, -size / 2); //1
	glVertex3f(-size / 2, size / 2, size / 2);// 2
	glVertex3f(size / 2, size / 2, size / 2);// 3
	glVertex3f(size / 2, size / 2, -size / 2);//4
	//앞면		 
	glVertex3f(-size / 2, size / 2, size / 2);//2
	glVertex3f(-size / 2, -size / 2, size / 2);//6
	glVertex3f(size / 2, -size / 2, size / 2);//7
	glVertex3f(size / 2, size / 2, size / 2);//3
	//오른쪽옆면
	glVertex3f(size / 2, size / 2, size / 2);//3
	glVertex3f(size / 2, -size / 2, size / 2);//7
	glVertex3f(size / 2, -size / 2, -size / 2);//8
	glVertex3f(size / 2, size / 2, -size / 2);//4
	//왼쪽옆면
	glVertex3f(-size / 2, size / 2, -size / 2);//1
	glVertex3f(-size / 2, -size / 2, -size / 2);//5
	glVertex3f(-size / 2, -size / 2, size / 2);//6
	glVertex3f(-size / 2, size / 2, size / 2);//2
	//뒷면
	glVertex3f(size / 2, size / 2, -size / 2);//4
	glVertex3f(size / 2, -size / 2, -size / 2);//8
	glVertex3f(-size / 2, -size / 2, -size / 2);//5
	glVertex3f(-size / 2, size / 2, -size / 2);//1

	//바닥면
	glVertex3f(-size / 2, -size / 2, size / 2);//6
	glVertex3f(-size / 2, -size / 2, -size / 2);//5
	glVertex3f(size / 2, -size / 2, -size / 2);//8
	glVertex3f(size / 2, -size / 2, size / 2);//7

	glEnd();
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
	GLfloat x, y, angle,centerx=0,centery=0,centerz=0;

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
void draw_basic_Whale(float r, float phi, float theta, GLdouble arr[])
{
	float x = r * cos(phi*RADIAN) * cos(theta*RADIAN);
	float y = r * sin(phi*RADIAN);
	float z = r * cos(phi*RADIAN) * sin(theta*RADIAN);
	glPushMatrix(); {
		glTranslatef(x, y, z);
		glPushMatrix(); {
			glRotatef(phi, 0.0, 1.0, 0.0);
			glMultMatrixd(arr);
			glGetDoublev(GL_MODELVIEW_MATRIX, arr);
		}glPopMatrix();
		///////////////////큰 몸통/////////////////////////////////////////////
		glColor3f(1.0, 1.0, 1.0);
		drawRect(100);
		/////////////////////////지느러미 2개//////////////////////////
		glPushMatrix(); {
			glTranslatef(0, -30, 20);
			/****************왼쪽*******************/
			glPushMatrix(); {
				glTranslatef(60, 0, 0);

				glScalef(0.5, 0.5, 1);
				//glColor3f(1.0, 1.0, 1.0);
				drawRect(40);
				glPushMatrix(); {
					glTranslatef(30, 0, 0);

					glScalef(0.7, 0.8, 0.7);
					//glColor3f(1.0, 1.0, 1.0);
					drawRect(40);
				}glPopMatrix();
			}glPopMatrix();
			/***************오른쪽*****************/
			glPushMatrix(); {
				glTranslatef(-60, 0, 0);

				glScalef(0.5, 0.5, 1);
				//glColor3f(1.0, 1.0, 1.0);
				drawRect(40);
				glPushMatrix(); {
					glTranslatef(-30, 0, 0);

					glScalef(0.7, 0.8, 0.7);
					//glColor3f(1.0, 1.0, 1.0);
					drawRect(40);
				}glPopMatrix();
			}glPopMatrix();
		}glPopMatrix();
		/////////////////////////////////////////////////////////////////////
		/**********************중간 몸통***********************************/
		glPushMatrix(); {
			glTranslatef(0, -10 + KDK::whale.tail_moveRange[0], 70);

			glRotatef(0, 0.0, 0.0, 1.0);
			glPushMatrix(); {
				glScalef(1, 1, 0.5);
				//glColor3f(1.0, 1.0, 1.0);
				drawRect(80);
			}glPopMatrix();
			/*****************꼬리 몸통**********************************/
			glPushMatrix(); {
				glTranslatef(0, -10 + KDK::whale.tail_moveRange[1], 40);

				glRotatef(0, 0.0, 0.0, 1.0);
				//glColor3f(1.0, 1.0, 1.0);
				drawRect(40);
				/*************************************************************/
				glPushMatrix(); {
					glTranslatef(0, 0, 20);
					glRotatef(KDK::whale.Tail_Radian, 1.0, 0.0, 0.0);
					glTranslatef(0, 0, 20);
					//////////////////꼬리 지느러미 2개+가운데 1개////////////////// 
					/******************지느러미 왼쪽**********************/
					glPushMatrix(); {
						glTranslatef(20, 0, 0);
						glRotatef(30, 0.0, 1.0, 0.0);
						glScalef(0.7, 0.5, 1);
						//glColor3f(1.0, 1.0, 1.0);
						drawRect(40);
					}glPopMatrix();
					/********************지느러미 오른쪽***************************/
					glPushMatrix(); {
						glTranslatef(-20, 0, 0);
						glRotatef(-30, 0.0, 1.0, 0.0);
						glScalef(0.7, 0.5, 1);
						//glColor3f(1.0, 1.0, 1.0);
						drawRect(40);
					}glPopMatrix();
					/***********************지느러미 가운데**********************/
					glPushMatrix(); {
						glTranslatef(0, 0, -10);
						glScalef(1, 1, 1);
						//glColor3f(1.0, 1.0, 1.0);
						drawRect(20);
					}glPopMatrix();
					/*****************************************************************/
				}glPopMatrix();
			}glPopMatrix();
		}glPopMatrix();
	}glPopMatrix();
}
void draw_Harpoon(float x,float y, float z)
{
	
	glPushMatrix(); {
		glTranslatef(x, y, z);
		glRotatef(0, 0.0, 1.0, 0.0);

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
void draw_Harpoon_Gun(float x, float y, float z)
{

	glPushMatrix(); {
		glTranslatef(x, y, z-5);
		glRotatef(0, 0.0, 1.0, 0.0);// 전체 y축 회전

		glColor3f(1.0, 0.0, 0.0);
		/*********************Y자 베이스 *********************/
		glPushMatrix(); {
			glTranslatef(0, -45, -8);
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
		glRotatef(0, 0.0, 1.0, 0.0);   //포신의 x축 회전

		glColor3f(1.0, 1.0, 0.8);
		glPushMatrix(); {
			/**********************조준쇠******************/
			glPushMatrix(); {
				glColor3f(0.5, 0.5, 0.5);
				glTranslatef(0, 15,-5);
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
				glRotatef(35, 1.0, 0.0, 0.0);
				glScalef(1.0, 1.0, 1.5);
				glRotatef(-45, 1.0, 0.0, 0.0);
				glScalef(1.0, 1.5, 1.2);
				glutSolidCube(16);
			}glPopMatrix();
			/************************************************/
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
				glTranslatef(0, 0,20);
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
GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//////////////////////////////////////////////
	m_camera.LookAt();
	
	glPushMatrix();
	
	/*********drawStart*********************************************/
	//////////////////////고래/////////////////////////////
	draw_basic_Whale(100, 0, 0, rotate);
	//////////////////////////작살//////////////////////////
	draw_Harpoon(0, 0, 0);
	draw_Harpoon_Gun(0,0,0);


	/*************drawEnd*************************************************/
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glFlush();
	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);

	gluPerspective(60.0f, w / h, 1.0, 1000.0);
	glTranslatef(0.0, 0.0, -500.0);
	//glOrtho(-400.0, 400.0, -300.0, 300.0, -300.0, 300.0);   //직각투영

	glMatrixMode(GL_MODELVIEW);
}
