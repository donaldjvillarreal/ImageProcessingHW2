
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
		cerr << "ordered_dither: Insufficient memory\n";
		exit(1);
	}

	// Create GC lut[]		
	double gc = 1/gamma;
	for(i=0; i<MXGRAY; i++){
		lut[i] = 255 * pow((double)i/255, gc);
		cerr <<  (int)lut[i] << endl;
	}
	cerr << "FLAG 0" << endl;
	
	// Apply GC lut[]
	for(i=0; i<total; i++)
		in[i] = lut[ in[i] ];	

	/////////////////////////////////
	// Output Gamma Corrected image
	char label1[] = "GC_mad256.pgm";
	imageP I9;
	I9 = NEWIMAGE;
	I9->image  = (uchar *) malloc(total);
	I9->width  = I1->width;
	I9->height = I1->height;
	out = I9->image;	// output image buffer
	for(i=0; i<total; i++)
		out[i] = in[i];
	IP_saveImage(I9, label1);
	//////////////////////////////////
	
	cerr << "FLAG 1" << endl;
	// Create Quantize lut[]
	scale = MXGRAY / (m*m);
	for(i=0; i<MXGRAY; i++)
		lut[i] = scale * (int) (i/scale) + (int) (scale/2);

	// Apply lut[]
	for(i=0; i<total; i++)
		in[i] = lut[ in[i] ];	

	cerr << "FLAG 2" << endl;	

	/////////////////////////////////
	// Output Gamma corrected and Quantized image
	char label2[] = "GC_QNTZ_mad256.pgm";
	imageP I8;
	I8 = NEWIMAGE;
	I8->width  = I1->width;
	I8->height = I1->height;
	I8->image  = (uchar *) malloc(total);
	out = I8->image;	// output image buffer
	for(i=0; i<total; i++)
		out[i] = lut[ in[i] ];
	IP_saveImage(I8, label2);
	//////////////////////////////////

	out = I2->image;	// output image buffer;
	cerr << "FLAG 3" << endl;
	switch(m){
	case 2:
	{	
		int D[2][2] = {
					{0, 2},
					{3, 1}};
		for(i=0;i<m;++i){
			for(j=0;j<m;++j){
				D[i][j] = ((scale*(double)D[i][j])+((double)D[i][j]*scale));
			}
		}
		cerr << "D[0][0] = " << D[0][0] << "D[0][1] = " << D[0][1] 
			 << "D[1][0] = " << D[1][0] << "D[1][1] = " << D[1][1] << endl;
		
		for(y=0;y<h;++y){
			for(x=0;x<w;++x){
				i = x%m;
				j = y%m;
				if(D[i][j] == 0){
					out[y*w+x] = 0;
				}
				else{
					out[y*w+x] = (in[y*w+x] > 255/D[i][j]) ? 255 : 0;
				}
			}
		}
	}
	break;
	case 3:
	{
		int D[3][3] = {
					{6, 8, 4}, 
					{1, 0, 3}, 
					{5, 2, 7}};
		for(y=0;y<h;++y){
			for(x=0;x<w;++x){
				i = x%m;
				j = y%m;
				if(D[i][j] == 0){
					out[y*w+x] = 0;
				}
				else{
					out[y*w+x] = (in[y*w+x] > 255/D[i][j]) ? 255 : 0;
				}
			}
		}	
	}
	break;
	case 4:
	{				
		int	D[4][4] = {
					{0, 8, 2, 10 }, 
					{12, 4, 14, 6}, 
					{3, 11, 1, 9 },
					{15, 7, 13, 5}};
		for(y=0;y<h;++y){
			for(x=0;x<w;++x){
				i = x%m;
				j = y%m;
				if(D[i][j] == 0){
					out[y*w+x] = 0;
				}
				else{
					out[y*w+x] = (in[y*w+x] > 255/D[i][j]) ? 255 : 0;
				}
			}
		}	
	}	
	break;
	case 8:
	{				
		int D[8][8] = {
					{0,32,8,40,2,34,10,42	}, 
					{48,16,56,24,50,18,58,26},
					{12,44,4,36,14,46,6,38	}, 
					{60,28,52,20,62,30,54,22},
					{3,35,11,43,1,33,9,41	}, 
					{51,19,59,27,49,17,57,25},
					{15,47,7,39,13,45,5,37	}, 
					{63,31,55,23,61,29,53,21}};
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
		}
	}
	break;
	default:
		cerr << "Invalid matrix dimensions. Valid m values are 2, 3, 4, 8." << endl;
	}
}
