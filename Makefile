#---------------------------------------------------------------#
# Homework 2 Makefile
#	Joseph Wagner		(phraxos@phraxos.biz)
#	Donald Villarreal	(donaldjvillarreal@gmail.com)
#---------------------------------------------------------------#

CC	= g++	# Use gcc compiler. Use g++ for C++ compilation.
CFLAGS	=	# Compiler flags: -g for debug, -O for optimization
LDFLAGS	= 	# Linker flags
LIBS	= -lm	# Libraries to link with (-lm is the math library)

# This is the Master Compile command, which can be used for
# all executables. Simply make a new entry for each one underneath,
# and change the ALL variable.
# YOU SHOULD NOT NEED TO TOUCH THIS.
#
#   $@ stands for the target name (i.e., the resulting executable)
#   $? stands for the dependency list (i.e., the .o files)
#
COMPILE_EXECUTABLE = $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $? $(LIBS)

# These are all the files to be compiled.
ALL	= unordered_dither	ordered_dither	blur sharpen

all:	$(ALL)

# List of executable targets

unordered_dither: 	$($@.o) IPutil.o
ordered_dither: 	$($@.o) IPutil.o
blur:				$($@.o) IPutil.o
sharpen:			$($@.o) IPutil.o

# Clean target to remove backup, object, and core files
clean:
	rm -f *~ *.o core
cleanx:
	rm -f *~ *.o core $(ALL)

# Dependencies
unordered_dither.o: 	IP.h
ordered_dither.o: 		IP.h
blur.o:					IP.h
sharpen.o:				IP.h