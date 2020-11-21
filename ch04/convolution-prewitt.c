#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define MASK_SIZE 3
int main(void) {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	int imgSize;
	int margin = MASK_SIZE / 2;
	int mask1[MASK_SIZE][MASK_SIZE] = { -1,-1,-1,0,0,0,1,1,1 };
	int mask2[MASK_SIZE][MASK_SIZE] = { -1,0,1,-1,0,1,-1,0,1 };
	double sum1 = 0.0, sum2 = 0.0;

	fp = fopen("lenna.bmp", "rb");
	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

	imgSize = hinfo.biWidth * hinfo.biHeight;
	BYTE* image = (BYTE*)malloc(imgSize);
	BYTE* output1 = (BYTE*)malloc(imgSize);
	BYTE* output2 = (BYTE*)malloc(imgSize);
	BYTE* result = (BYTE*)malloc(imgSize);
	fread(image, sizeof(BYTE), imgSize, fp);
	fclose(fp);

	for (int i = margin; i < hinfo.biHeight - margin; i++) {
		for (int j = margin; j < hinfo.biWidth - margin; j++) {
			for (int m = -margin; m <= margin; m++) {
				for (int n = -margin; n <= margin; n++) {
					sum1 += image[(i + m) * hinfo.biWidth + (j + n)] * mask1[margin + m][margin + n];
					sum2 += image[(i + m) * hinfo.biWidth + (j + n)] * mask2[margin + m][margin + n];
				}
			}
			output1[i * hinfo.biWidth + j] = (BYTE)(abs(sum1) / 3.0);
			output2[i * hinfo.biWidth + j] = (BYTE)(abs(sum2) / 3.0);
			sum1 = 0.0;
			sum2 = 0.0;
		}
	}


	for (int i = 0; i < imgSize; i++) {
		if (output1[i] > 30 || output2[i] > 30)result[i] = 255;
		else result[i] = 0;
	}
	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(result, sizeof(BYTE), imgSize, fp);
	fclose(fp);

	free(image);
	free(output1);
	free(output2);
	free(result);

	return 0;
}