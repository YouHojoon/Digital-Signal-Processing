#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

void imageTranslation(BYTE* image, BYTE* output, int W, int H) {
	int Tx, Ty;
	printf("X 이동량: ");
	scanf("%d", &Tx);
	printf("Y 이동량: ");
	scanf("%d", &Ty);
	Ty = -Ty;

	int CTx, CTy;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			CTx = j + Tx;
			CTy = i + Ty;
			if (CTx < 0) CTx = 0;
			if (CTx >= W) CTx = W - 1;
			if (CTy < 0)CTy = 0;
			if (CTy >= H)CTy = H - 1;

			output[CTy * W + CTx] = image[i*W+j];
		}
	}
}
void imageRotation(BYTE* image, BYTE* output, int W, int H) {
	int theta;
	printf("회전각을 정수로 입력 : ");
	scanf("%d", &theta);

	double Redian = 3.14 / 180.0 * theta;
	int Rx, Ry;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Rx =j*cos(Redian) + i*sin(Redian);
			Ry = -j*sin(Redian) + i*cos(Redian);
			if (Rx >= W || Rx < 0 || Ry >= H || Ry < 0)output[i * W + j] = 0;
			else output[i * W + j] = image[Ry * W + Rx];
		}
	}
}

int main(void) {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	BYTE* image, *output;
	int imgSize;

	fp = fopen("pupil1.bmp", "rb");
	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	if(hinfo.biBitCount==8)
		fread(hRGB, sizeof(RGBQUAD), 256, fp);

	imgSize = hinfo.biWidth * hinfo.biHeight;
	if (hinfo.biBitCount == 24)
		imgSize *= 3;
	image = (BYTE*)malloc(imgSize);
	output = (BYTE*)malloc(imgSize);
	fread(image, sizeof(BYTE), imgSize, fp);

	imageTranslation(image, output, hinfo.biWidth, hinfo.biHeight);
	fp = fopen("trans.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	if (hinfo.biBitCount == 8)
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(output, sizeof(BYTE), imgSize, fp);
	fclose(fp);
	imageRotation(image, output, hinfo.biWidth, hinfo.biHeight);


	fp = fopen("rotate.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	if(hinfo.biBitCount==8)
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(output, sizeof(BYTE), imgSize, fp);
	fclose(fp);

	free(image);
	free(output);

	return 0;

}

