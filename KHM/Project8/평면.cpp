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
#define HARPOON_Y 165
#define HARPOON_Z 70

enum Direction { LEFT = -1, KEEP, RIGHT };
using namespace std;

static double prev_mx = 0;
static double prev_my = 0;
float check_x = 0;
float check_y = 0;
float check_z = 0;
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
	// 바이너리 읽기 모드로 파일을 연다
	//if ((fp = fopen(filename, "rb")) == NULL)
	fopen_s(&fp, filename, "rb");
	if (fp == NULL)
		return NULL;
	// 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount + 7) / 8.0 * abs((*info)->bmiHeader.biHeight);
	// 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bits;
}
GLubyte *pBytes; // 데이터를 가리킬 포인터
BITMAPINFO *info; // 비트맵 헤더 저장할 변수
				  // 조명 설정
				  // n개의 이미지 텍스처 매핑을 한다.
GLuint textures[4];
GLdouble rotateWC[16]
= { 1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1 };



namespace KHM
{
	int MODE_OF_VIEW = 3; // 1 = 1인칭(작살), 3 = 3인칭 
	float temp_distance = 500;
	float temp_x_angle = 0;
	float temp_y_angle = 0;
	float Gauge = 0;

	struct BOAT
	{
		bool is_forward = false;
		int direction = KEEP;
		float direction_percent = 1.0f;
		bool is_breaking = false;
		float speed = 0.0;
		float velocity = 1;
		float x, y, z;
		float y_angle;
	};
	BOAT Boat;

	struct HARPOON
	{
		float x_angle_2 = 0;
		float fired_y_angle = 0;
		float y_angle = 0;
		float x_angle = 0;
		float x = 0;
		float y = 0;
		float z = 0;
		float t = 0;
		float power = 1000;
		bool is_hit = false;
		bool is_fired = false;
		GLdouble rotateWC[16]
			= { 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
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
		New->Harpoon.x = KHM::Boat.x + (70 * sin(KHM::Boat.y_angle * RADIAN));
		New->Harpoon.y = KHM::Boat.y + HARPOON_Y;
		New->Harpoon.z = KHM::Boat.z + (HARPOON_Z * cos(KHM::Boat.y_angle * RADIAN));
		New->Harpoon.x_angle = KHM::temp_x_angle;
		New->Harpoon.y_angle = KHM::Boat.y_angle;
		New->Harpoon.x_angle_2 = KHM::temp_x_angle;
		New->Harpoon.fired_y_angle = KHM::Boat.y_angle;
		New->Harpoon.is_hit = false;
		New->Harpoon.is_fired = false;

		New->next = Head->next;
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
		KHM::temp_x_angle = curr->Harpoon.x_angle;
		KHM::temp_y_angle = curr->Harpoon.y_angle;
		//curr->Harpoon.fired_y_angle = KHM::Boat.y_angle;
		curr->Harpoon.x_angle_2 = curr->Harpoon.x_angle;
		curr->Harpoon.y_angle = KHM::Boat.y_angle;
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
				curr->Harpoon.x = KHM::Boat.x + (70 * sin(curr->Harpoon.y_angle * RADIAN)) + curr->Harpoon.power * curr->Harpoon.t * cos(curr->Harpoon.x_angle * RADIAN) * sin(curr->Harpoon.y_angle * RADIAN);
				curr->Harpoon.y = KHM::Boat.y + HARPOON_Y - curr->Harpoon.power * curr->Harpoon.t * sin(curr->Harpoon.x_angle * RADIAN) - (0.5 * 1000 * curr->Harpoon.t * curr->Harpoon.t);
				curr->Harpoon.z = KHM::Boat.z + (HARPOON_Z * cos(curr->Harpoon.y_angle * RADIAN)) + curr->Harpoon.power * curr->Harpoon.t * cos(curr->Harpoon.x_angle * RADIAN) * cos(curr->Harpoon.y_angle * RADIAN);
				//curr->Harpoon.y_angle = KHM::Boat.y_angle;
				if (curr->Harpoon.t < 5)
				{

					curr->Harpoon.x_angle_2 += 0.5;
					curr->Harpoon.t += 0.01;
				}

				//여기 어딘가에 타겟 맞췄을 때 is_hit  true로 바꾸는거
				//맵의 범위를 넘었을 때 삭제
			}
			//if (curr->Harpoon.t >= 1)
			if ((-50 < curr->Harpoon.x && curr->Harpoon.x < 50) && (-50 < curr->Harpoon.y && curr->Harpoon.y < 50) && (250 < curr->Harpoon.z && curr->Harpoon.z < 350))
			{
				cout << "명중" << endl;
				curr = Delete_Harpoon(curr);
			}
			else if (curr->Harpoon.y <= -100)
			{
				cout << "지움" << endl;
				curr = Delete_Harpoon(curr);
			}
			else curr = curr->next;
		}
	}
	//void add_wc_x_angle(float speed)
	//{
	//	Node* curr = Head->next;
	//	while (curr != nullptr)
	//	{
	//		if (curr->Harpoon.is_fired) {
	//			glPushMatrix(); {
	//				glRotatef(speed, 1.0, 0.0, 0.0);
	//				glMultMatrixd(curr->Harpoon.rotateWC);
	//				glGetDoublev(GL_MODELVIEW_MATRIX, curr->Harpoon.rotateWC);
	//			}glPopMatrix();
	//		}
	//		curr = curr->next;
	//	}
	//}
	//void add_wc_y_angle(float speed)
	//{
	//	Node* curr = Head->next;
	//	while (curr != nullptr)
	//	{
	//		if (curr->Harpoon.is_fired) {
	//			glPushMatrix(); {
	//				glRotatef(speed, 0.0, 1.0, 0.0);
	//				glMultMatrixd(curr->Harpoon.rotateWC);
	//				glGetDoublev(GL_MODELVIEW_MATRIX, curr->Harpoon.rotateWC);
	//			}glPopMatrix();
	//		}
	//		curr = curr->next;
	//	}

	//}
	void draw_moving_Harpoon()
	{
		Node* curr = Head->next->next;
		while (curr != nullptr)
		{
			glPushMatrix(); {
				glBegin(GL_LINES);

				glVertex3f(curr->Harpoon.x, curr->Harpoon.y, curr->Harpoon.z);
				glVertex3f(check_x, check_y, check_z);


				glEnd();
				glMultMatrixd(curr->Harpoon.rotateWC);
				glTranslated(curr->Harpoon.x, curr->Harpoon.y, curr->Harpoon.z);
				glRotatef(curr->Harpoon.y_angle, 0.0, 1.0, 0.0);
				glRotatef(curr->Harpoon.x_angle_2, 1.0, 0.0, 0.0);
				glPushMatrix(); {
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
	const int SEAWEED_NUM = 40;
	GLdouble random_seaweed[SEAWEED_NUM][3];
	void init_seaweed_postion()
	{
		for (int i = 0; i < SEAWEED_NUM; ++i) {
			random_seaweed[i][0] = rand() % WORLD_SCALE - WORLD_SCALE / 2;
			random_seaweed[i][1] = rand() % WORLD_SCALE - WORLD_SCALE / 2;
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
			//윗면
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-15, 600, -15); //1
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-15, 600, 15);// 2
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(15, 600, 15);// 3
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(15, 600, -15);//4
													  //앞면		 
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-15, 600, 15);//2
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-15, -600, 15);//6
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(15, -600, 15);//7
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(15, 600, 15);//3
													 //오른쪽옆면
			glVertex3f(15, 600, 15);//3
			glVertex3f(15, -600, 15);//7
			glVertex3f(15, -600, -15);//8
			glVertex3f(15, 600, -15);//4
													  //왼쪽옆면
			glVertex3f(-15, 600, -15);//1
			glVertex3f(-15, -600, -15);//5
			glVertex3f(-15, -600, 15);//6
			glVertex3f(-15, 600, 15);//2
													  //뒷면
			glVertex3f(15, 600, -15);//4
			glVertex3f(15, -600, -15);//8
			glVertex3f(-15, -600, -15);//5
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
			glPushMatrix(); {
				glColor3f(1, 0, 0.6);
				//glRotated(30, 1, 0, 0);
				glTranslated(-2000, 0, 800);
				draw_pyramid(1500);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(1, 0.7, 0.6);
				//glRotated(270, 0, 0, 1);
				glTranslated(-1200, 0, -2018);
				draw_pyramid(1500);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(0.3, 0.7, 0.6);
				//glRotated(135, 1, 0, 1);
				glTranslated(2500, 0, 100);
				draw_pyramid(1500);
			}glPopMatrix();
			glPushMatrix(); {
				glColor3f(0.2, 0.2, 0.2);
				//glRotated(-90, 1, 0, 0);
				glTranslated(+1000, 0, 2000);
				draw_pyramid(1500);
			}glPopMatrix();
			glGenTextures(4, textures);
			//텍스처와 객체를 결합한다. --- (1)

			///////////////////////////////////////////////////////////////////////////////
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			//이미지 로딩을 한다. --- (2)
			pBytes = LoadDIBitmap("seaweed.bmp", &info);
			//텍스처 설정 정의를 한다. --- (3)
			glTexImage2D(GL_TEXTURE_2D, 0, 3, 128, 670, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

			//glBindTexture(GL_TEXTURE_2D, textures[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
			// 텍스처 매핑 활성화
			glEnable(GL_TEXTURE_2D);
			glColor3f(0.2, 0.2, 0.2);
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
			glPushMatrix(); {
				glColor3f(0, 0, 0);
				//glRotated(-90, 1, 0, 0);
				glTranslated(0, WORLD_SCALE, 0);
				draw_stone();
			}glPopMatrix();

		}glPopMatrix();
	}
};
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
	glColor4f(0.25f, 0.85f, 0.92f, 0.5f);
	glutSolidCube(WORLD_SCALE);
	glPopMatrix();
}
void draw_SKY()
{
	glPushMatrix();
	glScaled(2, 1, 2);

	glColor4f(0.1f, 0.3f, 1.0f, 1.0f);
	glutSolidCube(WORLD_SCALE + 4000);
	glPopMatrix();
}
void draw_BOAT()
{
	glPushMatrix(); {
		glPushMatrix(); {
			// 곡면 제어점 설정
			glColor3f(0.0, 1.0, 0.0);
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &KDK::back_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// 그리드를 이용한 곡면 드로잉
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// 선을 이용하여 그리드 연결
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glColor3f(1.0, 1.0, 1.0);
		}glPopMatrix();
		glPushMatrix(); {
			// 곡면 제어점 설정
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &KDK::deck_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// 그리드를 이용한 곡면 드로잉
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// 선을 이용하여 그리드 연결
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glColor3f(1.0, 0.0, 1.0);
		}glPopMatrix();
		glPushMatrix(); {
			// 곡면 제어점 설정
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &KDK::left_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// 그리드를 이용한 곡면 드로잉
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// 선을 이용하여 그리드 연결
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glColor3f(1.0, 1.0, 0.0);
		}glPopMatrix();
		glPushMatrix(); {
			// 곡면 제어점 설정
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 3, &KDK::right_ctrlpoints[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			// 그리드를 이용한 곡면 드로잉
			glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
			// 선을 이용하여 그리드 연결
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
				glPushMatrix(); {// 전봇대 줄
					glTranslated(0, 15, 0);
					glColor3f(1, 0, 0);
					glBegin(GL_LINES);

					glVertex3f(75 / 8, 75 / 2 + 75 / 4 + 10, 0);
					glVertex3f(30 * 1.3 / 2, 0, 0);

					glVertex3f(-75 / 8, 75 / 2 + 75 / 4 + 10, 0);
					glVertex3f(-30 * 1.3 / 2, 0, 0);

					glEnd();

				}glPopMatrix();
				glPushMatrix(); {//전봇대
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

void drawRect(float size, float x, float y, float z)
{
	glPushMatrix(); {
		//glTranslatef(0, WORLD_SCALE, 0);
		glTranslatef(x, y, z);
		glColor3f(1.0, 1.0, 1.0);
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
	}glPopMatrix();
}

//void draw_basic_Whale(float r, float phi, float theta, GLdouble arr[])
//{
//	//r 원점으로부터의 거리, phi x와 z의 각도, theta x,z평면과 y의 각도, arr 회전을 쓸 배열이 들어갈 듯
//	float x = r * cos(phi*RADIAN) * cos(theta*RADIAN);
//	float y = r * sin(phi*RADIAN);
//	float z = r * cos(phi*RADIAN) * sin(theta*RADIAN);
//	glPushMatrix(); {
//		glTranslatef(x, y, z);
//		glPushMatrix(); {
//			glRotatef(phi, 0.0, 1.0, 0.0);
//			glMultMatrixd(arr);
//			glGetDoublev(GL_MODELVIEW_MATRIX, arr);
//		}glPopMatrix();
//		///////////////////큰 몸통/////////////////////////////////////////////
//		glColor3f(1.0, 1.0, 1.0);
//		drawRect(100);
//		/////////////////////////지느러미 2개//////////////////////////
//		glPushMatrix(); {
//			glTranslatef(0, -30, 20);
//			/****************왼쪽*******************/
//			glPushMatrix(); {
//				glTranslatef(60, 0, 0);
//
//				glScalef(0.5, 0.5, 1);
//				//glColor3f(1.0, 1.0, 1.0);
//				drawRect(40);
//				glPushMatrix(); {
//					glTranslatef(30, 0, 0);
//
//					glScalef(0.7, 0.8, 0.7);
//					//glColor3f(1.0, 1.0, 1.0);
//					drawRect(40);
//				}glPopMatrix();
//			}glPopMatrix();
//			/***************오른쪽*****************/
//			glPushMatrix(); {
//				glTranslatef(-60, 0, 0);
//
//				glScalef(0.5, 0.5, 1);
//				//glColor3f(1.0, 1.0, 1.0);
//				drawRect(40);
//				glPushMatrix(); {
//					glTranslatef(-30, 0, 0);
//
//					glScalef(0.7, 0.8, 0.7);
//					//glColor3f(1.0, 1.0, 1.0);
//					drawRect(40);
//				}glPopMatrix();
//			}glPopMatrix();
//		}glPopMatrix();
//		/////////////////////////////////////////////////////////////////////
//		/**********************중간 몸통***********************************/
//		glPushMatrix(); {
//			glTranslatef(0, -10 + KDK::whale.tail_moveRange[0], 70);
//
//			glRotatef(0, 0.0, 0.0, 1.0);
//			glPushMatrix(); {
//				glScalef(1, 1, 0.5);
//				//glColor3f(1.0, 1.0, 1.0);
//				drawRect(80);
//			}glPopMatrix();
//			/*****************꼬리 몸통**********************************/
//			glPushMatrix(); {
//				glTranslatef(0, -10 + KDK::whale.tail_moveRange[1], 40);
//
//				glRotatef(0, 0.0, 0.0, 1.0);
//				//glColor3f(1.0, 1.0, 1.0);
//				drawRect(40);
//				/*************************************************************/
//				glPushMatrix(); {
//					glTranslatef(0, 0, 20);
//					glRotatef(KDK::whale.Tail_Radian, 1.0, 0.0, 0.0);
//					glTranslatef(0, 0, 20);
//					//////////////////꼬리 지느러미 2개+가운데 1개////////////////// 
//					/******************지느러미 왼쪽**********************/
//					glPushMatrix(); {
//						glTranslatef(20, 0, 0);
//						glRotatef(30, 0.0, 1.0, 0.0);
//						glScalef(0.7, 0.5, 1);
//						//glColor3f(1.0, 1.0, 1.0);
//						drawRect(40);
//					}glPopMatrix();
//					/********************지느러미 오른쪽***************************/
//					glPushMatrix(); {
//						glTranslatef(-20, 0, 0);
//						glRotatef(-30, 0.0, 1.0, 0.0);
//						glScalef(0.7, 0.5, 1);
//						//glColor3f(1.0, 1.0, 1.0);
//						drawRect(40);
//					}glPopMatrix();
//					/***********************지느러미 가운데**********************/
//					glPushMatrix(); {
//						glTranslatef(0, 0, -10);
//						glScalef(1, 1, 1);
//						//glColor3f(1.0, 1.0, 1.0);
//						drawRect(20);
//					}glPopMatrix();
//					/*****************************************************************/
//				}glPopMatrix();
//			}glPopMatrix();
//		}glPopMatrix();
//	}glPopMatrix();
//}

void initialize()
{
	KHM::Head = new KHM::Node;
	KHM::Head->next = nullptr;
	KHM::Insert_Harpoon();
	KDK::init_seaweed_postion();


}
void main(int argc, char *argv[])
{
	srand(time(NULL));
	initialize();
	m_camera.Initialize(float3{ 0,0,0 }, DISTANCE, 1, 99999, 90);
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
	glEnable(GL_TEXTURE_2D);
	//glBlendFunc(GL_SRC_COLOR, GL_ZERO);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glTexImage2D();
	glutMainLoop();
}
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	GLfloat AmbientLight[] = { 0.0f, 1.0f, 0.0f, 1.0f }; // 녹색조명
	GLfloat DiffuseLight[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // 적색조명
	GLfloat SpecularLight[] = { 1.0, 1.0, 1.0, 1.0 }; // 백색조명
	GLfloat lightPos[] = { 10.0, 5.0, 20.0, 1.0 }; // 위치: (10, 5, 20)

		// 조명을 사용하도록 설정
	glEnable(GL_LIGHTING);
	// 조명 설정
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 조명 효과를 설정한다.
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// 재질 컬러 트래킹을 설정한다.
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//////////////////////////////////////////////

	glPushMatrix(); {
		m_camera.LookAt();
		glTranslated(KHM::Boat.x, KHM::Boat.y, KHM::Boat.z);
		//glRotated((1 - KHM::Boat.direction_percent) * 90, 0, 1, 0);
		glRotated(KHM::Boat.y_angle, 0, 1, 0);
		draw_BOAT();
		if (KHM::MODE_OF_VIEW == 1)
		{
			draw_Harpoon_Gun(0, HARPOON_Y, HARPOON_Z, KHM::Head->next->Harpoon.x_angle, KHM::Head->next->Harpoon.y_angle); //KHM::Head->next->Harpoon.y_angle
		}
		if (KHM::MODE_OF_VIEW == 3)
		{
			draw_Harpoon_Gun(0, HARPOON_Y, HARPOON_Z, KHM::Head->next->Harpoon.x_angle, 0); //KHM::Head->next->Harpoon.y_angle
			m_camera.SetPosition(float3{ KHM::Boat.x, KHM::Boat.y, KHM::Boat.z });
		}
	}glPopMatrix();

	glPushMatrix(); {
		KHM::draw_moving_Harpoon();
	}glPopMatrix();

	glPushMatrix(); {
		draw_BOTTOM();
		draw_SKY();
		KDK::draw_MAP_object();
		draw_SEA();
	}glPopMatrix();

	glutSwapBuffers(); // 화면에 출력하기
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
		//if (KHM::MODE_OF_VIEW == 1)
		//{
		KHM::shot_Harpoon();
		
		//KHM::Gauge++;
		//}
		break;
	case '1':
		m_camera.Initialize_radian();
		prev_mx = 0;
		prev_my = 0;
		KHM::MODE_OF_VIEW = 1;
		break;
	case '3':
		KHM::Head->next->Harpoon.x_angle = 0;
		KHM::Head->next->Harpoon.y_angle = 0;
		m_camera.Initialize(float3{ 0,0,0 }, KHM::temp_distance, 1, 99999, 90);
		KHM::MODE_OF_VIEW = 3;
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
		//if (KHM::Gauge >= 2)
		//{
		//	KHM::shot_Harpoon();
		//}
		//KHM::Gauge = 0;
		break;
	}
}

void Timerfunction(int value)
{
	//if (KHM::MODE_OF_VIEW == 3)
	//{
	//	//m_camera.Initialize(float3{ KHM::Harpoon.x,KHM::Boat.y,KHM::Boat.z }, DISTANCE, 1, 9999, 90);
	//}
	glPushMatrix(); {
		KHM::move_Harpoon();
	}glPopMatrix();

	if (KHM::MODE_OF_VIEW == 1)
	{
		m_camera.Initialize(float3{ KHM::Boat.x + (70 * sin(KHM::Boat.y_angle * 3/180)) ,KHM::Boat.y + HARPOON_Y + 35,KHM::Boat.z + (HARPOON_Z * cos(KHM::Boat.y_angle * 3 / 180)) }, 30, 1, 99999, 90);
	}

	//======================================================================================
	if (KHM::Boat.is_forward == true)
	{
		if (KHM::Boat.speed < 10.0)
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
	KHM::Boat.x += KHM::Boat.speed * sin(KHM::Boat.y_angle * RADIAN);

	if (KHM::Boat.direction == LEFT)
	{
		KHM::Boat.direction_percent -= 0.01;
		KHM::Boat.y_angle++;
	}
	else if (KHM::Boat.direction == RIGHT)
	{
		KHM::Boat.direction_percent += 0.01;
		KHM::Boat.y_angle--;
	}
	//KHM::temp_y_angle = KHM::Boat.direction_percent * 90;
	//KHM::Boat.z += KHM::Boat.speed * KHM::Boat.direction_percent;
	//KHM::Boat.x += KHM::Boat.speed * (1 - KHM::Boat.direction_percent);
	//===========================================================================================
	//cout << KHM::fired_y_angle << endl;
	glutPostRedisplay();                  // 화면 재출력
	glutTimerFunc(10, Timerfunction, 1);      // 타이머함수 재설정
}