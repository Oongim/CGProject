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

Camera<float3> m_camera{ 600.f / 800.f };

GLdouble rotate[16]
= { 1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1 };

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
void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1);
}
void main(int argc, char *argv[])
{
	m_camera.Initialize(float3{ 0,0,0 }, 300, 1, 1000, 90);
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

GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//////////////////////////////////////////////
	m_camera.LookAt();
	glPushMatrix();
	/*********drawStart*********************************************/
	glPushMatrix(); {
		glTranslatef(0, 0, 0);

		glRotatef(0, 0.0, 0.0, 1.0);

		glColor3f(1.0, 1.0, 0.0);
		drawRect(100);//큰 몸통
		glPushMatrix(); {//지느러미 2개
			glTranslatef(0, -30, 20);
			glPushMatrix(); {
				glTranslatef(60, 0, 0);

				glScalef(0.5, 0.5, 1);
				glColor3f(1.0, 0.0, 0.0);
				drawRect(40);
				glPushMatrix(); {
					glTranslatef(30, 0, 0);

					glScalef(0.7, 0.8, 0.7);
					glColor3f(1.0, 0.0, 1.0);
					drawRect(40);
				}glPopMatrix();
			}glPopMatrix();
			glPushMatrix(); {
				glTranslatef(-60, 0, 0);

				glScalef(0.5, 0.5, 1);
				glColor3f(1.0, 0.0, 0.0);
				drawRect(40);
				glPushMatrix(); {
					glTranslatef(-30, 0, 0);

					glScalef(0.7, 0.8, 0.7);
					glColor3f(1.0, 0.0, 1.0);
					drawRect(40);
				}glPopMatrix();
			}glPopMatrix();
		}glPopMatrix();
		glPushMatrix(); {
			glTranslatef(0, -10, 70);

			glRotatef(0, 0.0, 0.0, 1.0);
			glPushMatrix(); {
				glScalef(1, 1, 0.5);
				glColor3f(0.0, 1.0, 0.0);
				drawRect(80);//중간 몸통
			}glPopMatrix();
			glPushMatrix(); {
				glTranslatef(0, -10, 40);

				glRotatef(0, 0.0, 0.0, 1.0);
				glColor3f(0.0, 0.0, 1.0);
				drawRect(40);//꼬리 몸통
				glPushMatrix(); {
					glTranslatef(0, 0, 40);
					//꼬리
					glPushMatrix(); {
						glTranslatef(20, 0, 0);
						glRotatef(30, 0.0, 1.0, 0.0);
						glScalef(0.7, 0.5, 1);
						glColor3f(1.0, 0.0, 0.0);
						drawRect(40);
					}glPopMatrix();
					glPushMatrix(); {
						glTranslatef(-20, 0, 0);
						glRotatef(-30, 0.0, 1.0, 0.0);
						glScalef(0.7, 0.5, 1);
						glColor3f(1.0, 0.0, 0.0);
						drawRect(40);
					}glPopMatrix();
					glPushMatrix(); {
						glTranslatef(0, 0, -10);
						glScalef(1, 1, 1);
						glColor3f(1.0, 0.0, 0.0);
						drawRect(20);
					}glPopMatrix();
				}glPopMatrix();
			}glPopMatrix();
		}glPopMatrix();
	}glPopMatrix();
	/**************************************************************/
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
