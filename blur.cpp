///////
// ================================================================
// blur.cpp - Directional blur program
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include "IP.h"
using namespace std;

// function prototype
void blur(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to directionally blur an image.
//
int
main(int argc, char** argv)
{
	int	xsz, ysz; 
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: blur in xsz ysz out\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	xsz  = atoi(argv[2]);
	ysz  = atoi(argv[3]);

	if(xsz < 1 || ysz < 1 || xsz > 127 || ysz > 127) {
		cerr << "xsz and ysz must be a positive integer less than 128 [1, 127]\n";
		exit(1);
	}

	if(xsz%2 == 0 || ysz%2 == 0) {
		cerr << "xsz and ysz must be an odd integer\n";
		exit(1);
	}

	// blur image and save result in file
	blur(I1, xsz, ysz, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// blur:
//
// Directionally blur an image.
// 

void
blur(imageP I1, int xsz, int ysz, imageP I2)
{
	int i, w_itr, h_itr, pad, total, pad_itr, mysum, out_itr, w, h;
	int xline[I1->width+xsz], yline[I1->height+ysz], rowblur[I1->width*I1->height], colblur[I1->width*I1->height];
	uchar *in, *out;

	w = I1->width;		h = I1->height;		total = w * h;		in  = I1->image;

	// init I2 dimensions and buffer
	I2->width  = I1->width;			I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "blur: Insufficient memory\n";
		exit(1);
	}
	out = I2->image;
	// iterate from top to hottom
	for(h_itr=0, out_itr=0, pad=(int)(xsz/2); h_itr<h; h_itr++) {
		// iterate from left to right
		for(i=h_itr*w, w_itr=0, mysum=0; i<(h_itr*w+w); i++) {
			// if the row iter is on the first or last pixel of line,
			// add xsz/2 pad pixels and the pixel itself to row buffer
			if(i%w==0 || i%w==(w-1))
				for(pad_itr=0; pad_itr<pad+1; pad_itr++) xline[w_itr++] = in[i];
			// if the row itr is not on the first or last pixel of row, copy the pixel into the row buffer
			else xline[w_itr++] = in[i];
		}
		// sum of neighbor pixels for first pixel in image row
		for(i=0; i<xsz; i++) mysum += xline[i];
		// place average of row neighbors into temporary image
		for(w_itr=pad; w_itr<pad+w; w_itr++){
			rowblur[out_itr++] = mysum/xsz;
			mysum += (xline[w_itr+pad+1] - xline[w_itr-pad]);
		}
	}
	// iterate over the new image, with rows blurred, from top to bottom
	for(h_itr=0, out_itr=0, pad=(int)(ysz/2); h_itr<w; h_itr++) {
		// iterate over the new image, with rows blurred from left to right
		for(i=h_itr, w_itr=0, mysum=0; i<total; i+=w) {
			// if the row iter is on the first or last pixel of line,
			// add ysz/2 pad pixels and the pixel itself to row buffer
			if(i<w || i>=(total-w))
				for(pad_itr=0; pad_itr<pad+1; pad_itr++) yline[w_itr++] = rowblur[i];
			// if the row itr is not on the first or last pixel of row, copy the pixel into the row buffer
			else yline[w_itr++] = rowblur[i];
		}
		// sum of neighbor pixels for the first pixel in new image row
		for(i=0; i<ysz; i++) mysum += yline[i];
		// place average of row neighbors into another temp image.
		// the rows and columns trade places, making the image rotate 90 degrees
		for(w_itr=pad; w_itr<pad+h; w_itr++){
			colblur[out_itr++] = mysum/ysz;
			mysum += (yline[w_itr+pad+1] - yline[w_itr-pad]);
		}
	}
	// send pixels to output image after switching the rows and columns again to rotate back 90 degrees
	for(h_itr=0, out_itr=0; h_itr<w; h_itr++)
		for(i=h_itr; i<total; i+=w) out[out_itr++] = colblur[i];
}