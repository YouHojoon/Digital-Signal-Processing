#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// 2차원 배열 동적할당 위함
unsigned char** imageMatrix;
// 이진영상에서 
unsigned char blankPixel = 0, imagePixel = 255;

typedef struct {
	int row, col;
}pixel;

int getBlackNeighbours(int row, int col);
int getBWTransitions(int row, int col);
int zhangSuenTest1(int row, int col);
int zhangSuenTest2(int row, int col);
void zhangSuen(unsigned char* image, unsigned char* output, int rows, int cols);
void dilation(BYTE* image, BYTE* output, int W, int H);
void erosion(BYTE* image, BYTE* output, int W, int H);
int cntPixel(BYTE* image, int Cx, int Cy, int W, int H);
void chkBifurcationAndEndPoint(BYTE* image, BYTE* output, int W, int H);
void inverseImage(BYTE* image, BYTE* output, int imgSize);

int main(void) {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	int imgSize;

	fp = fopen("dilation.bmp", "rb");
	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

	int W = hinfo.biWidth;
	int H = hinfo.biHeight;
	imgSize = W * H;

	BYTE* image = (BYTE*)malloc(imgSize);
	BYTE* output = (BYTE*)malloc(imgSize);

	fread(image, sizeof(BYTE), imgSize, fp);
	fclose(fp);


	dilation(image, output, W, H);
	dilation(output, image, W, H);
	dilation(image, output, W, H);

	erosion(output, image, W, H);
	erosion(image, output, W, H);
	erosion(output, image, W, H);

	zhangSuen(image, output, H, W);

	inverseImage(output, output, imgSize);
	chkBifurcationAndEndPoint(output, image, W, H);

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(image, sizeof(BYTE), imgSize, fp);
	fclose(fp);

	free(image);
	free(output);

	return 0;
}
int getBlackNeighbours(int row, int col) {

	int i, j, sum = 0;

	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (i != 0 || j != 0)
				sum += (imageMatrix[row + i][col + j] == imagePixel);
		}
	}

	return sum;
}

int getBWTransitions(int row, int col) {
	return 	((imageMatrix[row - 1][col] == blankPixel && imageMatrix[row - 1][col + 1] == imagePixel)
		+ (imageMatrix[row - 1][col + 1] == blankPixel && imageMatrix[row][col + 1] == imagePixel)
		+ (imageMatrix[row][col + 1] == blankPixel && imageMatrix[row + 1][col + 1] == imagePixel)
		+ (imageMatrix[row + 1][col + 1] == blankPixel && imageMatrix[row + 1][col] == imagePixel)
		+ (imageMatrix[row + 1][col] == blankPixel && imageMatrix[row + 1][col - 1] == imagePixel)
		+ (imageMatrix[row + 1][col - 1] == blankPixel && imageMatrix[row][col - 1] == imagePixel)
		+ (imageMatrix[row][col - 1] == blankPixel && imageMatrix[row - 1][col - 1] == imagePixel)
		+ (imageMatrix[row - 1][col - 1] == blankPixel && imageMatrix[row - 1][col] == imagePixel));
}

int zhangSuenTest1(int row, int col) {
	int neighbours = getBlackNeighbours(row, col);

	return ((neighbours >= 2 && neighbours <= 6)
		&& (getBWTransitions(row, col) == 1)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel || imageMatrix[row + 1][col] == blankPixel)
		&& (imageMatrix[row][col + 1] == blankPixel || imageMatrix[row + 1][col] == blankPixel || imageMatrix[row][col - 1] == blankPixel));
}

int zhangSuenTest2(int row, int col) {
	int neighbours = getBlackNeighbours(row, col);

	return ((neighbours >= 2 && neighbours <= 6)
		&& (getBWTransitions(row, col) == 1)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel || imageMatrix[row][col - 1] == blankPixel)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row + 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel));
}

void zhangSuen(unsigned char* image, unsigned char* output, int rows, int cols) {

	int startRow = 1, startCol = 1, endRow, endCol, i, j, count, processed;

	pixel* markers;

	imageMatrix = (unsigned char**)malloc(rows * sizeof(unsigned char*));

	for (i = 0; i < rows; i++) {
		imageMatrix[i] = (unsigned char*)malloc((cols + 1) * sizeof(unsigned char));
		for (int k = 0; k < cols; k++) imageMatrix[i][k] = image[i * cols + k];
	}

	endRow = rows - 2;
	endCol = cols - 2;
	do {
		markers = (pixel*)malloc((endRow - startRow + 1) * (endCol - startCol + 1) * sizeof(pixel));
		count = 0;

		for (i = startRow; i <= endRow; i++) {
			for (j = startCol; j <= endCol; j++) {
				if (imageMatrix[i][j] == imagePixel && zhangSuenTest1(i, j) == 1) {
					markers[count].row = i;
					markers[count].col = j;
					count++;
				}
			}
		}

		processed = (count > 0);

		for (i = 0; i < count; i++) {
			imageMatrix[markers[i].row][markers[i].col] = blankPixel;
		}

		free(markers);
		markers = (pixel*)malloc((endRow - startRow + 1) * (endCol - startCol + 1) * sizeof(pixel));
		count = 0;

		for (i = startRow; i <= endRow; i++) {
			for (j = startCol; j <= endCol; j++) {
				if (imageMatrix[i][j] == imagePixel && zhangSuenTest2(i, j) == 1) {
					markers[count].row = i;
					markers[count].col = j;
					count++;
				}
			}
		}

		if (processed == 0)
			processed = (count > 0);

		for (i = 0; i < count; i++) {
			imageMatrix[markers[i].row][markers[i].col] = blankPixel;
		}

		free(markers);
	} while (processed == 1);


	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			output[i * cols + j] = imageMatrix[i][j];
		}
	}
}

void dilation(BYTE* image, BYTE* output, int W, int H) {
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (image[i * W + j] == 0) {
				if (!(image[(i + 1) * W + j] == 0 && image[(i - 1) * W + j] == 0
					&& image[i * W + j + 1] == 0 && image[i * W + j - 1] == 0))
					output[i * W + j] = 255;
				else
					output[i * W + j] = 0;
			}
			else
				output[i * W + j] = 255;
		}
	}
}
void erosion(BYTE* image, BYTE* output, int W, int H) {
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (image[i * W + j] == 255) {
				if (!(image[(i + 1) * W + j] == 255 && image[(i - 1) * W + j] == 255
					&& image[i * W + j + 1] == 255 && image[i * W + j - 1] == 255))
					output[i * W + j] = 0;
				else
					output[i * W + j] = 255;
			}
			else
				output[i * W + j] = 0;
		}
	}
}
int cntPixel(BYTE* image, int Cx, int Cy, int W, int H) {
	int cnt = 0;

	for (int i = Cy - 1; i <= Cy + 1; i++) {
		for (int j = Cx - 1; j <= Cx + 1; j++) {
			if (image[i * W + j] == 0)
				cnt++;
		}
	}

	return --cnt;
}
void drawBox(BYTE* image, int Cx, int Cy, int W, int H) {
	for (int i = Cx - 1; i < Cx + 1; i++)
		image[(Cy + 1) * W + i] = 178;
	for (int i = Cx - 1; i < Cx + 1; i++)
		image[(Cy - 1) * W + i] = 178;
	for (int i = Cy - 1; i < Cy + 1; i++)
		image[i * W + Cx + 1] = 178;
	for (int i = Cy - 1; i < Cy + 1; i++)
		image[i * W + Cx - 1] = 178;
}
void chkBifurcationAndEndPoint(BYTE* image, BYTE* output, int W, int H) {
	for (int i = 0; i < W * H; i++)
		output[i] = image[i];

	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (image[i * W + j] == 0) {
				if (cntPixel(image, j, i, W, H) == 1)
					drawBox(output, j, i, W, H);
				else if (cntPixel(output, j, i, W, H) == 3)
				{
					int cnt = 0;
					int Cy = i - 1;
					int Cx = j - 1;
					int tmp = image[Cy * W + Cx];

					for (int k = Cx; k <= Cx + 2 + 1; k++) {
						if (image[Cy * W + k] == 255 && tmp == 0)
							cnt++;
						tmp = image[Cy * W + k];
					}
					Cx += 2;

					for (int k = Cy; k <= Cy + 2; k++) {
						if (image[k * W + Cx] == 255 && tmp == 0)
							cnt++;
						tmp = image[k * W + Cx];
					}
					Cy += 2;

					for (int k = Cx; k >= Cx - 2; k--) {
						if (image[Cy * W + k] == 255 && tmp == 0)
							cnt++;
						tmp = image[Cy * W + k];
					}

					Cx -= 2;

					for (int k = Cy; k >= Cy - 2; k--) {
						if (image[k * W + Cx] == 255 && tmp == 0)
							cnt++;
						tmp = image[k * W + Cx];
					}

					if (cnt == 3)
						drawBox(output, j, i, W, H);
				}

			}
		}
	}
}

void inverseImage(BYTE* image, BYTE* output, int imgSize) {
	for (int i = 0; i < imgSize; i++) {
		output[i] = 255 - image[i];
	}
}
