
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
	int i, x, y, xpad, ypad, total, pad_itr, mysum, out_itr, w, h;
	int xline[I1->width+xsz], yline[I1->height+ysz], rowblur[I1->width*I1->height], colblur[I1->width*I1->height];
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

	xpad = (int)(xsz/2);		ypad = (int)(ysz/2);

	out_itr=0;
	for(y=0; y<h; y++) {
		x=0;
		mysum=0;
		for(i=y*w; i<(y*w+w); i++) {
			if(i%w==0 || i%w==(w-1)) {
				for(pad_itr=0; pad_itr<xpad+1; pad_itr++) xline[x++] = in[i];
			}
			else xline[x++] = in[i];
		}
		
		for(i=0; i<xsz; i++) mysum += xline[i];

		for(x=xpad; x<xpad+w; x++){
			rowblur[out_itr++] = (int)mysum/xsz;
			mysum += (xline[x+xpad+1] - xline[x-xpad]);
		}
	}

	out_itr = 0;
	for(y=0; y<w; y++) {
		x=0;
		mysum=0;
		for(i=y; i<total; i+=w) {
			if(i<w || i>=(total-w)) {
				for(pad_itr=0; pad_itr<ypad+1; pad_itr++) yline[x++] = rowblur[i];
			}
			else yline[x++] = rowblur[i];
		}
		
		for(i=0; i<ysz; i++) mysum += yline[i];

		for(x=ypad; x<ypad+h; x++){
			colblur[out_itr++] = (int)mysum/ysz;
			mysum += (yline[x+ypad+1] - yline[x-ypad]);
		}
	}

	out = I2->image;	// output image buffer
	out_itr = 0;
	for(y=0; y<w; y++) {
		for(i=y; i<total; i+=w) {
			out[out_itr++] = colblur[i];
		}
	}

}