//#include <stdio.h>
//#include <stdlib.h>
//#include <Windows.h>
//
//int main(void) {
//	BITMAPFILEHEADER hf;
//	BITMAPINFOHEADER hinfo;
//	FILE* fp;
//	int imgSize,W,H,i;
//
//	fp = fopen("color.bmp", "rb");
//	if (fp == NULL) {
//		perror("fopen");
//		exit(1);
//	}
//
//	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
//	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
//	W = hinfo.biWidth;
//	H = hinfo.biHeight;
//	imgSize = W * H * 3;
//
//	BYTE* image = (BYTE*)malloc(imgSize);
//	BYTE* output = (BYTE*)malloc(imgSize);
//	fread(image, sizeof(BYTE), imgSize, fp);
//	fclose(fp);
//	
//	for (i = 0; i < H / 3; i++) {//Blue --> Yellow
//		for (int j = 0; j < W; j++) {
//			int tmp = i * W + j;
//			output[3 * tmp] = (BYTE)(255 - j / (W / 255.0));
//			output[3 * tmp+1] = (BYTE)(j / (W / 255.0));
//			output[3 * tmp+2] = (BYTE)(j / (W / 255.0));
//		}
//	}
//	for (; i < 2 *H / 3; i++) {//Green --> Magenta
//		for (int j = 0; j < W; j++) {
//			int tmp = i * W + j;
//			output[3 * tmp] =  (BYTE)(j / (W / 255.0));
//			output[3 * tmp + 1] = (BYTE)(255 - j / (W / 255.0));
//			output[3 * tmp + 2] = (BYTE)(j / (W / 255.0));
//		}
//	}
//	for (; i < H; i++) {//Red --> Cyan
//		for (int j = 0; j < W; j++) {
//			int tmp = i * W + j;
//			output[3 * tmp] = (BYTE)(j / (W / 255.0));
//			output[3 * tmp + 1] = (BYTE)(j / (W / 255.0));
//			output[3 * tmp + 2] = (BYTE)(255 - j / (W / 255.0));
//		}
//	}
//
//	fp = fopen("output.bmp", "wb");
//	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
//	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
//	fwrite(output, sizeof(BYTE), imgSize, fp);
//	fclose(fp);
//
//	free(image);
//	free(output);
//
//	return 0;
//}