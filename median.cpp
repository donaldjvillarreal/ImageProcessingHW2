///////
// ================================================================
// median.cpp - Median program
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include <algorithm>
#include "IP.h"
using namespace std;

// function prototype
void median(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to directionally blur an image.
//
int
main(int argc, char** argv)
{
	int	sz, avg_nbrs; 
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: median in sz avg_nbrs out\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	sz  = atoi(argv[2]);

	if(sz < 1 || sz > 127) {
		cerr << "sz must be a positive integer less than 128 [1, 127]\n";
		exit(1);
	}

	if(sz%2 == 0) {
		cerr << "sz must be an odd integer\n";
		exit(1);
	}

	avg_nbrs  = atoi(argv[3]);

	if(avg_nbrs > ((sz*sz-1)/2) || avg_nbrs < 0) {
		cerr << "avg_nbrs must be between 0 and half of one less than the size ((sz-1)/2)\n";
		exit(1);
	}


	// perform median of image and save result in file
	median(I1, sz, avg_nbrs, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// median:
//
// Median of an image.
// 

void
median(imageP I1, int sz, int avg_nbrs, imageP I2)
{
	int i, j, x, y, out_itr, pad, total, pad_itr, mysum, w, h, med_arr[sz*sz], padded_w,
		left, right, avg_denom, ksum, med,
		padded[(I1->width+(sz-1))*(I1->height+(sz-1))];
	uchar *in, *out;

	w = I1->width;		h = I1->height;		total = w * h;		in  = I1->image;
	pad=(int)(sz/2);

	// init I2 dimensions and buffer
	I2->width  = I1->width;			I2->height = I1->height;
	I2->image  = (uchar *) malloc(I1->width*I1->height);
	if(I2->image == NULL) {
		cerr << "median: Insufficient memory\n";
		exit(1);
	}

	out = I2->image;
	out_itr = 0;

	//create padded cells above image
	for(y=0; y<pad; y++) {
		for(i=0; i<w; i++) {
			if(i%w==0 || i%w==(w-1))
				for(pad_itr=0; pad_itr<pad+1; pad_itr++) padded[out_itr++] = in[i];
			else padded[out_itr++] = in[i];
		}
	}
	//create padded cells to left and right of image
	for(i=0; i<total; i++) {
		if(i%w==0 || i%w==(w-1))
			for(pad_itr=0; pad_itr<pad+1; pad_itr++) padded[out_itr++] = in[i];
		else padded[out_itr++] = in[i];
	}
	//create padded cells on bottom of image
	for(y=0; y<pad; y++) {
		for(i=0; i<w; i++) {
			if(i%w==0 || i%w==(w-1))
				for(pad_itr=0; pad_itr<pad+1; pad_itr++) padded[out_itr++] = in[i];
			else padded[out_itr++] = in[i];
		}
	}
	//padded image variables
	padded_w = w+sz-1;			left = pad;		right = w+pad;		out_itr=0;
	avg_denom = (avg_nbrs*2)+1;		med = (int)((sz*sz)/2);

	//iterate through the pixels of the image to be calculated
	//iterate topo to bottom
	for(y=pad; y<h+pad; y++) {
		//iterate left to right
		for(x=y*padded_w+pad; x<y*padded_w+right; x++) {
			//iterate top to bottom for kernel
			for(j=x-pad-padded_w*pad, pad_itr=0; j<x-pad+sz+padded_w*pad; j+=padded_w)
				//iterate left to right of kernel
				for(i=j; i<j+sz; i++)
					//create array to be sorted to find median and neighbors
					med_arr[pad_itr++]=padded[i];
			//sort array
			sort(med_arr, med_arr + (sz*sz));
			//select median and avg_nbrs below and above median
			for(i=med-avg_nbrs, ksum=0; i<=med+avg_nbrs; i++)
				//sum for average
				ksum+=med_arr[i];
			//average of the median and avg_nbrs below and above median
			out[out_itr++]=ksum/avg_denom;
		}
	}
}