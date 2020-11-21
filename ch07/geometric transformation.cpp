#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

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
// 라벨링 후 가장 넓은 영역에 대해서만 뽑아내는 코드 포함

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
void thresholding(BYTE* image, BYTE* output, int imgSize, int Th) {
	for (int i = 0; i < imgSize; i++)
		(image[i] > Th) ? output[i] = 0 : output[i] = 255;
}
int cntPixel(BYTE* image, int Cx, int Cy, int W) {
	int cnt=0;
	for (int i = Cy - 1; i < Cy + 1; i++) {
		for (int j = Cx - 1; j < Cx + 1; j++)
			if (image[i * W + j] == 255)cnt++;
	}
	return --cnt;
}
void edgeDetection(BYTE* image, BYTE* output, int W, int H) {
	for (int i = 0; i < W * H; i++)output[i] = 0;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (image[i * W + j] == 255) {
				if (cntPixel(image, i, j, W) != 8)
					output[i * W + j] = 255;
			}
		}
	}
}
void colorToGray(BYTE* image, BYTE* output, int W, int H) {
	int tmp;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			tmp = i * W + j;
			output[tmp] = (BYTE)((image[tmp * 3]+image[tmp*3+1]+image[tmp*3+2])/3.0);
		}
	}
}
void grayToColor(BYTE* image, BYTE* output, int W, int H) {
	int tmp;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			tmp = i * W + j;
			output[tmp * 3] = output[tmp * 3 + 1] = output[tmp * 3 + 2] = image[tmp];
		}
	}
}
void drawCross(BYTE* image,int W, int H, int Cx, int Cy, int R, int G, int B) {
	printf("%d %d", Cx, Cy);
	for (int i = 0; i < W; i++) {
		image[3 * (Cy * W + i)] = B;
		image[3 * (Cy * W + i)+1] = G;
		image[3 * (Cy * W + i)+2] = R;
	}

	for (int i = 0; i < H; i++) {
		image[3 * (i*W+Cx)] = B;
		image[3 * (i * W + Cx)+1] = G;
		image[3 * (i * W + Cx)+2] = R;
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
	
	/*BYTE* gray = (BYTE*)malloc(imgSize);
	BYTE* label = (BYTE*)malloc(imgSize);

	
	fclose(fp);

	colorToGray(image, gray, hinfo.biWidth, hinfo.biHeight);
	thresholding(gray, gray, imgSize, 80);
	m_BlobColoring(gray, hinfo.biHeight, hinfo.biWidth);

	int sumX=0, sumY=0, cnt=0;
	for (int i = 0; i < hinfo.biHeight; i++) {
		for (int j = 0; j < hinfo.biWidth; j++) {
			if (gray[i * hinfo.biWidth + j] == 255) {
				sumX += j;
				sumY += i;
				cnt++;
			}
		}
	}*/

	imageTranslation(image, output, hinfo.biWidth, hinfo.biHeight);
	fp = fopen("trans.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	if (hinfo.biBitCount == 8)
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(output, sizeof(BYTE), imgSize, fp);
	fclose(fp);
	imageRotation(image, output, hinfo.biWidth, hinfo.biHeight);

	/*drawCross(output, hinfo.biWidth, hinfo.biHeight, sumX / cnt, sumY / cnt, 255, 0, 255);*/

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

