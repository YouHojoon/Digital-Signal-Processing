#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main(void) {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	int imgSize, min, max, new_T, T, E, u1, u2, u1_cnt, u2_cnt;
	int Histo[256] = { 0, };

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
	BYTE* output = (BYTE*)malloc(imgSize);
	fread(image, sizeof(BYTE), imgSize, fp);
	fclose(fp);

	for (int i = 0; i < imgSize; i++)
		Histo[image[i]]++;

	for (int i = 0; i < 256; i++) {
		if (Histo[i] != 0) {
			min = i;
			break;
		}
	}

	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			max = i;
			break;
		}
	}

	T = (max + min) / 2;//초기 임계치
	scanf("%d", &E);

	printf("초기 임계치 : %d\n", T);

	while (1) {
		u1 = 0;
		u2 = 0;
		u1_cnt = 0;
		u2_cnt = 0;

		for (int i = 0; i < imgSize; i++) {
			if (image[i] > T) {//G1 영억
				u1 += image[i];
				u1_cnt++;
			}
			else {//G2영역
				u2 += image[i];
				u2_cnt++;
			}
		}
		u1 /= u1_cnt;
		u2 /= u2_cnt;
		new_T = (u1 + u2) / 2;
		int chk = (T - new_T < E) ? TRUE : FALSE;//사용자가 입력한 입실론보다 작은지 확인
		T = new_T;

		printf("새로운 임계치 : %d\n", T);
		if (chk) {
			printf("새로운 임계치가 입실론 값 %d보다 작으므로 계산을 종료합니다.\n", E);
			break;
		}
	}

	for (int i = 0; i < imgSize; i++) {
		if (image[i] > T)output[i] = 255;
		else output[i] = 0;
	}

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(output, sizeof(BYTE), imgSize, fp);
	fclose(fp);

	free(image);
	free(output);

}