#pragma once
#include <GL/freeglut.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>
#include <algorithm>

#include"LoadDlBitmap.h"

#define PI 3.141592
#define RADIAN PI / 180
#define WORLD_SCALE 4000
struct Pyramid
{
	int x, z, size;
};

Pyramid pyramid[4]
= {
	{ -2000,800,1000 },
	{ -1000,-2500,1000 },
	{ 2500,100,1000 },
	{ 1000,3000,1000 }
};

bool collide_pyramid(float x, float y, float z)
{
	for (int i = 0; i < 4; ++i) {
		if (x - 100 < pyramid[i].x + pyramid[i].size * -y / pyramid[i].size && x + 100 > pyramid[i].x - pyramid[i].size * -y / pyramid[i].size &&
			z + 100 >pyramid[i].z - pyramid[i].size * -y / pyramid[i].size && z - 100 < pyramid[i].z + pyramid[i].size * -y / pyramid[i].size)
		{
			return 1;
		}
	}

	return 0;
}


class Whale
{
private:
	float x, y, z;
	float next_x, next_y, next_z;
	float normal_r;
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
	bool isOut_pyramid;


public:
	Whale()
	{
		HP = 100;
		tail_moveVelocity = 3;
		move_Velocity = 3;
		Tail_Radian = -45;
		isAlive = true;
		isDeath_animation = false;

		do
		{
			x = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
			z = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
			y = -rand() % 800;
		} while (collide_pyramid(x, y, z));

		next_x = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
		next_z = rand() % (WORLD_SCALE * 2) - WORLD_SCALE;
		next_y = -rand() % 800;

		normal_r = sqrt(pow(next_x - x, 2) + pow(next_y - y, 2) + pow(next_z - z, 2));

		phi = ((atan2((next_x - x), (next_z - z))) * 180 / PI);

		theta = acos((next_y - y) / normal_r) * 180 / PI - 90;

	}

	int check_next_pos(int normal_r)
	{
		if (collide_pyramid(x, y, z) || y <= -800 || x > 4000 || x < -4000 || z > 4000 || z < -4000||
			(x > next_x - move_Velocity && x<next_x + move_Velocity &&
			y>next_y - move_Velocity && y<next_y + move_Velocity &&
			z>next_z - move_Velocity && z < next_z + move_Velocity))
		{
			if (isRun_away)
			{
				isRun_away = false;
				move_Velocity -= 2;
			}
			x -= (next_x - x) / normal_r * move_Velocity;
			y -= (next_y - y) / normal_r * move_Velocity;
			z -= (next_z - z) / normal_r * move_Velocity;
			next_x = rand() % WORLD_SCALE - WORLD_SCALE / 2;
			next_z = rand() % WORLD_SCALE - WORLD_SCALE / 2;
			next_y = -rand() % 400;

			normal_r = sqrt(pow(next_x - x, 2) + pow(next_y - y, 2) + pow(next_z - z, 2));

			phi = ((atan2((next_x - x), (next_z - z))) * 180 / PI);

			theta = acos((next_y - y) / normal_r) * 180 / PI - 90;
		}
	}

	void set_run_away(float boat_x, float boat_y, float boat_z, int num, int part)
	{
		isRun_away = true;
		move_Velocity += 2;
		next_x = x + x - boat_x;
		next_y = y + y - boat_y;
		next_z = z + z - boat_z;
		if (part == 0)
			HP -= rand() % 10 + 20;
		else
			HP -= rand() % 10 + 10;

		if (HP < 0)
		{
			isAlive = false;
			isDeath_animation = true;
			//sum_of_capture_Whale++;
		}
	}

	void drawRect(float size, float x, float y, float z)
	{
		glPushMatrix(); {
			glTranslatef(x, y, z);
			glBegin(GL_QUADS);
			//À­¸é
			glVertex3f(-size / 2, size / 2, -size / 2); //1
			glVertex3f(-size / 2, size / 2, size / 2);// 2
			glVertex3f(size / 2, size / 2, size / 2);// 3
			glVertex3f(size / 2, size / 2, -size / 2);//4
													  //¾Õ¸é		 
			glVertex3f(-size / 2, size / 2, size / 2);//2
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(size / 2, -size / 2, size / 2);//7
			glVertex3f(size / 2, size / 2, size / 2);//3
													 //¿À¸¥ÂÊ¿·¸é
			glVertex3f(size / 2, size / 2, size / 2);//3
			glVertex3f(size / 2, -size / 2, size / 2);//7
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(size / 2, size / 2, -size / 2);//4
													  //¿ÞÂÊ¿·¸é
			glVertex3f(-size / 2, size / 2, -size / 2);//1
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(-size / 2, size / 2, size / 2);//2
													  //µÞ¸é
			glVertex3f(size / 2, size / 2, -size / 2);//4
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(-size / 2, size / 2, -size / 2);//1

													   //¹Ù´Ú¸é
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
			//À­¸é
			glVertex3f(-size / 2, size / 2, -size / 2); //1
			glVertex3f(-size / 2, size / 2, size / 2);// 2
			glVertex3f(size / 2, size / 2, size / 2);// 3
			glVertex3f(size / 2, size / 2, -size / 2);//4
													  //¾Õ¸é	
			glVertex3f(-size / 2, size / 2, size / 2);//2
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(size / 2, -size / 2, size / 2);//7
			glVertex3f(size / 2, size / 2, size / 2);//3
													 //¿À¸¥ÂÊ¿·¸é
			glVertex3f(size / 2, size / 2, size / 2);//3
			glVertex3f(size / 2, -size / 2, size / 2);//7
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(size / 2, size / 2, -size / 2);//4
													  //¿ÞÂÊ¿·¸é
			glVertex3f(-size / 2, size / 2, -size / 2);//1
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(-size / 2, size / 2, size / 2);//2
													  //µÞ¸é
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(size / 2, size / 2, -size / 2);//4
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-size / 2, size / 2, -size / 2);//1

													   //¹Ù´Ú¸é
			glVertex3f(-size / 2, -size / 2, size / 2);//6
			glVertex3f(-size / 2, -size / 2, -size / 2);//5
			glVertex3f(size / 2, -size / 2, -size / 2);//8
			glVertex3f(size / 2, -size / 2, size / 2);//7

			glEnd();
		}glPopMatrix();

	}
	void draw()
	{
		glEnable(GL_TEXTURE_2D);
		if (isAlive) {
			glBindTexture(GL_TEXTURE_2D, textures[1]);
		}
		else if (isDeath_animation) {
			glBindTexture(GL_TEXTURE_2D, textures[3]);
		}
		glPushMatrix(); {
			glTranslatef(x, y, z);
			if (isDeath_animation) {
				glRotated(180, 0, 0, 1);
			}
			else
			{
				glRotatef(phi + 180, 0.0, 1.0, 0.0);

				glRotatef(-theta, 1.0, 0.0, 0.0);
			}
			///////////////////Å« ¸öÅë/////////////////////////////////////////////
			glColor3f(1.0, 1.0, 1.0);
			draw_face_Rect(100);
			/////////////////////////Áö´À·¯¹Ì 2°³//////////////////////////
			glPushMatrix(); {
				glTranslatef(0, -30, 20);
				/****************¿ÞÂÊ*******************/
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
				/***************¿À¸¥ÂÊ*****************/
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
			/**********************Áß°£ ¸öÅë***********************************/
			glPushMatrix(); {
				glTranslatef(0, -10 + tail_moveRange[0], 70);

				glRotatef(0, 0.0, 0.0, 1.0);
				glPushMatrix(); {
					glScalef(1, 1, 0.5);
					//glColor3f(1.0, 1.0, 1.0);
					drawRect(80, 0, 0, 0);
				}glPopMatrix();
				/*****************²¿¸® ¸öÅë**********************************/
				glPushMatrix(); {
					glTranslatef(0, -10 + tail_moveRange[1], 40);

					glRotatef(0, 0.0, 0.0, 1.0);
					//glColor3f(1.0, 1.0, 1.0);
					drawRect(40, 0, 0, 0);
					/*************************************************************/
					glPushMatrix(); {
						glTranslatef(0, 0, 20);
						glRotatef(Tail_Radian, 1.0, 0.0, 0.0);
						glTranslatef(0, 0, 20);
						//////////////////²¿¸® Áö´À·¯¹Ì 2°³+°¡¿îµ¥ 1°³////////////////// 
						/******************Áö´À·¯¹Ì ¿ÞÂÊ**********************/
						glPushMatrix(); {
							glTranslatef(20, 0, 0);
							glRotatef(30, 0.0, 1.0, 0.0);
							glScalef(0.7, 0.5, 1);
							//glColor3f(1.0, 1.0, 1.0);
							drawRect(40, 0, 0, 0);
						}glPopMatrix();
						/********************Áö´À·¯¹Ì ¿À¸¥ÂÊ***************************/
						glPushMatrix(); {
							glTranslatef(-20, 0, 0);
							glRotatef(-30, 0.0, 1.0, 0.0);
							glScalef(0.7, 0.5, 1);
							//glColor3f(1.0, 1.0, 1.0);
							drawRect(40, 0, 0, 0);
						}glPopMatrix();
						/***********************Áö´À·¯¹Ì °¡¿îµ¥**********************/
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

		glDisable(GL_TEXTURE_2D);
	}

	void update()
	{
		if (isAlive) {
			if (tail_moveRange[0] <= 20 && tail_moveRange[0] >= 0) {
				tail_moveRange[0] += tail_moveVelocity;
				Tail_Radian += tail_moveVelocity * 4;

			}
			else {
				tail_moveRange[1] += tail_moveVelocity;
				if (tail_moveRange[1] > 30 || tail_moveRange[1] < 0)
				{
					tail_moveVelocity *= -1;
					if (tail_moveVelocity > 0)
						tail_moveRange[0] = 0;
					else
						tail_moveRange[0] = 20;
				}
			}
			x += (next_x - x) / normal_r * move_Velocity;
			y += (next_y - y) / normal_r * move_Velocity;
			z += (next_z - z) / normal_r * move_Velocity;
			
			check_next_pos(normal_r);
		}
		else if (isDeath_animation) {
			y += 1;
			if (y > 0)
				isDeath_animation = false;
		}
	}
};



