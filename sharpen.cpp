
//IN PROGRESS!

///////
// ================================================================
// blur.cpp - Directional blur program
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include <vector>
#include "IP.h"

using namespace std;

// function prototype
void sharpen(imageP, int, double, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to directionally blur an image.
//
int
main(int argc, char** argv)
{
	int	sz, fctr; 
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: sharpen in sz fctr out\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	sz  = atoi(argv[2]);

	if(sz < 1 || sz > 127) {
		cerr << "sz must be a positive integer less than 128 [1, 127]\n";
		exit(1);
	}

	if(sz%2 == 0) {
		cerr << "sz must be an odd integer\n";
		exit(1);
	}

	fctr  = atof(argv[3]);
	if(fctr < 1.0 || fctr > 10.0) {
		cerr << "fctr must be between 1 and 10 [1.0, 10.0]\n";
		exit(1);
	}

	// blur image and save result in file
	sharpen(I1, sz, fctr, I2);
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
sharpen(imageP I1, int sz, double fctr, imageP I2)
{
	int i, x, y, padsz, total, pad_itr, mysum, out_itr, w, h, calc;
	int xline[I1->width+sz], yline[I1->height+sz], rowblur[I1->width*I1->height], colblur[I1->width*I1->height],
		blur[I1->width*I1->height];
	uchar *in, *out;

	w = I1->width;		h = I1->height;		total = w * h;		in  = I1->image;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "blur: Insufficient memory\n";
		exit(1);
	}

	padsz = (int)(sz/2);

	out_itr=0;
	for(y=0; y<h; y++) {
		x=0;
		mysum=0;
		for(i=y*w; i<(y*w+w); i++) {
			if(i%w==0 || i%w==(w-1)) {
				for(pad_itr=0; pad_itr<padsz+1; pad_itr++) xline[x++] = in[i];
			}
			else xline[x++] = in[i];
		}
		
		for(i=0; i<sz; i++) mysum += xline[i];

		for(x=padsz; x<padsz+w; x++){
			rowblur[out_itr++] = (int)mysum/sz;
			mysum += (xline[x+padsz+1] - xline[x-padsz]);
		}
	}

	out_itr = 0;
	for(y=0; y<w; y++) {
		x=0;
		mysum=0;
		for(i=y; i<total; i+=w) {
			if(i<w || i>=(total-w)) {
				for(pad_itr=0; pad_itr<padsz+1; pad_itr++) yline[x++] = rowblur[i];
			}
			else yline[x++] = rowblur[i];
		}
		
		for(i=0; i<sz; i++) mysum += yline[i];

		for(x=padsz; x<padsz+h; x++){
			colblur[out_itr++] = (int)mysum/sz;
			mysum += (yline[x+padsz+1] - yline[x-padsz]);
		}
	}

	out_itr = 0;
	for(y=0; y<w; y++) {
		for(i=y; i<total; i+=w) {
			blur[out_itr++] = colblur[i];
		}
	}

	out = I2->image;	// output image buffer
	for(i=0; i<total; i++) {
		calc = in[i] + (fctr*(in[i] - blur[i]));
		if(calc < BLACK) out[i] = BLACK;
		else if (calc > WHITE) out[i] = WHITE;
		else out[i] = calc;
	}
}