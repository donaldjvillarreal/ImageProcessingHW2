
//IN PROGRESS!

///////
// ================================================================
// qntz.cpp - Quantization program.
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void unordered_dither(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to quantize image.
//
int
main(int argc, char** argv)
{
	int	levels, gamma;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: unordered_dither infile gamma n outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	levels  = atoi(argv[3]);
	gamma  = atoi(argv[2]);

	// quantize image and save result in file
	unordered_dither(I1, levels, gamma, I2);
	IP_saveImage(I2, argv[3]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// qntz:
//
// Quantize I1 into n uniform intervals and save the output in I2.
// 

void
unordered_dither(imageP I1, int levels, int gamma, imageP I2)
{
	int i, total, scale;
	uchar *in, *out, lut[MXGRAY];

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "qntz: Insufficient memory\n";
		exit(1);
	}

	// init lookup table
	scale = MXGRAY / levels;

	in  = I1->image;	// input  image buffer
	int rnd; 
	for(i=0; i<total; i++){
		rnd = rand() % scale;
		if(i%2 == 0){	
			in[i] = (int)in[i] + rnd;
		}else{
			//cerr << "flag 1";
			in[i] = (int)in[i] - rnd;
		}
	}

	for(i=0; i<MXGRAY; i++){
		lut[i] = scale * (int) (i/scale) + (int) (scale/2);
		cerr << (int)lut[i] << endl;
	}
	// iterate over all pixels
	
	out = I2->image;	// output image buffer
	for(i=0; i<total; i++){
		out[i] = lut[ in[i] ];
	}
}
