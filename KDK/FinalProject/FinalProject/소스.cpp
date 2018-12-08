//#pragma comment(lib."winmm")
//#include<mmsystem.h>
//#include<Digitalv.h>
#include <GL/freeglut.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include"float3.h"
#include"camera.h"
#include <iostream>
#include <algorithm>
#define PI 3.141592
#define RADIAN PI / 180
#define WORLD_SCALE 4000
#define DISTANCE 500.0
#define HARPOON_Y 165
#define HARPOON_Z 70
#define SOUND_BGM "../Resource/sound/BGM.wav"
enum Direction { LEFT = -1, KEEP, RIGHT };
using namespace std;

static double prev_mx = 0;
static double prev_my = 0;
int windowW, windowH;
GLfloat AmbientLight[] = { 0.0f, 1.0f, 0.0f, 1.0f }; // �������
GLfloat DiffuseLight[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // ��������
GLfloat SpecularLight[] = { 1.0, 1.0, 1.0, 1.0 }; // �������
GLfloat lightPos[] = { 100.0, 100.0, 200.0, 1.0 }; // ��ġ: (10, 5, 20)

GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const char *shot_string[] = {
	"NOT READY" ,"CHARGING","READY"
};
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

Camera<float3> m_camera{ 768.f / 1024.f };

void Keyboard(unsigned char key, int x, int y);
void UpKeyboard(unsigned char key, int x, int y);

void Timerfunction(int value);
GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	// ���̳ʸ� �б� ���� ������ ����
	//if ((fp = fopen(filename, "rb")) == NULL)
	fopen_s(&fp, filename, "rb");
	if (fp == NULL)
		return NULL;
	// ��Ʈ�� ���� ����� �д´�.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	// ������ BMP �������� Ȯ���Ѵ�.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER ��ġ�� ����.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// ��Ʈ�� �̹��� �����͸� ���� �޸� �Ҵ��� �Ѵ�.
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// ��Ʈ�� ���� ����� �д´�.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// ��Ʈ���� ũ�� ����
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount + 7) / 8.0 * abs((*info)->bmiHeader.biHeight);
	// ��Ʈ���� ũ�⸸ŭ �޸𸮸� �Ҵ��Ѵ�.
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// ��Ʈ�� �����͸� bit(GLubyte Ÿ��)�� �����Ѵ�.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bits;
}
GLubyte *pBytes; // �����͸� ����ų ������
BITMAPINFO *info; // ��Ʈ�� ��� ������ ����
GLuint textures[4];
GLdouble rotateWC[16]
= { 1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1 };


namespace KDK {
	float boat_hight = 120;
	GLfloat left_ctrlpoints[3][4][3] = {
		{ { 0.0, boat_hight, 100.0 },{ 50.0, boat_hight + 30, 40.0 },{ 50.0, 50.0, 40.0 },{ 50.0, 50.0, -100.0 } },
	{ { 0.0, 0.0, 90.0 },{ 40.0, -25.0, 40.0 },{ 40.0, -25.0, 0.0 },{ 50.0, -25.0, -100.0 } },
	{ { 0.0, -50.0, 60.0 },{ 0.0, -50.0, 40.0 },{ 0.0, -50.0, 0.0 },{ 0.0, -50.0, -100.0 } }
	};
	GLfloat right_ctrlpoints[3][4][3] = {
		{ { 0.0, boat_hight, 100.0 },{ -50.0, boat_hight + 30, 40.0 },{ -50.0, 50.0, 40.0 },{ -50.0, 50.0, -100.0 } },
	{ { 0.0, 0.0, 90.0 },{ -40.0, -25.0, 40.0 },{ -40.0, -25.0, 0.0 },{ -50.0, -25.0, -100.0 } },
	{ { 0.0, -50.0, 60.0 },{ 0.0, -50.0, 40.0 },{ 0.0, -50.0, 0.0 },{ 0.0, -50.0, -100.0 } }
	};
	GLfloat deck_ctrlpoints[3][4][3] = {
		{ { 0.0, boat_hight - 10, 100.0 },{ -50.0, boat_hight + 20, 40.0 },{ -50.0, 40.0, 40.0 },{ -50.0, 40.0, -100.0 } },
	{ { 0.0, boat_hight - 10, 100.0 },{ 0.0, boat_hight, 40.0 },{ 0.0, 40.0, 40.0 },{ 0.0, 40.0, -100.0 } },
	{ { 0.0, boat_hight - 10, 100.0 },{ 50.0, boat_hight , 40.0 },{ 50.0, 40.0, 40.0 },{ 50.0, 40.0, -100.0 } }
	};
	GLfloat back_ctrlpoints[3][3][3] = {
		{ { 50.0, 50.0, -100.0 },{ 50.0, -25.0, -100.0 },{ 0.0, -50.0, -100.0 } },
	{ { 0.0, 50.0, -100.0 },{ 0.0, -25.0, -100.0 },{ 0.0, -50, -100.0 } },
	{ { -50.0, 50.0, -100.0 },{ -50.0, -25.0, -100.0 },{ 0.0, -50.0, -100.0 } }
	};
	/////////////////////////////////�ĵ�/////////////////////////////////////////////////////
	const int number_of_wave_pivots = 8;
	const int wave_speed = 1;
	GLfloat wave_pivots[number_of_wave_pivots][number_of_wave_pivots][3];
	int wave_velocity[number_of_wave_pivots][number_of_wave_pivots] = { 0 };
	void init_wave_pivots()
	{
		for (int i = 0; i < number_of_wave_pivots; ++i)
		{
			for (int j = 0; j < number_of_wave_pivots; ++j) {
				wave_pivots[i][j][0] = ((WORLD_SCALE * 2) / (number_of_wave_pivots - 1)) * i - WORLD_SCALE;
				wave_pivots[i][j][2] = ((WORLD_SCALE * 2) / (number_of_wave_pivots - 1)) * j - WORLD_SCALE;
				if (i == 0 && j == 0 || i == number_of_wave_pivots - 1 && j == number_of_wave_pivots - 1 ||
					i == 0 && j == number_of_wave_pivots - 1 || i == number_of_wave_pivots - 1 && j == 0)
				{

					wave_pivots[i][j][1] = 0;
					wave_velocity[i][j] = 0;
				}
				else
				{

					wave_pivots[i][j][1] = rand() % 200 - 100;
					if (rand() % 2)
						wave_velocity[i][j] = wave_speed;
					else
						wave_velocity[i][j] = -wave_speed;
				}
			}
		}
	}
	void update_wave()
	{
		for (int i = 0; i < number_of_wave_pivots; ++i)
		{
			for (int j = 0; j < number_of_wave_pivots; ++j) {
				wave_pivots[i][j][1] += wave_velocity[i][j];
				if (wave_pivots[i][j][1] > 100 || wave_pivots[i][j][1] < -100)
				{
					wave_velocity[i][j] *= -1;
				}
			}
		}
	}
	void draw_wave()
	{
		glPushMatrix(); {
			// ��� ������ ����
			glColor4f(0.0, 0.0, 1.0, 0.5f);
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, number_of_wave_pivots, 0.0, 1.0, number_of_wave_pivots * 3, number_of_wave_pivots, &wave_pivots[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// �׸��带 �̿��� ��� �����
			glMapGrid2f(number_of_wave_pivots, 0.0, 1.0, number_of_wave_pivots, 0.0, 1.0);
			// ���� �̿��Ͽ� �׸��� ����
			//glEvalMesh2(GL_LINE, 0, number_of_wave_pivots, 0, number_of_wave_pivots);
			glEvalMesh2(GL_FILL, 0, number_of_wave_pivots, 0, number_of_wave_pivots);
			/*glPointSize(10.0);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_POINTS);
			for (int i = 0; i < number_of_wave_pivots; i++)
				for (int j = 0; j < number_of_wave_pivots; j++)
					glVertex3fv(wave_pivots[i][j]);
			glEnd();*/

		}glPopMatrix();
	}
	///////////////////////////////��////////////////////////////////////////////////////////
	const int SEAWEED_NUM = 40;
	GLdouble random_seaweed[SEAWEED_NUM][3];
	void init_seaweed_postion()
	{
		for (int i = 0; i < SEAWEED_NUM; ++i) {
			random_seaweed[i][1] = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
			random_seaweed[i][0] = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
			random_seaweed[i][2] = rand() % 90;
		}
	}
	void draw_pyramid(float size)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(0, 0, 1);
		glVertex3f(0, size / 2, 0);//2
		glVertex3f(-size / 2, -size / 2, size / 2);//6
		glVertex3f(size / 2, -size / 2, size / 2);//7
		glNormal3f(1, 0, 0);
		glVertex3f(0, size / 2, 0);//2
		glVertex3f(size / 2, -size / 2, size / 2);//7
		glVertex3f(size / 2, -size / 2, -size / 2);//8
		glNormal3f(-1, 0, 0);
		glVertex3f(0, size / 2, 0);//2
		glVertex3f(-size / 2, -size / 2, -size / 2);//5
		glVertex3f(-size / 2, -size / 2, size / 2);//6
		glNormal3f(0, 0, -1);
		glVertex3f(0, size / 2, 0);//2
		glVertex3f(size / 2, -size / 2, -size / 2);//8
		glVertex3f(-size / 2, -size / 2, -size / 2);//5
		glEnd();
	}
	void draw_Seaweed()
	{
		glTranslated(0, -300, 0);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			//����
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-15, 600, -15); //1
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-15, 600, 15);// 2
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(15, 600, 15);// 3
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(15, 600, -15);//4
									 //�ո�		 
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-15, 600, 15);//2
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-15, -600, 15);//6
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(15, -600, 15);//7
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(15, 600, 15);//3
									//�����ʿ���
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(15, 600, 15);//3
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(15, -600, 15);//7
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(15, -600, -15);//8
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(15, 600, -15);//4
									 //���ʿ���
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-15, 600, -15);//1
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-15, -600, -15);//5
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-15, -600, 15);//6
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-15, 600, 15);//2
									 //�޸�
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(15, 600, -15);//4
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(15, -600, -15);//8
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-15, -600, -15);//5
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-15, 600, -15);//1

			glEnd();

		}glPopMatrix();
	}
	void draw_stone()
	{
		glTranslated(0, 00, 0);
		glScaled(1, 0.5, 0.8);
		glutSolidSphere(100, 20, 20);
	}
	void draw_MAP_object()
	{
		glPushMatrix(); {
			glTranslated(0, -500, 0);
			/**************************************��?*************************************************************/
			glPushMatrix(); {
				glColor3f(1, 0, 0.6);
				glTranslated(-2000, 0, 800);
				draw_pyramid(2000);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(1, 0.7, 0.6);

				glTranslated(-1000, 0, -2500);
				draw_pyramid(2000);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(0.3, 0.7, 0.6);

				glTranslated(2500, 0, 100);
				draw_pyramid(2000);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(0.2, 0.2, 0.2);

				glTranslated(+1000, 0, 3000);
				draw_pyramid(2000);
			}glPopMatrix();
			/************************************����************************************************************/

			// �ؽ�ó ���� Ȱ��ȭ
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[0]);

			glColor4f(0.2, 0.2, 0.2, 0.5);
			for (int k = 0; k < SEAWEED_NUM; ++k) {
				glPushMatrix(); {
					glTranslated(random_seaweed[k][0], 0, random_seaweed[k][1]);
					for (int i = 0; i < 3; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							glPushMatrix(); {
								glRotated(random_seaweed[k][2], 0, 1, 0);
								glTranslated(50 * i, 0, 50 * j);

								draw_Seaweed();
							}glPopMatrix();
						}
					}
				}glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
			/*************************************��******************************************************/
			glPushMatrix(); {
				glColor3f(0, 0, 0);
				//glRotated(-90, 1, 0, 0);
				glTranslated(0, WORLD_SCALE, 0);
				draw_stone();
			}glPopMatrix();

		}glPopMatrix();
	}
	////////////////////////////////��///////////////////////////////////////////////////////
	void drawRect(float size, float x, float y, float z)
	{
		glPushMatrix(); {
			glTranslatef(x, y, z);
			glBegin(GL_QUADS);
			//����
			glVertex3f(-size / 2, size / 2, -size / 2); //1
			glVertex3f(-size / 2, size / 2, size / 2);// 2
			glVertex3f(size / 2, size / 2, size / 2);// 3
			glVertex3f(size / 2, size / 2, -size / 2);//4
													  //�ո�		 
			glVertex3f(-size / 2, size / 2, size / 2);//2
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(size / 2, -size / 2, size / 2);//7
			glVertex3f(size / 2, size / 2, size / 2);//3
													 //�����ʿ���
			glVertex3f(size / 2, size / 2, size / 2);//3
			glVertex3f(size / 2, -size / 2, size / 2);//7
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(size / 2, size / 2, -size / 2);//4
													  //���ʿ���
			glVertex3f(-size / 2, size / 2, -size / 2);//1
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(-size / 2, size / 2, size / 2);//2
													  //�޸�
			glVertex3f(size / 2, size / 2, -size / 2);//4
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(-size / 2, size / 2, -size / 2);//1

													   //�ٴڸ�
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(size / 2, -size / 2, size / 2);//7

			glEnd();
		}glPopMatrix();
	}
	void draw_face_Rect(float size)
	{

		glPushMatrix(); {
			glBegin(GL_QUADS);
			//����
			glVertex3f(-size / 2, size / 2, -size / 2); //1
			glVertex3f(-size / 2, size / 2, size / 2);// 2
			glVertex3f(size / 2, size / 2, size / 2);// 3
			glVertex3f(size / 2, size / 2, -size / 2);//4
													  //�ո�	
			glVertex3f(-size / 2, size / 2, size / 2);//2
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(size / 2, -size / 2, size / 2);//7
			glVertex3f(size / 2, size / 2, size / 2);//3
													 //�����ʿ���
			glVertex3f(size / 2, size / 2, size / 2);//3
			glVertex3f(size / 2, -size / 2, size / 2);//7
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(size / 2, size / 2, -size / 2);//4
													  //���ʿ���
			glVertex3f(-size / 2, size / 2, -size / 2);//1
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(-size / 2, size / 2, size / 2);//2
													  //�޸�
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(size / 2, size / 2, -size / 2);//4
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-size / 2, size / 2, -size / 2);//1

													   //�ٴڸ�
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(size / 2, -size / 2, size / 2);//7

			glEnd();
		}glPopMatrix();

	}
	struct Whale {
		float x, y, z;
		float next_x, next_y, next_z;
		int move_Velocity;
		int tail_moveVelocity;
		int moveTailCount;
		int Tail_Radian;
		int tail_moveRange[2];
		float phi, theta;
		int HP;
		bool isRun_away;
		bool isAlive;
		bool isDeath_animation;
	};
	Whale whale[10];

	void draw_basic_Whale(float x, float y, float z, int i)
	{
		glPushMatrix(); {
			glTranslatef(x, y, z);
			if (whale[i].isDeath_animation) {
				glRotated(180, 0, 0, 1);
			}
			else
			{
				glRotatef(whale[i].phi + 180, 0.0, 1.0, 0.0);

				glRotatef(-whale[i].theta, 1.0, 0.0, 0.0);
			}
			///////////////////ū ����/////////////////////////////////////////////
			glColor3f(1.0, 1.0, 1.0);
			draw_face_Rect(100);
			/////////////////////////�������� 2��//////////////////////////
			glPushMatrix(); {
				glTranslatef(0, -30, 20);
				/****************����*******************/
				glPushMatrix(); {
					glTranslatef(60, 0, 0);

					glScalef(0.5, 0.5, 1);
					//glColor3f(1.0, 1.0, 1.0);
					drawRect(40, 0, 0, 0);
					glPushMatrix(); {
						glTranslatef(30, 0, 0);

						glScalef(0.7, 0.8, 0.7);
						//glColor3f(1.0, 1.0, 1.0);
						drawRect(40, 0, 0, 0);
					}glPopMatrix();
				}glPopMatrix();
				/***************������*****************/
				glPushMatrix(); {
					glTranslatef(-60, 0, 0);

					glScalef(0.5, 0.5, 1);
					//glColor3f(1.0, 1.0, 1.0);
					drawRect(40, 0, 0, 0);
					glPushMatrix(); {
						glTranslatef(-30, 0, 0);

						glScalef(0.7, 0.8, 0.7);
						//glColor3f(1.0, 1.0, 1.0);
						drawRect(40, 0, 0, 0);
					}glPopMatrix();
				}glPopMatrix();
			}glPopMatrix();
			/////////////////////////////////////////////////////////////////////
			/**********************�߰� ����***********************************/
			glPushMatrix(); {
				glTranslatef(0, -10 + KDK::whale[i].tail_moveRange[0], 70);

				glRotatef(0, 0.0, 0.0, 1.0);
				glPushMatrix(); {
					glScalef(1, 1, 0.5);
					//glColor3f(1.0, 1.0, 1.0);
					drawRect(80, 0, 0, 0);
				}glPopMatrix();
				/*****************���� ����**********************************/
				glPushMatrix(); {
					glTranslatef(0, -10 + KDK::whale[i].tail_moveRange[1], 40);

					glRotatef(0, 0.0, 0.0, 1.0);
					//glColor3f(1.0, 1.0, 1.0);
					drawRect(40, 0, 0, 0);
					/*************************************************************/
					glPushMatrix(); {
						glTranslatef(0, 0, 20);
						glRotatef(KDK::whale[i].Tail_Radian, 1.0, 0.0, 0.0);
						glTranslatef(0, 0, 20);
						//////////////////���� �������� 2��+��� 1��////////////////// 
						/******************�������� ����**********************/
						glPushMatrix(); {
							glTranslatef(20, 0, 0);
							glRotatef(30, 0.0, 1.0, 0.0);
							glScalef(0.7, 0.5, 1);
							//glColor3f(1.0, 1.0, 1.0);
							drawRect(40, 0, 0, 0);
						}glPopMatrix();
						/********************�������� ������***************************/
						glPushMatrix(); {
							glTranslatef(-20, 0, 0);
							glRotatef(-30, 0.0, 1.0, 0.0);
							glScalef(0.7, 0.5, 1);
							//glColor3f(1.0, 1.0, 1.0);
							drawRect(40, 0, 0, 0);
						}glPopMatrix();
						/***********************�������� ���**********************/
						glPushMatrix(); {
							glTranslatef(0, 0, -10);
							glScalef(1, 1, 1);
							//glColor3f(1.0, 1.0, 1.0);
							drawRect(20, 0, 0, 0);
						}glPopMatrix();
						/*****************************************************************/
					}glPopMatrix();
				}glPopMatrix();
			}glPopMatrix();
		}glPopMatrix();
	}
	bool collide_pyramid(float x1, float y1, float z1, float x2, float z2, float size)
	{
		if (x1 - 100 < x2 + size * -y1 / size && x1 + 100 > x2 - size * -y1 / size &&
			z1 + 100 > z2 - size * -y1 / size && z1 - 100 < z2 + size * -y1 / size)
		{
			return 1;
		}
		return 0;
	}
	void init_Whale()
	{
		for (int i = 0; i < 10; ++i) {
			whale[i].HP = 100;
			whale[i].tail_moveVelocity = 3;
			whale[i].move_Velocity = 3;
			whale[i].Tail_Radian = -45;
			whale[i].x = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
			whale[i].z = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
			whale[i].y = -rand() % 800;
			whale[i].isAlive = true;
			whale[i].isDeath_animation = false;
			while ((collide_pyramid(whale[i].x, whale[i].y, whale[i].z, -2000, 800, 1000) ||
				collide_pyramid(whale[i].x, whale[i].y, whale[i].z, -1000, -2500, 1000) ||
				collide_pyramid(whale[i].x, whale[i].y, whale[i].z, 2500, 100, 1000) ||
				collide_pyramid(whale[i].x, whale[i].y, whale[i].z, 1000, 3000, 1000)))
			{
				whale[i].x = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
				whale[i].z = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
				whale[i].y = -rand() % 800;
			}

			whale[i].next_x = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
			whale[i].next_z = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
			whale[i].next_y = -rand() % 800;
		}
	}
	void draw_Whale()
	{
		glEnable(GL_TEXTURE_2D);

		for (int i = 0; i < 10; ++i) {
			if (whale[i].isAlive) {
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				draw_basic_Whale(whale[i].x, whale[i].y, whale[i].z, i);
			}
			else if (whale[i].isDeath_animation) {
				glBindTexture(GL_TEXTURE_2D, textures[3]);

				draw_basic_Whale(whale[i].x, whale[i].y, whale[i].z, i);
			}
		}
		glDisable(GL_TEXTURE_2D);
	}
	void update_Whale()
	{
		float normal_r = 0;
		for (int i = 0; i < 10; ++i) {
			if (whale[i].isAlive) {
				if (KDK::whale[i].tail_moveRange[0] <= 20 && KDK::whale[i].tail_moveRange[0] >= 0) {
					KDK::whale[i].tail_moveRange[0] += KDK::whale[i].tail_moveVelocity;
					KDK::whale[i].Tail_Radian += KDK::whale[i].tail_moveVelocity * 4;

				}
				else {
					KDK::whale[i].tail_moveRange[1] += KDK::whale[i].tail_moveVelocity;
					if (KDK::whale[i].tail_moveRange[1] > 30 || KDK::whale[i].tail_moveRange[1] < 0)
					{
						KDK::whale[i].tail_moveVelocity *= -1;
						if (KDK::whale[i].tail_moveVelocity > 0)
							KDK::whale[i].tail_moveRange[0] = 0;
						else
							KDK::whale[i].tail_moveRange[0] = 20;
					}
				}


				normal_r = sqrt(pow(whale[i].next_x - whale[i].x, 2) + pow(whale[i].next_y - whale[i].y, 2) + pow(whale[i].next_z - whale[i].z, 2));
				whale[i].x += (whale[i].next_x - whale[i].x) / normal_r * whale[i].move_Velocity;
				whale[i].y += (whale[i].next_y - whale[i].y) / normal_r * whale[i].move_Velocity;
				whale[i].z += (whale[i].next_z - whale[i].z) / normal_r * whale[i].move_Velocity;
				if (collide_pyramid(whale[i].x, whale[i].y, whale[i].z, -2000, 800, 1000) ||
					collide_pyramid(whale[i].x, whale[i].y, whale[i].z, -1000, -2500, 1000) ||
					collide_pyramid(whale[i].x, whale[i].y, whale[i].z, 2500, 100, 1000) ||
					collide_pyramid(whale[i].x, whale[i].y, whale[i].z, 1000, 3000, 1000))
				{
					if (whale[i].isRun_away)
					{
						whale[i].isRun_away = false;
						whale[i].move_Velocity -= 2;
					}
					whale[i].x -= (whale[i].next_x - whale[i].x) / normal_r * whale[i].move_Velocity;
					whale[i].y -= (whale[i].next_y - whale[i].y) / normal_r * whale[i].move_Velocity;
					whale[i].z -= (whale[i].next_z - whale[i].z) / normal_r * whale[i].move_Velocity;
					whale[i].next_x = rand() % WORLD_SCALE - WORLD_SCALE / 2;
					whale[i].next_z = rand() % WORLD_SCALE - WORLD_SCALE / 2;
					whale[i].next_y = -rand() % 400;
				}
				if (whale[i].x > whale[i].next_x - whale[i].move_Velocity&&whale[i].x<whale[i].next_x + whale[i].move_Velocity&&
					whale[i].y>whale[i].next_y - whale[i].move_Velocity&&whale[i].y<whale[i].next_y + whale[i].move_Velocity&&
					whale[i].z>whale[i].next_z - whale[i].move_Velocity&&whale[i].z < whale[i].next_z + whale[i].move_Velocity)
				{
					if (whale[i].isRun_away)
					{
						whale[i].isRun_away = false;
						whale[i].move_Velocity -= 2;
					}
					whale[i].next_x = rand() % WORLD_SCALE - WORLD_SCALE / 2;
					whale[i].next_z = rand() % WORLD_SCALE - WORLD_SCALE / 2;
					whale[i].next_y = -rand() % 400;
				}
				whale[i].phi = ((atan2((whale[i].next_x - whale[i].x), (whale[i].next_z - whale[i].z))) * 180 / PI);

				whale[i].theta = acos((whale[i].next_y - whale[i].y) / normal_r) * 180 / PI - 90;
			}
			else if (whale[i].isDeath_animation) {
				whale[i].y += 1;
				if (whale[i].y > 0)
					whale[i].isDeath_animation = false;
			}
		}
	}
	void run_away_Whale(float boat_x, float boat_y, float boat_z, int num)
	{
		whale[num].isRun_away = true;
		whale[num].move_Velocity += 2;
		whale[num].next_x = whale[num].x + whale[num].x - boat_x;
		whale[num].next_y = whale[num].y + whale[num].y - boat_y;
		whale[num].next_z = whale[num].z + whale[num].z - boat_z;
		whale[num].HP -= rand() % 10 + 20;
		if (whale[num].HP < 0)
		{
			whale[num].isAlive = false;
			whale[num].isDeath_animation = true;
		}
	}

	/////////////////////////////////������//////////////////////////////////////////////////////
	struct Spray {
		float x, y, z;
		float angleY;
		int count;

	};
	struct Node {
		Spray spray_of_water;
		Node * next;
	};
	Node *Head;
	void Insert_Spray();
	Node* delete_Spray(Node *del)
	{
		Node*curr = Head->next;
		Node*prev = Head;
		while (curr != nullptr)
		{
			if (del == curr)
			{
				prev->next = curr->next;
				delete curr;

				return prev->next;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	void update_Spray()
	{
		Node*curr = Head->next;
		while (curr != nullptr)
		{
			curr->spray_of_water.count++;
			if (curr->spray_of_water.count == 50)
				curr = delete_Spray(curr);
			if (curr == nullptr)
				break;
			curr = curr->next;

		}
	}
	void draw_spray_of_water();


	void initiallize()
	{
		Head = new Node;
		Head->spray_of_water.x = NULL;
		Head->next = nullptr;
		init_seaweed_postion();
		init_Whale();
		init_wave_pivots();

		//�ؽ�ó ���� ���Ǹ� �Ѵ�. --- (3)
		glGenTextures(4, textures);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		pBytes = LoadDIBitmap("seaweed.bmp", &info);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 128, 670, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, textures[1]);
		pBytes = LoadDIBitmap("whale_face.bmp", &info);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 701, 526, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, textures[2]);
		pBytes = LoadDIBitmap("background.bmp", &info);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 1920, 900, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, textures[3]);
		pBytes = LoadDIBitmap("whale_death_face.bmp", &info);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 701, 526, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	}

};
namespace KHM
{
	int MODE_OF_VIEW = 3; // 1 = 1��Ī(�ۻ�), 3 = 3��Ī 
	float temp_distance = 500;
	float temp_x_angle = 0;
	float temp_y_angle = 0;
	float Gauge = 0;
	float real_temp = 0;


	struct BOAT
	{
		bool is_forward = false;
		int direction = KEEP;
		bool is_breaking = false;
		float speed = 0.0;
		float velocity = 0.25;
		float x, y, z;
		float y_angle;
		float waver_z_angle;
		float waver_x_angle;
		float waver_z_velocity = 0.1;
		float waver_x_velocity = 0.05;
	};
	BOAT Boat;
	bool collide_pyramid(float x, float z, float size)
	{
		if (Boat.x - 100 < x + size && Boat.x + 100 > x - size &&
			Boat.z + 100 > z - size && Boat.z - 100 < z + size)
		{
			return 1;
		}
		return 0;
	}
	struct HARPOON
	{
		float x_angle = 0;
		float x_angle_2 = 0;
		float y_angle = 0;
		float real_temp_2 = 0;
		float x = 0;
		float y = 0;
		float z = 0;
		float x_2 = 0;
		float y_2 = 0;
		float z_2 = 0;
		float t = 0;
		float power = 2000;
		float hit_locate_x = 0;
		float hit_locate_y = 0;
		float hit_locate_z = 0;
		int hit_whale_num = -1;
		bool is_fired = false;
	};
	struct Node
	{
		HARPOON Harpoon;
		BOAT Boat;
		Node * next;
	};
	Node * Head;
	void Insert_Harpoon()
	{
		Node *New = new Node;
		New->Harpoon.x = KHM::Boat.x + (70 * sin(KHM::Boat.y_angle * RADIAN));
		New->Harpoon.y = KHM::Boat.y + HARPOON_Y;
		New->Harpoon.z = KHM::Boat.z + (HARPOON_Z * cos(KHM::Boat.y_angle * RADIAN));
		New->Harpoon.x_2 = KHM::Boat.x;
		New->Harpoon.y_2 = KHM::Boat.y;
		New->Harpoon.z_2 = KHM::Boat.z;
		New->Harpoon.x_angle = KHM::temp_x_angle;
		New->Harpoon.y_angle = KHM::temp_y_angle;
		New->Harpoon.x_angle_2 = KHM::temp_x_angle;
		//New->Harpoon.y_angle_2 = KHM::temp_y_angle;
		New->Harpoon.real_temp_2 = KHM::real_temp;
		New->Harpoon.hit_locate_x = 0;
		New->Harpoon.hit_locate_y = 0;
		New->Harpoon.hit_locate_z = 0;
		New->Harpoon.hit_whale_num = -1;
		New->Harpoon.is_fired = false;
		New->next = Head->next;
		New->Boat.z;
		Head->next = New;
	}
	Node* Delete_Harpoon(Node * key)
	{
		Node*curr = Head->next;
		Node*prev = Head;
		while (curr != nullptr)
		{
			if (key == curr)
			{
				prev->next = curr->next;
				delete curr;

				return prev->next;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	void shot_Harpoon()
	{
		Node *curr = Head->next;
		KHM::temp_x_angle = curr->Harpoon.x_angle; //���� �ʱ�ȭ�� �ۻ��� ���� ����
		KHM::temp_y_angle = curr->Harpoon.y_angle; //���� �ʱ�ȭ�� �ۻ��� ���� ����
		curr->Harpoon.x_2 = KHM::Boat.x;
		curr->Harpoon.y_2 = KHM::Boat.y;
		curr->Harpoon.z_2 = KHM::Boat.z;
		curr->Harpoon.x_angle_2 = curr->Harpoon.x_angle;
		curr->Harpoon.y_angle = KHM::Boat.y_angle;
		curr->Harpoon.real_temp_2 = KHM::real_temp;
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

				if (curr->Harpoon.hit_whale_num == -1)
				{
					curr->Harpoon.x = curr->Harpoon.x_2 + (70 * sin(curr->Harpoon.y_angle * RADIAN)) + curr->Harpoon.power * curr->Harpoon.t * cos(curr->Harpoon.x_angle * RADIAN) * sin((curr->Harpoon.y_angle + curr->Harpoon.real_temp_2) * RADIAN);
					curr->Harpoon.y = curr->Harpoon.y_2 + HARPOON_Y - curr->Harpoon.power * curr->Harpoon.t * sin(curr->Harpoon.x_angle * RADIAN) - (0.5 * 1000 * curr->Harpoon.t * curr->Harpoon.t);
					curr->Harpoon.z = curr->Harpoon.z_2 + (HARPOON_Z * cos(curr->Harpoon.y_angle * RADIAN)) + curr->Harpoon.power * curr->Harpoon.t * cos(curr->Harpoon.x_angle * RADIAN) * cos((curr->Harpoon.y_angle + curr->Harpoon.real_temp_2)* RADIAN);
					if (curr->Harpoon.t < 5)
					{
						curr->Harpoon.x_angle_2 += 0.5;//�������� �ۻ찢�� ����
						curr->Harpoon.t += 0.01;
					}
					for (int i = 0; i < 10; i++)
					{
						if ((KDK::whale[i].x - 50 <= curr->Harpoon.x && curr->Harpoon.x <= KDK::whale[i].x + 50) &&
							(KDK::whale[i].y - 50 <= curr->Harpoon.y && curr->Harpoon.y <= KDK::whale[i].y + 50) &&
							(KDK::whale[i].z - 50 <= curr->Harpoon.z && curr->Harpoon.z <= KDK::whale[i].z + 50) &&
							KDK::whale[i].isAlive)
						{
							curr->Harpoon.hit_whale_num = i;
							curr->Harpoon.hit_locate_x = curr->Harpoon.x - KDK::whale[i].x;
							curr->Harpoon.hit_locate_y = curr->Harpoon.y - KDK::whale[i].y;
							curr->Harpoon.hit_locate_z = curr->Harpoon.z - KDK::whale[i].z;
							cout << "����" << endl;

							KDK::run_away_Whale(Boat.x, Boat.y, Boat.z, i);
							break;
						}
						else if (curr->Harpoon.y <= -4000 || curr->Harpoon.x > 4000 || curr->Harpoon.x < -4000 || curr->Harpoon.z > 4000 || curr->Harpoon.z < -4000)
						{
							curr = Delete_Harpoon(curr);
							break;
						}

					}
				}
				else if (curr->Harpoon.hit_whale_num != -1)
				{
					if (KDK::whale[curr->Harpoon.hit_whale_num].isAlive) {
						curr->Harpoon.x = KDK::whale[curr->Harpoon.hit_whale_num].x + curr->Harpoon.hit_locate_x;
						curr->Harpoon.y = KDK::whale[curr->Harpoon.hit_whale_num].y + curr->Harpoon.hit_locate_y;
						curr->Harpoon.z = KDK::whale[curr->Harpoon.hit_whale_num].z + curr->Harpoon.hit_locate_z;
					}
					else
					{
						curr = Delete_Harpoon(curr);
					}
				}
			}
			if (curr == nullptr)
				break;
			curr = curr->next;
		}
	}
	void draw_moving_Harpoon() //�����̴� �ۻ� �׸��׸���
	{
		Node* curr = Head->next->next;
		while (curr != nullptr)
		{

			glPushMatrix(); {
				glPushMatrix(); {
					glColor3f(0.3, 0.2, 0.2);
					glBegin(GL_LINES);

					glVertex3f(curr->Harpoon.x, curr->Harpoon.y, curr->Harpoon.z);
					glVertex3f(Boat.x - HARPOON_Z * cos((Boat.y_angle + 90)*RADIAN), Boat.y + HARPOON_Y, Boat.z + HARPOON_Z * sin((Boat.y_angle + 90)*RADIAN));

					glEnd();
				}glPopMatrix();
				glTranslated(curr->Harpoon.x, curr->Harpoon.y, curr->Harpoon.z); //������ �ۻ� �����̰� �ϱ�
				if (curr->Harpoon.hit_whale_num != -1)
				{
					glRotatef(curr->Harpoon.y_angle + curr->Harpoon.real_temp_2+ KDK::whale[curr->Harpoon.hit_whale_num].phi+180, 0.0, 1.0, 0.0);//���ư��� �ۻ��� �����ִ� ����
					glRotatef(curr->Harpoon.x_angle_2- KDK::whale[curr->Harpoon.hit_whale_num].theta, 1.0, 0.0, 0.0);//�������� �ۻ��� ����
				}
				else {
					glRotatef(curr->Harpoon.y_angle + curr->Harpoon.real_temp_2, 0.0, 1.0, 0.0);//���ư��� �ۻ��� �����ִ� ����
					glRotatef(curr->Harpoon.x_angle_2, 1.0, 0.0, 0.0);//�������� �ۻ��� ����
				}
				glPushMatrix(); {
					glScalef(1.0, 1.0, 1.0);

					glColor3f(1.0, 0.0, 1.0);
					/*********************�ۻ� ��*********************/
					glPushMatrix(); {
						glScalef(1.0, 1.0, 50.0);           //������ 50
						glutSolidTorus(0.5, 1, 20, 20);
					}glPopMatrix();
					/*********************�ۻ� ��*********************/
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
void KDK::Insert_Spray()
{
	Node *New = new Node;
	New->spray_of_water.z = KHM::Boat.z;
	New->spray_of_water.y = KHM::Boat.y;
	New->spray_of_water.x = KHM::Boat.x;
	New->spray_of_water.count = 0;
	New->spray_of_water.angleY = KHM::Boat.y_angle;

	New->next = Head->next;
	Head->next = New;
}
void KDK::draw_spray_of_water()
{
	Node*curr = Head->next;
	while (curr != nullptr) {
		glPushMatrix(); {
			glPointSize(5.0);
			glColor3f(0.0, 0.0, 1.0);

			if (curr->spray_of_water.count <= 10) {
				glPushMatrix(); {
					glTranslatef(KHM::Boat.x, KHM::Boat.y, KHM::Boat.z);
					glRotatef(curr->spray_of_water.angleY, 0, 1, 0);
					glTranslatef(0, 0, 100);
					glPushMatrix(); {

						for (int j = 0; j < 10; ++j)
						{
							int rand_r = rand() % 30;
							glTranslated(rand_r*sin(rand() % 180 * RADIAN)*cos(rand() % 360 * RADIAN),
								rand_r *sin(rand() % 180 * RADIAN)*sin(rand() % 360 * RADIAN),
								rand_r *cos((rand() % 90 + 90)* RADIAN));
							glBegin(GL_POINTS);

							glVertex3f(0, 0, 0);
							glEnd();
						}
					}glPopMatrix();
				}glPopMatrix();
			}

			glTranslatef(curr->spray_of_water.x, curr->spray_of_water.y, curr->spray_of_water.z);
			glRotatef(curr->spray_of_water.angleY, 0, 1, 0);

			glPushMatrix(); {
				glTranslatef(curr->spray_of_water.count * 1 + rand() % 2, 0, 80);
				glBegin(GL_POINTS);
				glVertex3f(0, 2, 0);
				glEnd();
				glTranslatef(curr->spray_of_water.count * 1 + rand() % 2, 0, 0);
				glBegin(GL_POINTS);
				glVertex3f(10, 0, 0);
				glEnd();
			}glPopMatrix();

			glPushMatrix(); {
				glTranslatef(-curr->spray_of_water.count * 1 - rand() % 2, 0, 80);
				glBegin(GL_POINTS);
				glVertex3f(0, 2, 0);
				glEnd();
				glTranslatef(-curr->spray_of_water.count * 1 - rand() % 2, 0, 0);
				glBegin(GL_POINTS);
				glVertex3f(-10, 0, 0);
				glEnd();
			}glPopMatrix();
		}glPopMatrix();

		curr = curr->next;
	}
}
void draw_rader_Whale()
{
	for (int i = 0; i < 10; i++) {
		if (pow((KDK::whale[i].x - KHM::Boat.x), 2) + pow((KDK::whale[i].z - KHM::Boat.z), 2)
			< pow(810, 2)) {
			glPushMatrix(); {
				glTranslated(-(KDK::whale[i].x - KHM::Boat.x) * 200 / windowW / 2, (KDK::whale[i].z - KHM::Boat.z) * 200 / windowH / 2, 0);
				glutSolidCube(10);
			}glPopMatrix();
		}
	}
}
void drawCylinder(GLfloat radius, GLfloat h)
{
	GLfloat x, y, angle, centerx = 0, centery = 0, centerz = 0;

	glBegin(GL_TRIANGLE_FAN);           //������� ����
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

	glBegin(GL_QUAD_STRIP);            //������� ����
	for (angle = 0.0f; angle < (2.0f*PI); angle += (PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(sin(angle), cos(angle), 0.0f);
		glVertex3f(x, y, centerz);
		glVertex3f(x, y, centerz + h);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);           //������� �ظ�
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
		glRotatef(y_angle, 0.0, 1.0, 0.0);// ��ü y�� ȸ��
		glColor3f(1.0, 0.0, 0.0);
		/*********************Y�� ���̽� *********************/
		glPushMatrix(); {
			glTranslatef(0, -45, 0);
			glPushMatrix(); {
				glRotatef(90, 1.0, 0.0, 0.0);
				drawCylinder(15, 10);//�ٴ� �����
			}glPopMatrix();
			/***************y�� ����*******************/
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
		/*******************//*����*//********************/
		glRotatef(x_angle, 1.0, 0.0, 0.0);   //������ x�� ȸ��

		glColor3f(1.0, 0.0, 1.0);
		glPushMatrix(); {
			glTranslatef(0, 0, 10);
			/*********************�ۻ� ��*********************/
			glPushMatrix(); {
				glScalef(1.0, 1.0, 50.0);           //������ 50
				glutSolidTorus(0.5, 1, 20, 20);
			}glPopMatrix();
			/*********************�ۻ� ��*********************/
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
			/**********************���ؼ�******************/
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
			/*********************�׸����*******************************/
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

				/*************������*****************/
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
			/********************** ���� ����**************************/
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
			if (KHM::Head->next->Harpoon.y_angle >= -28.6472)
			{
				KHM::Head->next->Harpoon.y_angle -= 2.86472;

			}
		}
		else
		{
			//KHM::Head->next->Harpoon.y_angle = 0;
			m_camera.Rotate(0.05, 0);
		}
	}
	else if (x < prev_mx)
	{
		if (KHM::MODE_OF_VIEW == 1)
		{
			if (KHM::Head->next->Harpoon.y_angle <= 28.6472)
			{
				KHM::Head->next->Harpoon.y_angle += 2.86472;
			}
		}
		else
		{
			//KHM::Head->next->Harpoon.y_angle = 0;
			m_camera.Rotate(-0.05, 0);
		}
	}

	if (y > prev_my)
	{
		if (KHM::MODE_OF_VIEW == 1)
		{
			if (KHM::Head->next->Harpoon.x_angle <= 28.6472)
			{
				KHM::Head->next->Harpoon.x_angle += 2.86472;
			}
		}
		else
		{
			KHM::Head->next->Harpoon.x_angle = 0;
			m_camera.Rotate(0, -0.05);
		}
	}
	else if (y < prev_my)
	{
		if (KHM::MODE_OF_VIEW == 1)
		{
			if (KHM::Head->next->Harpoon.x_angle >= -28.6472)
			{
				KHM::Head->next->Harpoon.x_angle -= 2.86472;
			}
		}
		else
		{
			KHM::Head->next->Harpoon.x_angle = 0;
			m_camera.Rotate(0, 0.05);
		}
	}
	glutPostRedisplay();
	prev_mx = x;
	prev_my = y;
}

void draw_BOTTOM()
{
	glPushMatrix();
	glTranslated(0, -(WORLD_SCALE) / 2 - 500, 0);
	glColor4f(0.66f, 0.66f, 0.66f, 1.0f);
	glScaled(2, 0.5, 2);
	glutSolidCube(WORLD_SCALE);
	glPopMatrix();
}
void draw_SEA()
{
	glPushMatrix();
	glTranslated(0, -WORLD_SCALE / 2, 0);
	glScaled(2, 1, 2);
	glColor4f(0.25f, 0.25f, 0.92f, 0.5f);
	glutSolidCube(WORLD_SCALE);
	glPopMatrix();
}
void drawRect_BG(float size)
{
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	/*glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glutSolidSphere(size, 50, 50);*/
	glPushMatrix(); {
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-size / 2, size, size / 2);//2
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-size / 2, -size, size / 2);//6
		glTexCoord2f(0.25f, 0.0f);
		glVertex3f(size / 2, -size, size / 2);//7
		glTexCoord2f(0.25f, 1.0f);
		glVertex3f(size / 2, size, size / 2);//3
											 //�����ʿ���
		glTexCoord2f(0.25f, 1.0f);
		glVertex3f(size / 2, size, size / 2);//3
		glTexCoord2f(0.25f, 0.0f);
		glVertex3f(size / 2, -size, size / 2);//7
		glTexCoord2f(0.5f, 0.0f);
		glVertex3f(size / 2, -size, -size / 2);//8
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(size / 2, size, -size / 2);//4
											  //���ʿ���
		glTexCoord2f(0.75f, 1.0f);
		glVertex3f(-size / 2, size, -size / 2);//1
		glTexCoord2f(0.75f, 0.0f);
		glVertex3f(-size / 2, -size, -size / 2);//5
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-size / 2, -size, size / 2);//6
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-size / 2, size, size / 2);//2
											  //�޸�
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(size / 2, size, -size / 2);//4
		glTexCoord2f(0.5f, 0.0f);
		glVertex3f(size / 2, -size, -size / 2);//8
		glTexCoord2f(0.75f, 0.0f);
		glVertex3f(-size / 2, -size, -size / 2);//5
		glTexCoord2f(0.75f, 1.0f);
		glVertex3f(-size / 2, size, -size / 2);//1
		glEnd();
		//glDisable(GL_TEXTURE_GEN_S);
		//glDisable(GL_TEXTURE_GEN_T);
	}glPopMatrix();
}
void draw_SKY()
{
	glPushMatrix();
	glTranslated(0, WORLD_SCALE / 4, 0);
	glEnable(GL_TEXTURE_2D);

	glColor4f(0.1f, 0.3f, 1.0f, 1.0f);
	drawRect_BG(WORLD_SCALE * 2 + 10);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
void draw_BOAT()
{

	glPushMatrix(); {
		glPushMatrix(); {
			// ��� ������ ����
			glColor3f(1.0, 0.0, 1.0);
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &KDK::back_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// �׸��带 �̿��� ��� �����
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// ���� �̿��Ͽ� �׸��� ����
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glColor3f(0.8, 0.3, 0.3);
		}glPopMatrix();
		glPushMatrix(); {
			// ��� ������ ����
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &KDK::deck_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// �׸��带 �̿��� ��� �����
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// ���� �̿��Ͽ� �׸��� ����
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glColor3f(1.0, 0.0, 1.0);
		}glPopMatrix();
		glPushMatrix(); {
			// ��� ������ ����
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &KDK::left_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// �׸��带 �̿��� ��� �����
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// ���� �̿��Ͽ� �׸��� ����
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glColor3f(1.0, 0.0, 1.0);
		}glPopMatrix();
		glPushMatrix(); {
			// ��� ������ ����
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &KDK::right_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// �׸��带 �̿��� ��� �����
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// ���� �̿��Ͽ� �׸��� ����
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
		}glPopMatrix();

		glPushMatrix(); {
			glColor3f(1.0f, 0.5f, 0.5f);
			glTranslated(0, 70, -60);
			glPushMatrix(); {
				glScalef(1.3, 1.0, 1.3);
				glutSolidCube(50);
				glColor3f(0, 0, 1);
				glutWireCube(50);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(0.5f, 0.5f, 0.5f);
				glTranslated(0, 40, 12.5);
				glPushMatrix(); {
					glScalef(1.3, 1.0, 1.2);
					glutSolidCube(30);
					glColor3f(0, 0, 1);
					glutWireCube(30);
				}glPopMatrix();
				glPushMatrix(); {// ������ ��
					glTranslated(0, 15, 0);
					glColor3f(1, 0, 0);
					glBegin(GL_LINES);

					glVertex3f(75 / 8, 75 / 2 + 75 / 4 + 10, 0);
					glVertex3f(30 * 1.3 / 2, 0, 0);

					glVertex3f(-75 / 8, 75 / 2 + 75 / 4 + 10, 0);
					glVertex3f(-30 * 1.3 / 2, 0, 0);

					glEnd();

				}glPopMatrix();
				glPushMatrix(); {//������
					glColor3f(1, 1, 1);
					glTranslated(0, 15, 0);
					glRotated(-90, 1, 0, 0);
					drawCylinder(2, 75 / 2);
					glTranslated(0, 0, 75 / 2);
					drawCylinder(1, 75 / 2);
					glPushMatrix(); {
						glTranslated(0, 0, 75 / 4 + 10);
						glRotated(-90, 0, 1, 0);
						glTranslated(0, 0, -75 / 8);
						drawCylinder(1, 75 / 4);
					}glPopMatrix();
					glColor3f(1, 0.8, 1);
					glTranslated(0, 0, 75 / 2);
					drawCylinder(3, 5);
				}glPopMatrix();
			}glPopMatrix();
		}glPopMatrix();
	}glPopMatrix();
}
//....................................................................................................
void initialize()
{
	KHM::Head = new KHM::Node;
	KHM::Head->next = nullptr;
	KHM::Insert_Harpoon();
	KDK::initiallize();

	m_camera.Initialize(float3{ 0,0,0 }, DISTANCE, 1, 99999, 90);
	///////////////////////////////////////////////////////////////////////////////
	//MCI_OPEN_PARMS mciOpen;   // MCI_OPEN_PARAMS ����ü ���� 
	//mciOpen.lpstrDeviceType = "mpegvideo";  // mpegvideo : mp3, waveaudio : wav, avivideo : avi
	//mciOpen.lpstrElementName = "hammer.mp3"; // �����̸�
	////mciSendCommand(0, MCI_OPEN,
	//	MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_OPEN_TYPE,
	//	(DWORD)(LPVOID)&mciOpen);
	//MCI_PLAY_PARMS mciPlay;
	//int dwID = mciOpen.wDeviceID;
	////mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
	//////MCI_NOTIFY : �⺻, MCI_DGV_PLAY_REPEAT : �ݺ�

	//////�ٽý���
	////mciSendCommandW(dwID, MCI_RESUME, 0, NULL);

	////// �Ͻ�����
	////mciSendCommand(dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);

	////// ����
	////mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);
	////PlaySound(TEXT(SOUND_BGM), NULL, SND_ASYNC | SND_ALIAS);
	////SND_ASYNC  ����߿��� ���α׷��� ��� ���ư�
	////SND_SYNC ����� ������ ���α׷��� ���ư�
	////SND_FILENAME �Ű������� ���� ������ �̸��� ���
	////SND_LOOP �ݺ���� SND_ASYNC�� ���̽����
	////SND_PURGE �������
}
void main(int argc, char *argv[])
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ����
	glutInitWindowSize(1024, 768); // �������� ũ�� ����
	glutCreateWindow("����"); // ������ ���� (������ �̸�)
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMouseWheelFunc(mouseWheel);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);                  // Ű���� �Է� �ݹ� �Լ�
	glutKeyboardUpFunc(UpKeyboard);                  // Ű���� �Է� �ݹ� �Լ�
	glutTimerFunc(20, Timerfunction, 1);         // Ÿ�̸� �Լ� ����
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initialize();
	glutMainLoop();
}
// ������ ��� �Լ�

GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������ ����ϵ��� ����
	glEnable(GL_LIGHTING);
	// ���� ����
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);


	// ���� ȿ���� �����Ѵ�.
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// ���� �÷� Ʈ��ŷ�� �����Ѵ�.
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glViewport(0, 0, windowW, windowH);
	glLoadIdentity();
	//////////////////////////////////////////////

	glPushMatrix(); {
		m_camera.LookAt();
		glTranslated(KHM::Boat.x, KHM::Boat.y, KHM::Boat.z);
		glRotated(KHM::Boat.y_angle, 0, 1, 0);

		if (KHM::MODE_OF_VIEW == 1)
		{
			draw_Harpoon_Gun(0, HARPOON_Y, HARPOON_Z, KHM::Head->next->Harpoon.x_angle, KHM::Head->next->Harpoon.y_angle); //KHM::Head->next->Harpoon.y_angle
		}
		if (KHM::MODE_OF_VIEW == 3)
		{
			m_camera.SetPosition(float3{ KHM::Boat.x, KHM::Boat.y, KHM::Boat.z });
			glRotated(KHM::Boat.waver_x_angle, 1, 0, 0);
			glRotated(KHM::Boat.waver_z_angle, 0, 0, 1);
			draw_Harpoon_Gun(0, HARPOON_Y, HARPOON_Z, KHM::Head->next->Harpoon.x_angle, 0); //KHM::Head->next->Harpoon.y_angle

		}

		draw_BOAT();
	}glPopMatrix();

	glPushMatrix(); {
		KHM::draw_moving_Harpoon();
		KDK::draw_spray_of_water();
	}glPopMatrix();

	glPushMatrix(); {
		draw_BOTTOM();
		draw_SKY();
		KDK::draw_MAP_object();
		KDK::draw_Whale();
		KDK::draw_wave();
		draw_SEA();
	}glPopMatrix();

	glDisable(GL_LIGHTING);
	//------------------------------------------------------------------------------------------
	glViewport(windowW - 200, windowH - 200, 200, 200);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
		glColor4f(0.0, 1.0, 0.0, 1.0f);
		glutWireCone(100, 1, 20, 4);
		glColor4f(0.0, 0.2, 0.0, 0.5f);
		glutSolidCone(100, 1, 20, 4);
		glRotated(KHM::Boat.y_angle, 0, 0, 1);
		glColor4f(1.0, 0.0, 0.0, 1.0f);
		draw_rader_Whale();
	}glPopMatrix();
	//----------------------------------------------------------------------------------------------
	if (KHM::MODE_OF_VIEW == 1)
	{

		glViewport(windowW / 2 - 150, 50, 300, 100);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-150.0, 150.0, -100.0, 100.0, -100.0, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); {
			if (int(KHM::Gauge) < 2)
				glColor4f(1.0, 0.0, 0.0, 1.0f);
			else if (int(KHM::Gauge) >= 2)
				glColor4f(rand() % 10 * 0.1, rand() % 10 * 0.1, rand() % 10 * 0.1, 1.0f);

			int len = (int)strlen(shot_string[std::min(2, int(KHM::Gauge))]);
			if (int(KHM::Gauge) == 0)
				glRasterPos3f(-54, 0, 0);
			else if (int(KHM::Gauge) == 1)
				glRasterPos3f(-45, 0, 0);
			else if (int(KHM::Gauge) >= 2)
				glRasterPos3f(-27, 0, 0);
			for (int i = 0; i < len; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, shot_string[std::min(2, int(KHM::Gauge))][i]);


		}glPopMatrix();
	}
	
	glFlush(); // ȭ�鿡 ����ϱ�
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	windowW = w;
	windowH = h;

}
void Keyboard(unsigned char key, int x, int y)
{
	int mod = 0;

	switch (key)
	{
	case 'q':
		exit(1);
		break;
	case 'w':
		if (KHM::MODE_OF_VIEW == 3)
		{
			KHM::Boat.is_forward = true;
			mod = glutGetModifiers();
			if (mod == 'a')
			{
				glPushMatrix();
				KHM::Boat.direction = LEFT;
				glPopMatrix();
			}
			else if (mod == 'd')
			{
				glPushMatrix();
				KHM::Boat.direction = RIGHT;
				glPopMatrix();
			}
		}
		break;
	case 'a':
		if (KHM::MODE_OF_VIEW == 3)
		{
			KHM::Boat.direction = LEFT;
			mod = glutGetModifiers();
			if (mod == 'w')
			{
				KHM::Boat.is_forward = true;
			}
		}
		break;
	case 's':
		if (KHM::Boat.speed > 0.0)
		{
			KHM::Boat.is_breaking = true;
		}
		break;
	case 'd':
		if (KHM::MODE_OF_VIEW == 3)
		{
			KHM::Boat.direction = RIGHT;
			mod = glutGetModifiers();
			if (mod == 'w')
			{
				KHM::Boat.is_forward = true;
			}
		}
		break;
	case 'f':
		if (KHM::MODE_OF_VIEW == 1)
		{

			KHM::real_temp = KHM::Head->next->Harpoon.y_angle;
			//KHM::shot_Harpoon();

			KHM::Gauge++;
		}
		break;
	case '1':
		m_camera.Initialize_radian();
		prev_mx = 0;
		prev_my = 0;
		KHM::MODE_OF_VIEW = 1;
		m_camera.Rotate(-KHM::Boat.y_angle * RADIAN, 0);
		break;
	case '3':
		KHM::Head->next->Harpoon.x_angle = 0;
		KHM::Head->next->Harpoon.y_angle = 0;
		m_camera.Initialize(float3{ 0,0,0 }, KHM::temp_distance, 1, 99999, 90);
		KHM::MODE_OF_VIEW = 3;
		break;
	case 'i':
		m_camera.Rotate(3.141592, 0);
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
	case 'f':
		KHM::real_temp = KHM::Head->next->Harpoon.y_angle;
		if (KHM::Gauge >= 2)
		{
			KHM::shot_Harpoon();
		}
		KHM::Gauge = 0;
		break;
	}
}

void Timerfunction(int value)
{

	glPushMatrix(); {
		KHM::move_Harpoon();
	}glPopMatrix();

	if (KHM::MODE_OF_VIEW == 1)
	{
		m_camera.Initialize(float3{ KHM::Boat.x + (70 * sin(KHM::Boat.y_angle * 3.141592f / 180)) ,KHM::Boat.y + HARPOON_Y + 35,KHM::Boat.z + (HARPOON_Z * cos(KHM::Boat.y_angle * 3.141592f / 180)) }, 30, 1, 99999, 90);

	}

	//======================================================================================
	if (KHM::Boat.is_forward == true)
	{
		if (KHM::Boat.speed < 5.0)
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

	KHM::Boat.z += KHM::Boat.speed * cos(KHM::Boat.y_angle * RADIAN);
	if (KHM::Boat.z > WORLD_SCALE - 100 || KHM::Boat.z < -(WORLD_SCALE - 100))
	{
		KHM::Boat.z -= KHM::Boat.speed * cos(KHM::Boat.y_angle * RADIAN);
	}

	KHM::Boat.x += KHM::Boat.speed * sin(KHM::Boat.y_angle * RADIAN);
	if (KHM::Boat.x > (WORLD_SCALE - 100) || KHM::Boat.x < -(WORLD_SCALE - 100))
	{
		KHM::Boat.x -= KHM::Boat.speed * sin(KHM::Boat.y_angle * RADIAN);
	}
	if (KHM::collide_pyramid(-2000, 800, 1000 * (1000 - 800) / 1000) ||
		KHM::collide_pyramid(-1000, -2500, 1000 * (1000 - 800) / 1000) ||
		KHM::collide_pyramid(2500, 100, 1000 * (1000 - 800) / 1000) ||
		KHM::collide_pyramid(1000, 3000, 1000 * (1000 - 800) / 1000))
	{
		KHM::Boat.z -= KHM::Boat.speed * cos(KHM::Boat.y_angle * RADIAN);
	}
	if (KHM::collide_pyramid(-2000, 800, 1000 * (1000 - 800) / 1000) ||
		KHM::collide_pyramid(-1000, -2500, 1000 * (1000 - 800) / 1000) ||
		KHM::collide_pyramid(2500, 100, 1000 * (1000 - 800) / 1000) ||
		KHM::collide_pyramid(1000, 3000, 1000 * (1000 - 800) / 1000))
	{
		KHM::Boat.x -= KHM::Boat.speed * sin(KHM::Boat.y_angle * RADIAN);
	}

	if (KHM::Boat.direction == LEFT)
	{
		KHM::Boat.y_angle += KHM::Boat.speed / 5 + 0.5;
	}
	else if (KHM::Boat.direction == RIGHT)
	{
		KHM::Boat.y_angle -= KHM::Boat.speed / 5 + 0.5;
	}




	if (KHM::Boat.speed > 0.0) {

		KDK::Insert_Spray();
	}

	if (KHM::Boat.waver_z_angle >= 5.0 || KHM::Boat.waver_z_angle <= -5.0) {
		KHM::Boat.waver_z_velocity *= -1;
	}

	KHM::Boat.waver_z_angle += KHM::Boat.waver_z_velocity;

	if (KHM::Boat.waver_x_angle >= 5.0 || KHM::Boat.waver_x_angle <= -5.0) {
		KHM::Boat.waver_x_velocity *= -1;
	}

	KHM::Boat.waver_x_angle += KHM::Boat.waver_x_velocity;
	KDK::update_Spray();
	//===========================================================================================
	KDK::update_Whale();
	KDK::update_wave();
	KHM::real_temp = KHM::Head->next->Harpoon.y_angle;
	glutPostRedisplay();                  // ȭ�� �����
	glutTimerFunc(20, Timerfunction, 1);      // Ÿ�̸��Լ� �缳��
}