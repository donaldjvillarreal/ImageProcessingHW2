///////
// ================================================================
// error_diffusion.cpp - Directional blur program
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include "IP.h"
using namespace std;

// Global Variables
int argc;
char **argv;

// function prototype
void error_diffusion(imageP, int, int, double, imageP);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to directionally blur an image.
//
int
main(int argc, char** argv)
{
	
	int	mtd, serpentine,out;
	double gamma;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 6) {
		cerr << "Usage: error_diffusion in mtd serpentine gamma out\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	mtd  = atoi(argv[2]);
	serpentine  = atoi(argv[3]);
	gamma = atof(argv[4]);


	// error_diffuse and save result in file
	error_diffusion(I1, mtd, serpentine, gamma, I2);
	cerr << "I2->height=" << I2->height << "width=" << I2->width;
	IP_saveImage(I2, argv[5]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// error_diffusion:
//
// Error diffused dithering.
// 

void
error_diffusion(imageP I1, int mtd, int serpentine, double gamma, imageP I2)
{
	int i, total, scale;
	uchar *in, *out, lut[MXGRAY];

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	int total2 = I2->width * I2->height;
	I2->image  = (uchar *) malloc(total2);
	in  = I1->image;	// input  image buffer
	out = I2->image;
	if(I2->image == NULL) {
		cerr << "error_diffusion: Insufficient memory\n";
		exit(1);
	}

	// Create GC lut[]		
	double gc = 1/gamma;
	for(i=0; i<MXGRAY; i++){
		lut[i] = 255 * pow((double)i/255, gc);
		//cerr <<  gamma << endl;
	}
	
	// Apply GC lut[]
	for(i=0; i<total; i++)
		in[i] = lut[ in[i] ];	

	// WEIGHT and SCAN decision
	if(mtd == 0){// set FS 
		if(serpentine == 0){//  FS-RASTER
			// Begin FS-RASTER scan
			int x, y, i;
			int w = I1->width;
			int h = I1->height;
			int buf[2][w+2];
			int e, *in1, *in2;
			int thr = MXGRAY / 2; // init threshold value
			
			for(i=0; i<w; ++i){ // copy row 0 to circular buffer
				buf[0][i+1]=in[i];
				
			}
			for(y=0; y<h; y++){ // visit all input rows
				for(i=0;i<w;++i){ // copy next row to circ buffer
					buf[(y+1)%2][i+1]=in[(y+1)*w+i];
				}
				in1 = buf[y%2]+1; // circ buffer pointer skip over pad
				in2 = buf[(y+1)%2]+1; // circ buffer ptr; skip over pad
				for(x=0; x<w; x++){ // visit all input cols
					*out = (*in1 < thr)? 255 : 0; // threshold
					e = *in1 - *out; //eval error
					in1[1] += (e*7/16.); // add error to E nbr
					in2[-1] += (e*3/16.); // add error to SW nbr
					in2[0] += (e*5/16.); // add error to S nbr
					in2[1] += (e*1/16.); // add error to SE nbr
					in1++; in2++; //advance circular buffer
					out++; //advance output ptr
				}
			}
		}else{ // FS-SERPENTINE	
						int x, y, i;
			int w = I1->width;
			int h = I1->height;
			int buf[2][w+2];
			int e, *in1, *in2;
			int thr = MXGRAY / 2; // init threshold value
			
			for(i=0; i<w; ++i){ // copy row 0 to circular buffer
				buf[0][i+1]=in[i];
				
			}
			for(y=0; y<h; y++){ // visit all input rows
				for(i=0;i<w;++i){ // copy next row to circ buffer
					buf[(y+1)%2][i+1]=in[(y+1)*w+i];
				}
				in1 = buf[y%2]+1; // circ buffer pointer skip over pad
				in2 = buf[(y+1)%2]+1; // circ buffer ptr; skip over pad
				for(x=0; x<w; x++){ // visit all input cols
					*out = (*in1 < thr)? 255 : 0; // threshold
					e = *in1 - *out; //eval error
					in1[1] += (e*7/16.); // add error to E nbr
					in2[-1] += (e*3/16.); // add error to SW nbr
					in2[0] += (e*5/16.); // add error to S nbr
					in2[1] += (e*1/16.); // add error to SE nbr
					in1++; in2++; //advance circular buffer
					out++; //advance output ptr
				}
			}
		}
	}else{ // set JJN
		if(serpentine == 0){ // JJN-RASTER
			int x, y, i;
			int w = I1->width;
			int h = I1->height;
			uchar buf[3][w+4];
			uchar e, *in1, *in2, *in3;
			int thr = MXGRAY / 2; // init threshold value			
			for(i=0; i<w; ++i){ // copy row 0 to circular buffer
				buf[0][i+2]=in[i];
			}
			for(i=0; i<w; ++i){ // copy row 0 to circular buffer
				buf[1][i+2]=in[w+i+1];
			}
			for(y=0; y<h; y++){ // visit all input rows
				for(i=0;i<w;++i){ // copy next row to circ buffer
					buf[(y+2)%3][i+2]=in[(y+2)*w+i+2];
				}
				in1 = buf[y%3]+2; // circ buffer pointer skip over pad
				in2 = buf[(y+1)%3]+2; // circ buffer ptr; skip over pad
				in3 = buf[(y+2)%3]+2;
				for(x=0; x<w; x++){ // visit all input cols
					*out = (*in1 < thr)? 0 : 255; // threshold
					e = *in1 - *out; //eval error
					in1[1] += (e*7/48.); 
					in1[2] += (e*5/48.);
					in2[-2] += (e*3/48.);
					in2[-1] += (e*5/48.);
					in2[0] += (e*7/48.);
					in2[1] += (e*5/48.);
					in2[2] += (e*3/48.);
					in3[-2] += (e*1/48.);
					in3[-1] += (e*3/48.);
					in3[0] += (e*5/48.);
					in3[1] += (e*3/48.);
					in3[2] += (e*1/48.);
					in1++; in2++; in3++;//advance circular buffer
					out++; //advance output ptr
				}
			}
		}else{// JJN-SERPENTINE
			// Begin FS-RASTER scan
					int x, y, i;
			int w = I1->width;
			int h = I1->height;
			uchar buf[3][w+4];
			uchar e, *in1, *in2, *in3;
			int thr = MXGRAY / 2; // init threshold value
			
			for(i=0; i<w; ++i){ // copy row 0 to circular buffer
				buf[0][i]=in[i];
			}
			for(i=0; i<w; ++i){ // copy row 0 to circular buffer
				buf[1][i]=in[w+i];
			}
			for(y=0; y<h; y++){ // visit all input rows
				for(i=0;i<w;++i){ // copy next row to circ buffer
					buf[(y+2)%3][w-i]=in[(y+1)*w+i];
				}
				in1 = buf[y%3]+1; // circ buffer pointer skip over pad
				in2 = buf[(y+1)%3]+1; // circ buffer ptr; skip over pad
				in3 = buf[(y+2)%3]+1;
				for(x=0; x<w; x++){ // visit all input cols
					*out = (*in1 < thr)? 0 : 255; // threshold
					e = *in1 - *out; //eval error
					in1[1] += (e*7/48.); 
					in1[2] += (e*5/48.);
					in2[-2] += (e*3/48.);
					in2[-1] += (e*5/48.);
					in2[0] += (e*7/48.);
					in2[1] += (e*5/48.);
					in2[2] += (e*3/48.);
					in3[-2] += (e*1/48.);
					in3[-1] += (e*3/48.);
					in3[0] += (e*5/48.);
					in3[1] += (e*3/48.);
					in3[2] += (e*1/48.);
					in1++; in2++; in3++;//advance circular buffer
					out++; //advance output ptr
				}
			}			
		}
	}
}


