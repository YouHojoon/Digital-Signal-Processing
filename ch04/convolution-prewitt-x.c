//#include <stdio.h>
//#include <stdlib.h>
//#include <Windows.h>
//
//#define MASK_SIZE 3
//int main(void) {
//	BITMAPFILEHEADER hf;
//	BITMAPINFOHEADER hinfo;
//	RGBQUAD hRGB[256];
//	FILE* fp;
//	int imgSize;
//	int margin = MASK_SIZE / 2;
//	int mask[MASK_SIZE][MASK_SIZE] = { {-1,0,1 }, {-1, 0, 1}, {-1, 0, 1} };
//	double sum = 0.0;
//
//	fp = fopen("lenna.bmp", "rb");
//	if (fp == NULL) {
//		perror("fopen");
//		exit(1);
//	}
//
//	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
//	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
//	fread(hRGB, sizeof(RGBQUAD), 256, fp);
//
//	imgSize = hinfo.biWidth * hinfo.biHeight;
//	BYTE* image = (BYTE*)malloc(imgSize);
//	BYTE* output = (BYTE*)malloc(imgSize);
//	fread(image, sizeof(BYTE), imgSize, fp);
//	fclose(fp);
//
//	for (int i = margin; i < hinfo.biHeight - margin; i++) {
//		for (int j = margin; j < hinfo.biWidth - margin; j++) {
//			for (int m = -margin; m <= margin; m++) {
//				for (int n = -margin; n <= margin; n++) {
//					sum += image[(i + m) * hinfo.biWidth + (j + n)] * mask[margin+m][margin+n];
//				}
//			}
//			output[i * hinfo.biWidth + j] = (BYTE)(abs(sum) / 3.0);
//			sum = 0.0;
//		}
//	}
//
//	fp = fopen("output.bmp", "wb");
//	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
//	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
//	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
//	fwrite(output, sizeof(BYTE), imgSize, fp);
//	fclose(fp);
//
//	free(image);
//	free(output);
//
//  return 0;
//}