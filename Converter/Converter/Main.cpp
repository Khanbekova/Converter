#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Main.h"

static unsigned short read_u16(FILE *fp)
{
	unsigned char b0, b1;

	b0 = getc(fp);
	b1 = getc(fp);

	return ((b1 << 8) | b0);
}


static unsigned int read_u32(FILE *fp)
{
	unsigned char b0, b1, b2, b3;

	b0 = getc(fp);
	b1 = getc(fp);
	b2 = getc(fp);
	b3 = getc(fp);

	return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


static int read_s32(FILE *fp)
{
	unsigned char b0, b1, b2, b3;

	b0 = getc(fp);
	b1 = getc(fp);
	b2 = getc(fp);
	b3 = getc(fp);

	return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

int main()
{
	char buffer[BUFSIZ];
	int nread = 0;
	FILE *bmpfile = fopen("C:\\Users\\Елена\\Desktop\\bee-on-daisy.bmp", "rb");
	FILE *bmpfile2 = fopen("C:\\Users\\Елена\\Desktop\\bee-on-daisy.bmp", "rb");
	FILE *txtfile = fopen("C:\\Users\\Елена\\Desktop\\bmptxt.txt", "w");
	FILE *binfile = fopen("C:\\Users\\Елена\\Desktop\\bmpbin.bin", "wb");
	// заголовок файла
	BITMAPFILEHEADER header[[gnu::unused]];

	header.bfType = read_u16(bmpfile);
	header.bfSize = read_u32(bmpfile);
	header.bfReserved1 = read_u16(bmpfile);
	header.bfReserved2 = read_u16(bmpfile);
	header.bfOffBits = read_u32(bmpfile);

	//заголовок изображения
	BITMAPINFOHEADER bmiHeader;

	bmiHeader.biSize = read_u32(bmpfile);
	bmiHeader.biWidth = read_s32(bmpfile);
	bmiHeader.biHeight = read_s32(bmpfile);
	bmiHeader.biPlanes = read_u16(bmpfile);
	bmiHeader.biBitCount = read_u16(bmpfile);
	bmiHeader.biCompression = read_u32(bmpfile);
	bmiHeader.biSizeImage = read_u32(bmpfile);
	bmiHeader.biXPelsPerMeter = read_s32(bmpfile);
	bmiHeader.biYPelsPerMeter = read_s32(bmpfile);
	bmiHeader.biClrUsed = read_u32(bmpfile);
	bmiHeader.biClrImportant = read_u32(bmpfile);

	//коды цвета
	RGBQUAD **rgb = new RGBQUAD*[bmiHeader.biWidth];
	for (int i = 0; i < bmiHeader.biWidth; i++) {
		rgb[i] = new RGBQUAD[bmiHeader.biHeight];
	}

	for (int i = 0; i < bmiHeader.biWidth; i++) {
		for (int j = 0; j < bmiHeader.biHeight; j++) {
			rgb[i][j].rgbBlue = getc(bmpfile);
			rgb[i][j].rgbGreen = getc(bmpfile);
			rgb[i][j].rgbRed = getc(bmpfile);
		}

		// пропускаем последний байт в строке
		getc(bmpfile);
	}

	// запись в txt
	for (int i = 0; i < bmiHeader.biWidth; i++) {
		for (int j = 0; j < bmiHeader.biHeight; j++) {
			fprintf(txtfile, "0x%x, 0x%x, 0x%x,", rgb[i][j].rgbRed, rgb[i][j].rgbGreen, rgb[i][j].rgbBlue);
		}
	}

	//запись в bin
	while (nread = fread(buffer, sizeof(char), sizeof(buffer), bmpfile2))  
	{
		fwrite(buffer, sizeof(char), nread, binfile);
	}

	fclose(bmpfile);
	fclose(txtfile);
	fclose(binfile);
	system("pause");
	return 0;
}


