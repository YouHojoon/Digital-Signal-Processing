#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

void rgbToYcbcr(BYTE* image, BYTE* Y, BYTE* Cb, BYTE* Cr, int imgSize, int W, int H);
void drawBox(BYTE* biImage, BYTE* output, int W, int H);
int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top);
int pop(short* stackx, short* stacky, short* vx, short* vy, int* top);
void m_BlobColoring(BYTE* CutImage, int height, int width);
void skinDetection(BYTE* image, BYTE* output, int W, int H, BYTE* Y, BYTE* Cb, BYTE* Cr);

int main(void) {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinfo;
	FILE* fp;
	int imgSize, W, H;


	fp = fopen("face.bmp", "rb");
	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);

	W = hinfo.biWidth;
	H = hinfo.biHeight;

	imgSize = W * H * 3;

	BYTE* image = (BYTE*)malloc(imgSize);
	BYTE* tmp = (BYTE*)malloc(imgSize / 3);
	BYTE* output = (BYTE*)malloc(imgSize);

	BYTE* Y = (BYTE*)malloc(imgSize * sizeof(BYTE));
	BYTE* Cb = (BYTE*)malloc(imgSize * sizeof(BYTE));
	BYTE* Cr = (BYTE*)malloc(imgSize * sizeof(BYTE));

	fread(image, sizeof(BYTE), imgSize, fp);
	fclose(fp);

	rgbToYcbcr(image, Y, Cb, Cr, imgSize, W, H);
	skinDetection(image, output, W, H, Y, Cb, Cr);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++)
			(output[3 * (i * W + j)] == 0) ? tmp[i * W + j] = 0 : tmp[i * W + j] = 255;
	}

	m_BlobColoring(tmp, H, W);
	drawBox(tmp, image, W, H);

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(image, sizeof(BYTE), imgSize, fp);
	fclose(fp);

	free(image);
	free(output);
	free(Y);
	free(Cb);
	free(Cr);

	return 0;
}

void rgbToYcbcr(BYTE* image, BYTE* Y, BYTE* Cb, BYTE* Cr, int imgSize, int W, int H) {
	for (int i = 0; i < H; i++) { // Y좌표
		for (int j = 0; j < W; j++) { // X좌표
			Y[i * W + j] = (BYTE)(0.299 * image[i * W * 3 + j * 3 + 2] + 0.587 * image[i * W * 3 + j * 3 + 1] + 0.114 * image[i * W * 3 + j * 3]);
			Cb[i * W + j] = (BYTE)(-0.16874 * image[i * W * 3 + j * 3 + 2] - 0.3313 * image[i * W * 3 + j * 3 + 1] + 0.5 * image[i * W * 3 + j * 3] + 128.0);
			Cr[i * W + j] = (BYTE)(0.5 * image[i * W * 3 + j * 3 + 2] - 0.4187 * image[i * W * 3 + j * 3 + 1] - 0.0813 * image[i * W * 3 + j * 3] + 128.0);
		}
	}

}
void drawBox(BYTE* biImage, BYTE* output, int W, int H) {
	int x1, x2, y1, y2;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (biImage[i * W + j] == 0) {
				y2 = i;
				break;
			}
		}
	}

	for (int i = H - 1; i >= 0; i--) {
		for (int j = W - 1; j >= 0; j--) {
			if (biImage[i * W + j] == 0) {
				y1 = i;
				break;
			}
		}
	}

	for (int i = 0; i < W; i++) {
		for (int j = 0; j < H; j++) {
			if (biImage[j * W + i] == 0) {
				x2 = i;
				break;
			}
		}
	}

	for (int i = W - 1; i >= 0; i--) {
		for (int j = H - 1; j >= 0; j--) {
			if (biImage[j * W + i] == 0) {
				x1 = i;
				break;
			}
		}
	}

	for (int i = x1; i <= x2; i++) {
		int tmp = 3 * (y1 * W + i);
		output[tmp + 2] = 255;
		output[tmp] = output[tmp + 1] = 0;
	}


	for (int i = x1; i <= x2; i++) {
		int tmp = 3 * (y2 * W + i);
		output[tmp + 2] = 255;
		output[tmp] = output[tmp + 1] = 0;
	}


	for (int i = y1; i <= y2; i++) {
		int tmp = 3 * (i * W + x1);
		output[tmp + 2] = 255;
		output[tmp] = output[tmp + 1] = 0;
	}

	for (int i = y1; i <= y2; i++) {
		int tmp = 3 * (i * W + x2);
		output[tmp + 2] = 255;
		output[tmp] = output[tmp + 1] = 0;
	}

}

int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}


// GlassFire 알고리즘을 이용한 라벨링 함수

void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;


	// 스택으로 사용할 메모리 할당
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // 메모리 초기화

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//관심 픽셀이 영상경계를 벗어나면 처리 안함
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // 현재 라벨로 마크
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 255.0f / (float)curColor;

	// 가장 면적이 넓은 영역을 찾아내기 위함 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage 배열 클리어~
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	// coloring에 저장된 라벨링 결과중 (Out_Area에 저장된) 영역이 가장 큰 것만 CutImage에 저장
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // 가장 큰 것만 저장
		//CutImage[k] = (unsigned char)(coloring[k] * grayGap);
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}

void skinDetection(BYTE* image, BYTE* output, int W, int H, BYTE* Y, BYTE* Cb, BYTE* Cr) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			/*
				Human Skin Detection Using RGB, HSV and YCbCr Color Models 논문 참조
				https://arxiv.org/ftp/arxiv/papers/1708/1708.02694.pdf

			*/
			if (image[3 * (i * W + j) + 2] > 95 && image[3 * (i * W + j) + 1] > 40 && image[3 * (i * W + j)] > 20
				&& image[3 * (i * W + j) + 2] > image[3 * (i * W + j) + 1] && image[3 * (i * W + j) + 2] > image[3 * (i * W + j)]
				&& abs(image[3 * (i * W + j) + 2] - image[3 * (i * W + j) + 1]) > 15 && Cr[i * W + j] > 135 && Cb[i * W + j] > 85
				&& Y[i * W + j] > 80 && Cr[i * W + j] <= (1.5862 * Cb[i * W + j]) + 20 && Cr[i * W + j] >= (0.3448 * Cb[i * W + j]) + 76.2069
				&& Cr[i * W + j] >= (-4.5652 * Cb[i * W + j]) + 234.5652 && Cr[i * W + j] <= (-1.15 * Cb[i * W + j]) + 301.75
				&& Cr[i * W + j] <= (-2.2857 * Cb[i * W + j]) + 432.85) {
				output[i * W * 3 + j * 3] = image[i * W * 3 + j * 3];
				output[i * W * 3 + j * 3 + 1] = image[i * W * 3 + j * 3 + 1];
				output[i * W * 3 + j * 3 + 2] = image[i * W * 3 + j * 3 + 2];
			}
			else
				output[i * W * 3 + j * 3] = output[i * W * 3 + j * 3 + 1] = output[i * W * 3 + j * 3 + 2] = 0;
		}
	}
}