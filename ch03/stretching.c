#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main(void) {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	int imgSize, Low, High;
	int Histo[256] = { 0, };//히스토그램
	
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

	for (int i = 0; i < 256; i++)
		if (Histo[i] != 0){
			Low = i;
			break;
		}
	for(int i=255; i>=0; i--)
		if (Histo[i] != 0) {
			High = i;
			break;
		}
	for (int i = 0; i < imgSize; i++)
		output[i] = (BYTE)(((image[i] - Low) / (double)(High - Low)) * 255);
	

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(output, sizeof(BYTE), imgSize, fp);
	fclose(fp);


	fclose(fp);
	free(image);
	free(output);

}