
#include "stdafx.h"
#include <stdio.h>
#include "Bmp.h"


//unsigned char* readBMP(char* filename)
//{
//	int i;
//	FILE* f = fopen(filename, "rb");
//	unsigned char info[54];
//	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
//
//											   // extract image height and width from header
//	int width = *(int*)&info[18];
//	int height = *(int*)&info[22];
//
//	int size = 3 * width * height;
//	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
//	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
//	fclose(f);
//
//	for (i = 0; i < size; i += 3)
//	{
//		unsigned char tmp = data[i];
//		data[i] = data[i + 2];
//		data[i + 2] = tmp;
//	}
//
//	return data;
//}

ULONG* OpenBMP(char *filename, int *width, int *height)
{
	FILE* fp;
	errno_t err;

	err = fopen_s(&fp, filename, "rb");
	if (!fp) return NULL;

	BITMAPFILEHEADER bmpfh;
	size_t size = fread(&bmpfh, 1, sizeof(BITMAPFILEHEADER), fp);
	if (size != sizeof(BITMAPFILEHEADER)) return NULL;

	BITMAPINFOHEADER bmpih;
	size = fread(&bmpih, 1, sizeof(BITMAPINFOHEADER), fp);
	if (size != sizeof(BITMAPINFOHEADER)) return NULL;

	int bytePerLine = ((bmpih.biWidth * 3) + 3) &~3;
	size_t imageSize = bytePerLine * bmpih.biHeight;
	BYTE *pBmp = new BYTE[imageSize];
	if (!pBmp) return NULL;

	*width = bmpih.biWidth;
	*height = bmpih.biHeight;
	size = fread(pBmp, 1, imageSize, fp);
	if (size != imageSize)
	{
		delete[] pBmp;
		return NULL;
	}

	fclose(fp);

	ULONG *pImageBuf = new ULONG[bmpih.biWidth * bmpih.biHeight];

	for (int j = 0; j < bmpih.biHeight; j++)
	{
		for (int i = 0; i < bmpih.biWidth; i++)
		{
			int ColorIndex = bytePerLine * (bmpih.biHeight - j - 1) + (i * 3);
			BYTE BValue = pBmp[ColorIndex];
			BYTE GValue = pBmp[ColorIndex + 1];
			BYTE RValue = pBmp[ColorIndex + 2];

			int DestIndex = j * bmpih.biWidth + i;
			
			BYTE *CurrentColor = (BYTE *)(pImageBuf + DestIndex);
			CurrentColor[0] = 255;
			CurrentColor[1] = RValue;
			CurrentColor[2] = GValue;
			CurrentColor[3] = BValue;
			//pImageBuf[j * (bmpih.biWidth * 4) + i] = (ULONG)BValue | ((ULONG)GValue) << 8 | ((ULONG)RValue) << 16 | ((ULONG)255) << 24;
		}
	}

	if (pBmp)
	{
		delete[] pBmp;
	}
	return (ULONG*)pImageBuf;
}

ULONG GetPixel(int x, int width, int y, ULONG* bmp)
{
	return bmp[y * width + x];
}