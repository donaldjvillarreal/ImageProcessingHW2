///////
// ================================================================
// ordered_dither.cpp - Unordered dithering program.
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void ordered_dither(imageP, int, double, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to order dither image.
//
int
main(int argc, char** argv)
{
	int	m; 
	double gamma;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: halftone infile m gamma outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	m  = atoi(argv[2]);
	gamma  = atof(argv[3]);

	// quantize image and save result in file
	ordered_dither(I1, m, gamma, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// halftone:
//
// 
// 

void
halftone(imageP I1, int m, double gamma, imageP I2)
{
	int i, j, x, y, w, h, total, scale;
	uchar *in, *out, lut[MXGRAY];
	
	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	in  = I1->image;	// input  image buffer
	h = I2->height;
	w = I2->width;
	if(I2->image == NULL) {
		cerr << "halftone: Insufficient memory\n";
		exit(1);
	}

	// Create GC lut[]		
	double gc = 1/gamma;
	for(i=0; i<MXGRAY; i++){
		lut[i] = 255 * pow((double)i/255, gc);
	}
	
	// Apply GC lut[]
	for(i=0; i<total; i++)
		in[i] = lut[ in[i] ];	

	imageP I3;
	I3 = NEWIMAGE;
	I3->width  = I1->width * m;
	I3->height = I1->height * m;
	totalInt = I3->width * I3->height;
	I3->image  = (uchar *) malloc(totalInt);
	
	int intH = I3->height;
	int intW = I3->width; 
	uchar *intOut = I3->image;
	y = z = 0;
	int row;

	int rowOffset = 0;
	int offset = 0;
	int srcRow = 0;
	int srcPix = 0;
	int trgRow = 0;
	int offset = 0;
	int i, j, y, x;

	for(i = 0; i < h; i++){
		srcRow = i*w;
		for(j = 0; j < w; j++){
			srcPix = srcRow + j;
			for(y = 0; y < m; ++y){
				trgRow = y*intW+rowOffset;
				for(x = 0; x < m; ++x){
					intOut[offset+x+trgRow] = in[srcPix];
				}
			}
			offset += m; 
		}
		rowOffset += m*rowOffset; 
		offset = 0;
	}
			
	int D[8][8] = {
					{42,43,44,45,46,47,48,49}, 
					{41,20,21,22,23,24,25,50},
					{40,19,6,7,8,9,26,51}, 
					{39,18,5,0,1,10,27,52},
					{38,17,4,3,2,11,28,53}, 
					{37,16,15,14,13,12,29,54},
					{36,35,34,33,32,31,30,55}, 
					{63,62,61,60,59,58,57,56}
					};

	out = I2->image;	// output image buffer;

	for(y = 0; y < intH; y++){
		for(x = 0; x < intW; x++){
			i = x%m;
			j = y%m;

		}
	}


	for(y=0;y<h;++y){
		for(x=0;x<w;++x){
			i = x%m;
			j = y%m;
			if(D[i][j] == 0){
				out[y*w+x] = 0;
			}
			else{
				out[y*w+x] = (in[y*w+x] > 256/D[i][j]) ? 255 : 0;
			}
}	