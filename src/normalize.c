/*
 * normalize.c
 *
 *  Created on: 18 Nov 2013
 *      Author: kelcey
 */

#include <stdlib.h>
#include <math.h>

void normalize(float* buffer, int num_samps, float maxval)
{
    float loudest_sample = 0.0;
    float multiplier = 0.0;
    int i;

    for (i = 0; i < num_samps; i++)
    {
        if (fabs(buffer[i]) > loudest_sample) loudest_sample = buffer[i];
    }

    multiplier = maxval / loudest_sample;

    for (i = 0; i < num_samps; i++)
    {
        buffer[i] *= multiplier;
    }
}
