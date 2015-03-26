README.txt
Authors: Donald Villarreal & Joseph Wagner
Class: CSC470

unordered_dither in n gamma out [IN PROGRESS!]
Function unordered_dither reads the input in file in and applies unordered (random) dither to it before quantizing it to n quantization levels. The output is stored in file out.

blur in xsz ysz out
This function will take an input image, n and perform averaging neighborhood operations of xsz by ysz box.  The output is stored in the output file, out.

sharpen in sz fctr out
This function will take an input image, n and perform blur using a sz by sz box.  This time, the blurred image is subtracted from the input image, multiplied by fctr and added back to in.  The ranges are then clipped to [0, 255] and stored in the output file, out.

median in sz avg_nbrs out
This function will take an input image, in and apply a sz by sz kernel to each pixel.  The median is found and avg_nbrs below and above the median are averaged.  This is calculated along each pixel and stored in the outfile, out.

convolve in kernel out
This function will take an input image, in and a kernel file, kernel.  The first row of the kernel file is displays the height and width of the kernel.  The next lines is the kernel itself.  The kernel is then applied to each pixel and it's neighbors by multiplying the file kernel with the image kernel and dividing by the sum of the file kernel (or 1 if the sum is 0).  The output file is stored in out.