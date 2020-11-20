//#include <stdio.h>
//#include <stdlib.h>
//#include <Windows.h>
//
//#define MASK_SIZE 3
//
//void merge_sort(BYTE array[], int start, int end);
//void merge(BYTE array[], int left, int right);
//
//int main(void) {
//	BITMAPFILEHEADER hf;
//	BITMAPINFOHEADER hinfo;
//	RGBQUAD hRGB[256];
//	FILE* fp;
//	int imgSize;
//	int margin = MASK_SIZE / 2;
//	int mask1[MASK_SIZE][MASK_SIZE] = { -1,-1,-1,0,0,0,1,1,1 };
//	int mask2[MASK_SIZE][MASK_SIZE] = { -1,0,1,-1,0,1,-1,0,1 };
//	double sum1 = 0.0, sum2 = 0.0;
//
//	fp = fopen("lenna_noise.bmp", "rb");
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
//	BYTE tmp[MASK_SIZE * MASK_SIZE];
//
//	for (int i = margin; i < hinfo.biHeight - margin; i++) {
//		for (int j = margin; j < hinfo.biWidth - margin; j++) {
//			for (int m = -margin; m <= margin; m++) {
//				for (int n = -margin; n <= margin; n++) {
//					tmp[(m + 1) * MASK_SIZE + (n + 1)] = image[(i + m) * hinfo.biWidth + (j + n)];
//				}
//			}
//			merge_sort(tmp, 0, sizeof(tmp) - 1);
//			output[i * hinfo.biWidth + j] = tmp[MASK_SIZE * MASK_SIZE / 2];
//		}
//	}
//
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
//}
//
//void merge_sort(BYTE array[], int start, int end) {
//	if (start == end)return;
//
//	int mid = (start+end) / 2;
//	merge_sort(array, start, mid);
//	merge_sort(array, mid + 1, end);
//	merge(array, start, end);
//
//
//}
//
//void merge(BYTE array[], int left, int right) {
//	int l = left;
//	int mid = (left + right) / 2;
//	int r = mid + 1;
//	int cnt = 0;
//
//	BYTE* tmp = (BYTE*)malloc((right - left + 1) * sizeof(BYTE));
//
//	while (l <= mid && r <= right) {
//		if (array[l] < array[r])
//			tmp[cnt++] = array[l++];
//		else
//			tmp[cnt++] = array[r++];
//	}
//
//	if (l > mid)
//		while (r <= right)
//			tmp[cnt++] = array[r++];
//	if (r > mid)
//		while (l <= mid)
//			tmp[cnt++] = array[l++];
//
//	for (int i = 0; i < cnt; i++)
//		array[left++] = tmp[i];
//}
