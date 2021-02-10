#pragma once
#include<iostream>
#include <GL/freeglut.h>

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
 GLuint textures[8];

 void init_textures()
 {
	 //�ؽ�ó ���� ���Ǹ� �Ѵ�. --- (3)
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