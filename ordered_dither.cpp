
//IN PROGRESS!

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
		cerr << "Usage: ordered_dither infile m gamma outfile\n";
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
// ordered_dither:
//
// 
// 

void
ordered_dither(imageP I1, int m, double gamma, imageP I2)
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
		cerr << "ordered_dither: Insufficient memory\n";
		exit(1);
	}

	// Gamma correct first
	
	//scale = MXGRAY / levels;
	in  = I1->image;	// input  image buffer
	int temp[total];

	double gc = 1/gamma;
	for(i=0; i<total; i++){
		temp[i] = 255 * pow((double)in[i]/255, gc);
	}

	//create dither matrix
	int n = 0;
	int *D;
	switch(m){
	case 2:
	{
		n = 2; 	
		int D2[2][2] = {
					{0, 2},
					{3, 1}};
		D = &D2[0][0];
	}
	break;
	case 3:
	{
		n = 3;
		int D3[3][3] = {
					{6, 8, 4}, 
					{1, 0, 3}, 
					{5, 2, 7}};
		D = &D3[0][0];
	}
	break;
	case 4:
	{
		n = 4;				
		int D4[4][4] = {
					{0, 8, 2, 10 }, 
					{12, 4, 14, 6}, 
					{3, 11, 1, 9 },
					{15, 7, 13, 5}};
		D = &D4[0][0];
	}	
	break;
	case 8:
	{
		n = 8;				
		int D8[8][8] = {
					{0,32,8,40,2,34,10,42	}, 
					{48,16,56,24,50,18,58,26},
					{12,44,4,36,14,46,6,38	}, 
					{60,28,52,20,62,30,54,22},
					{3,35,11,43,1,33,9,41	}, 
					{51,19,59,27,49,17,57,25},
					{15,47,7,39,13,45,5,37	}, 
					{63,31,55,23,61,29,53,21}};
		D = &D8[0][0];
	}
	break;
	default:
		cerr << "Invalid matrix dimension. Valid m values are 2, 3, 4, 8." << endl;

	}
	cerr << n << endl;
	cerr << D[1] << endl;

	// init lookup table
	
	/*
	int rnd; 
	for(i=0; i<total; i++){
		rnd = rand() % (int)(scale*0.5);
		cerr << rnd << endl;
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
	}*/
}
