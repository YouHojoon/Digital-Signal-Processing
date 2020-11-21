#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

//int main(void) {
//	BITMAPFILEHEADER hf;
//	BITMAPINFOHEADER hinfo;
//	RGBQUAD hRGB[256];
//	FILE* fp;
//	int imgSize,Nt, Gmax=255;
//	int Histo[256] = {0,};//히스토그램
//	int AHisto[256];//누적 히스토그램
//	int NormSum[256];
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
//	
//	Nt = imgSize;
//	double ratio = Gmax / (double)Nt;
//	for (int i = 0; i < imgSize; i++)
//		Histo[image[i]]++;
//	AHisto[0] = Histo[0];
//
//	for (int i = 1; i < 256; i++) {
//		AHisto[i] = AHisto[i - 1] + Histo[i];
//		NormSum[i] = ratio * AHisto[i];
//	}
//
//	for (int i = 0; i < imgSize; i++)
//		output[i] = (BYTE)NormSum[image[i]];
//
//
//	fp = fopen("output.bmp", "wb");
//	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
//	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
//	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
//	fwrite(output, sizeof(BYTE), imgSize, fp);
//	fclose(fp);
//
//	fp = fopen("histo.txt", "wt");
//	for (int i = 0; i < 256; i++)
//		fprintf(fp, "%d\t%d\t%d\n", Histo[i], AHisto[i], NormSum[i]);
//	
//	fclose(fp);
//	free(image);
//	free(output);

//  return 0;

//}