#include <stdlib.h>

#include <stdio.h>

#include <unistd.h>

#include "bmplib.h"

void usage()
{

	fprintf(stderr, "usage: bmptool [-f | -r | -l | -s scale | -v] [ -o output_file] [input_file]\n");

	exit(1);

}

/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */

int enlarge(PIXEL* original, int rows, int cols, int scale,

	PIXEL** new, int* newrows, int* newcols)

{
	int row, col, y, x;
	if ((rows <= 0) || (cols <= 0)) return -1;

	*new = (PIXEL*)malloc((rows) * (cols) * sizeof(PIXEL));

	for (row = 0; row < rows; row++)
	{
		for (y = 0; y < scale; y++)
		{
			for (col = 0; col < cols; cols++)
			{
				for (x = 0; x < scale; x++)
				{
					PIXEL* o = original + row * cols + col;
					PIXEL* n = ((*new) + (row * cols * scale * scale) + (cols * scale * y) + (col * scale) + x);
					*n = *o;
				}
			}
		}
	}
	*newrows = scale * rows;
	*newcols = scale * cols;

	return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file 90 degrees clockwise that
 * has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */

int rotate(PIXEL* original, int rows, int cols, int rotation,
	PIXEL** new, int* newrows, int* newcols)
{
	int row, col;
	if (rotation == 90)
	{
		*new = (PIXEL*)malloc((rows) * (cols) * sizeof(PIXEL));

		for (row = 0; row < rows; row++)
		{
			for (col = 0; col < cols; col++)
			{

				PIXEL* o = original + (row * cols) + col;
				PIXEL* n = (*new) + ((cols - col - 1) * rows) + row;
				*n = *o;
			}
		}
		*newcols = rows;
		*newrows = cols;
		printf("rotate R");
	}

	if (rotation == -90)
	{
		*new = (PIXEL*)malloc((rows) * (cols) * sizeof(PIXEL));

		for (row = 0; row < rows; row++)
		{
			for (col = 0; col < cols; col++)
			{
				PIXEL* o = original + (row * cols) + col;
				PIXEL* n = (*new) + (col * rows) + (rows - row - 1);
				*n = *o;
			}
		}

		*newcols = rows;
		*newrows = cols;
		printf("rotate L");
	}

	return 0;
}

/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int verticalflip(PIXEL* original, PIXEL** new, int rows, int cols)
{
	int row, col;

	if ((rows <= 0) || (cols <= 0)) return -1;

	*new = (PIXEL*)malloc(rows * cols * sizeof(PIXEL));

	for (row = 0; row < rows; row++)
		for (col = 0; col < cols; col++) {
			PIXEL* o = original + row * cols + col;
			PIXEL* n = (*new) + (rows - row - 1) * cols + (cols - 1 - col);
			*n = *o;
		}

	return 0;
}

int flip(PIXEL* original, PIXEL** new, int rows, int cols)
{
	int row, col;

	if ((rows <= 0) || (cols <= 0)) return -1;

	*new = (PIXEL*)malloc(rows * cols * sizeof(PIXEL));

	for (row = 0; row < rows; row++)
		for (col = 0; col < cols; col++) {
			PIXEL* o = original + row * cols + col;
			PIXEL* n = (*new) + row * cols + (cols - 1 - col);
			*n = *o;
		}

	return 0;
}

int main(int argc, char* argv[])
{

	int r, c, nr, nc;
	PIXEL* b, * nb;
	char* infile;
	char* outfile;

	int fflag, rflag, lflag, sflag, vflag, oflag, nro;

	int scale;

	outfile = NULL;
	infile = NULL;


	scale = 0;
	fflag = rflag = lflag = sflag = vflag = oflag = nro = 0;


	while ((c = getopt(argc, argv, "frls:vo:")) != -1) {

		switch (c) {

		case 'f':

			fflag = 1;
			break;

		case 'r':

			printf("r\n");
			rflag = 1;
			break;

		case 'l':

			lflag = 1;
			break;

		case 's':

			sflag = 1;
			scale = atoi(optarg);
			break;

		case 'v':

			vflag = 1;
			break;

		case 'o':

			oflag = 1;
			outfile = optarg;
			break;

		case '?':
			break;

		default:
			usage();
			printf("default");

		}

	}
	
	if (optind < argc)
	{
		infile = argv[optind];
	}


	readFile(infile, &r, &c, &b);

	if (sflag) {
		enlarge(b, r, c, scale, &nb, &nr, &nc);
		r = nr;
		c = nc;
	}

	if (rflag == 1) {
		rotate(b, r, c, 90, &nb, &nr, &nc);
		r = nr;
		c = nc;
	}

	if (lflag == 1) {
		rotate(b, r, c, -90, &nb, &nr, &nc);
		r = nr;
		c = nc;
	}

	if (fflag) {
		flip(b, &nb, r, c);
	}

	if (vflag) {
		verticalflip(b, &nb, r, c);
	}


	writeFile(outfile, r, c, nb);
	free(b);
	free(nb);
	return 0;

}
