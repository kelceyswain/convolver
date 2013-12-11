/*
 * main.c
 *
 *  Created on: 18 Nov 2013
 *      Author: kelcey
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sndfile.h>
#include "splitchans.h"
#include "normalize.h"

int main(int argc, char** argv)
{
    // Initialize the variables
    enum {ARG_NAME, ARG_INFILE1, ARG_INFILE2, ARG_OUTFILE, ARG_NARGS};
    int i, j, k;
    time_t current_time, end_time;
    double process_time;
    SNDFILE* infile1;
    SNDFILE* infile2;
    SNDFILE* outfile;
    SF_INFO props1;
    SF_INFO props2;
    SF_INFO props3;
    int outFrames;
    float *b1;
    float *b2;
    float *b3;
    float** buf1;
    float** buf2;
    float** buf3;

    // Say hello
    printf("Audio Convolver\nby Kelcey Swain\n\n");
    current_time = time(NULL);

    // Check the inputs
    if (argc != ARG_NARGS)
    {
        fprintf(stderr, "Error: incorrect number of arguments.\nUsage: %s infile1 infile2 outfile\n", argv[ARG_NAME]);
        return -1;
    }

    // Initialize the files
    infile1 = sf_open(argv[ARG_INFILE1], SFM_READ, &props1);
    infile2 = sf_open(argv[ARG_INFILE2], SFM_READ, &props2);

    if (infile1 == NULL)
    {
        fprintf(stderr, "Error opening %s\n", argv[ARG_INFILE1]);
        return -1;
    }

    if (infile2 == NULL)
    {
        fprintf(stderr, "Error opening %s\n", argv[ARG_INFILE2]);
        return -1;
    }

    if (props1.samplerate != props2.samplerate)
    {
        fprintf(stderr, "Error, samplerates must match:\n%s:\t%i\n%s:\t%i\n",
                argv[ARG_INFILE1], props1.samplerate,
                argv[ARG_INFILE2], props2.samplerate
               );
        return -1;
    }

    if (props1.channels != props2.channels)
    {
        fprintf(stderr, "Error, channels must match:\n%s:\t%i\n%s:\t%i\n",
                argv[ARG_INFILE1], props1.channels,
                argv[ARG_INFILE2], props2.channels
               );
        return -1;
    }

    // Let's make some buffers and the output file
    outFrames = (props1.frames + props2.frames - 1);

    props3 = props1;
    props3.frames = outFrames;
    outfile = sf_open(argv[ARG_OUTFILE], SFM_WRITE, &props3);

    buf1 = (float**)malloc(sizeof(float*) * props1.channels);
    buf2 = (float**)malloc(sizeof(float*) * props2.channels);
    buf3 = (float**)malloc(sizeof(float*) * props1.channels);


    for (i = 0; i < props1.channels; i++)
    {
        buf1[i] = (float*)malloc(sizeof(float) * props1.frames);
        buf2[i] = (float*)malloc(sizeof(float) * props2.frames);
        buf3[i] = (float*)malloc(sizeof(float) * outFrames);
    }

    b1 = (float*)malloc(sizeof(float) * props1.frames * props1.channels);
    b2 = (float*)malloc(sizeof(float) * props2.frames * props2.channels);
    b3 = (float*)malloc(sizeof(float) * outFrames * props1.channels);

    // Read
    sf_readf_float(infile1, b1, props1.frames);
    sf_readf_float(infile2, b2, props2.frames);

    if (!b1)
    {
        fprintf(stderr, "Error reading %s\n", argv[ARG_INFILE1]);
        return -1;
    }
    if (!b2)
    {
        fprintf(stderr, "Error reading %s\n", argv[ARG_INFILE2]);
        return -1;
    }

    // Split channels
    channel_split(b1, props1.frames, buf1, props1.channels);
    channel_split(b2, props2.frames, buf2, props2.channels);

    for (k = 0; k < props1.channels; k++)
    {
    	for (i = 0; i < outFrames; i ++)
    	{
    		buf3[k][i] = 0.0;
    	}
    }
    // Convolve

    for (k = 0; k < props1.channels; k++)
    {
    	for (i = 0; i < props1.frames; i++)
    	{
    		for ( j = 0; j < props2.frames; j++)
    		{
    			buf3[k][i+j] += buf1[k][i] * buf2[k][j];
    		}
    	}
    }

    // Join output file
    channel_join(buf3, props1.channels, b3, outFrames);

    // Normalize
    normalize(b3, outFrames*props1.channels, 0.8);

    // Write file
    sf_writef_float(outfile, b3, outFrames);

    // Clear up
    if (infile1) sf_close(infile1);
    if (infile2) sf_close(infile2);
    if (outfile) sf_close(outfile);

    for (i = 0; i < props1.channels; i++)
    {
        if (buf1[i]) free(buf1[i]);
        if (buf2[i]) free(buf2[i]);
        if (buf3[i]) free(buf3[i]);
    }

    if (buf1) free(buf1);
    if (buf2) free(buf2);
    if (buf3) free(buf3);

    if (b1) free(b1);
    if (b2) free(b2);

    // Exit with an all clear
    end_time = time(NULL);
    process_time = difftime(end_time, current_time);

    printf("Convolution complete in %.1f seconds.\n", process_time);
    return 0;
}
