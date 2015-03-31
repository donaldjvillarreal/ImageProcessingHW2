///////
// ================================================================
// convolve.cpp - Convolve program
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include <algorithm>
#include "IP.h"
using namespace std;

// function prototype
void convolve(imageP, char*, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to directionally blur an image.
//
int
main(int argc, char** argv)
{
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 4) {
		cerr << "Usage: convolve in kernel out\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	char *kernel = argv[2];
	// perform convolve on an image and save result in file
	convolve(I1, kernel, I2);
	IP_saveImage(I2, argv[3]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// convolve:
//
// Convolve an image.
// 

void
convolve(imageP I1, char *kernel, imageP I2)
{
	int kw, kh, i, j, x, y, out_itr, wpad, hpad, total, pad_itr, mysum, w, h, padded_w,
		left, right, myArray[2];
	float kweight, ksum, cread;
	uchar *in, *out;

	//Read input from kernel file
    ifstream file(kernel);
    if(file.is_open()) {
    	file >> kw;		file >> kh;
    }
	float myKernel[kw*kh], fixKernel[kw*kh];

    if(file.is_open())
		for(i=0; i < kw*kh; i++)
			file >> myKernel[i];

	for(i=0, kweight=0.; i<kw*kh; i++) kweight += myKernel[i];

	if(kweight==0) kweight=1;

	w = I1->width;		h = I1->height;		total = w * h;		in  = I1->image;
	wpad=(int)(kw/2);	hpad=(int)(kh/2);

	int con_arr[kh*kw], padded[(I1->width+(kw-1))*(I1->height+(kh-1))];

	// init I2 dimensions and buffer
	I2->width  = I1->width;			I2->height = I1->height;
	I2->image  = (uchar *) malloc(I1->width*I1->height);
	if(I2->image == NULL) {
		cerr << "convolve: Insufficient memory\n";
		exit(1);
	}

	out = I2->image;
	out_itr = 0;

	//create padded cells above image
	for(y=0; y<hpad; y++) {
		for(i=0; i<w; i++) {
			if(i%w==0 || i%w==(w-1))
				for(pad_itr=0; pad_itr<wpad+1; pad_itr++) padded[out_itr++] = in[i];
			else padded[out_itr++] = in[i];
		}
	}
	//create padded cells to left and right of image
	for(i=0; i<total; i++) {
		if(i%w==0 || i%w==(w-1))
			for(pad_itr=0; pad_itr<wpad+1; pad_itr++) padded[out_itr++] = in[i];
		else padded[out_itr++] = in[i];
	}
	//create padded cells on bottom of image
	for(y=0; y<hpad; y++) {
		for(i=0; i<w; i++) {
			if(i%w==0 || i%w==(w-1))
				for(pad_itr=0; pad_itr<wpad+1; pad_itr++) padded[out_itr++] = in[i];
			else padded[out_itr++] = in[i];
		}
	}

	//padded image variables
	padded_w = w+kw-1;			left = wpad;		right = w+wpad;		out_itr=0;

	//iterate through the pixels of the image to be calculated
	//iterate topo to bottom
	for(y=hpad; y<h+hpad; y++) {
		//iterate left to right
		for(x=y*padded_w+wpad; x<y*padded_w+right; x++) {
			//iterate top to bottom for kernel
			for(j=x-wpad-padded_w*hpad, pad_itr=0; j<x-wpad+kh+padded_w*hpad; j+=padded_w)
				//iterate left to right of kernel
				for(i=j; i<j+kw; i++)
					//create array of pixel and neighbors to match with kernel
					con_arr[pad_itr++]=padded[i];
			for(i=0, ksum=0; i<=kw*kh; i++)
				//sum for weighted average
				ksum+=con_arr[i]*myKernel[i];
			//weighted average
			if(ksum/kweight<0) out[out_itr++]=0;
			else if(ksum/kweight>255) out[out_itr++]=255;
			else out[out_itr++]=(int)ksum/kweight;
		}
	}
}