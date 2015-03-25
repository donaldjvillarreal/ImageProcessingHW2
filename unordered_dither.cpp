///////
// ================================================================
// unordered_dither.cpp - Unordered dithering program.
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void unordered_dither(imageP, int, double, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to quantize image.
//
int
main(int argc, char** argv)
{
	int	levels; 
	double gamma;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: unordered_dither infile n gamma outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	levels  = atoi(argv[2]);
	gamma  = atof(argv[3]);

	// quantize image and save result in file
	unordered_dither(I1, levels, gamma, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// unordered_dither:
//
// Unordered dither with gamma correction.
// 

void
unordered_dither(imageP I1, int levels, double gamma, imageP I2)
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
		cerr << "unordered_dither: Insufficient memory\n";
		exit(1);
	}

	// Gamma correct first
	
	scale = MXGRAY / levels;
	in  = I1->image;	// input  image buffer
	int temp[total];

	double gc = 1/gamma;
	for(i=0; i<total; i++){
		temp[i] = 255 * pow((double)in[i]/255, gc);
	}

	// init lookup table
	
	int rnd; 
	for(i=0; i<total; i++){
		rnd = rand() % (int)(scale*0.5);		
		if(i%2 == 0){	
			temp[i] = temp[i] + rnd;
		}else{
			temp[i] = temp[i] - rnd;
		}
		if(temp[i]>256){
			temp[i] = 256;
		}
		else if (temp[i] < 0){
			temp[i] = 0;
		}
	}

	for(i=0; i<MXGRAY; i++){
		lut[i] = scale * (i/scale) + (scale/2);
		if((i>1) && (lut[i] < (scale/2))){
			lut[i] = lut[i-1];
		}	
	}
	// iterate over all pixels
	
	out = I2->image;	// output image buffer
	for(i=0; i<total; i++){
		out[i] = lut[ temp[i] ];
	}
}
