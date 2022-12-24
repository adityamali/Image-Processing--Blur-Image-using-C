#include <stdio.h>
int main()
{

	FILE *fIn = fopen("images/1.bmp", "r");
	FILE *fOut = fopen("2.bmp", "w+");

	int i, j, y, x;
	unsigned char byte[54];

	if (fIn == NULL)
	{
		printf("File does not exist.\n");
	}

	for (i = 0; i < 54; i++)
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte, sizeof(unsigned char), 54, fOut);

	int height = *(int *)&byte[18];
	int width = *(int *)&byte[22];
	int bitDepth = *(int *)&byte[28];

	printf("width: %d\n", width);
	printf("height: %d\n", height);

	int size = height * width;

	unsigned char buffer[size][3];
	unsigned char out[size][3];

	for (i = 0; i < size; i++)
	{
		buffer[i][2] = getc(fIn);
		buffer[i][1] = getc(fIn);
		buffer[i][0] = getc(fIn);
	}

	float v = 1.0 / 9.0;
	float kernel[3][3] = {{v, v, v},
						  {v, v, v},
						  {v, v, v}};

	for (x = 1; x < height - 1; x++)
	{
		for (y = 1; y < width - 1; y++)
		{
			float sum0 = 0.0;
			float sum1 = 0.0;
			float sum2 = 0.0;
			for (i = -1; i <= 1; ++i)
			{
				for (j = -1; j <= 1; ++j)
				{

					sum0 = sum0 + (float)kernel[i + 1][j + 1] * buffer[(x + i) * width + (y + j)][0];
					sum1 = sum1 + (float)kernel[i + 1][j + 1] * buffer[(x + i) * width + (y + j)][1];
					sum2 = sum2 + (float)kernel[i + 1][j + 1] * buffer[(x + i) * width + (y + j)][2];
				}
			}
			out[(x)*width + (y)][0] = sum0;
			out[(x)*width + (y)][1] = sum1;
			out[(x)*width + (y)][2] = sum2;
		}
	}

	for (i = 0; i < size; i++)
	{
		putc(out[i][2], fOut);
		putc(out[i][1], fOut);
		putc(out[i][0], fOut);
	}

	fclose(fOut);
	fclose(fIn);

	return 0;
}
