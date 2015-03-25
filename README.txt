README.txt
Authors: Donald Villarreal & Joseph Wagner
Class: CSC470

unordered_dither in n gamma out [IN PROGRESS!]
Function unordered_dither reads the input in file in and applies unordered (random) dither to it before quantizing it to n quantization levels. The output is stored in file out.

blur in xsz ysz out
This function will take an input image and perform averaging neighborhood operations of xsz by ysz box.  The output is stored in the output file, out.

sharpen in sz fctr out
This function will take an input image and perform blur using a sz by sz box.  This time, the blurred image is subtracted from the input image, multiplied by fctr and added back to in.  The ranges are then clipped to [0, 255] and stored in the output file, out.