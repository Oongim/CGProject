#pragma once
#include<iostream>
#include <GL/freeglut.h>

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
 GLuint textures[8];

 void init_textures()
 {
	 //텍스처 설정 정의를 한다. --- (3)
	 glGenTextures(10, textures);
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

	 glBindTexture(GL_TEXTURE_2D, textures[4]);
	 pBytes = LoadDIBitmap("START.bmp", &info);
	 glTexImage2D(GL_TEXTURE_2D, 0, 4, 285, 108, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	 glBindTexture(GL_TEXTURE_2D, textures[5]);
	 pBytes = LoadDIBitmap("GreenPeace.bmp", &info);
	 glTexImage2D(GL_TEXTURE_2D, 0, 4, 800, 533, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	 glBindTexture(GL_TEXTURE_2D, textures[6]);
	 pBytes = LoadDIBitmap("TrueEnding.bmp", &info);
	 glTexImage2D(GL_TEXTURE_2D, 0, 4, 533, 330, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	 glBindTexture(GL_TEXTURE_2D, textures[7]);
	 pBytes = LoadDIBitmap("name.bmp", &info);
	 glTexImage2D(GL_TEXTURE_2D, 0, 4, 197, 182, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
 }