/*
 * splitchans.c
 *
 *  Created on: 18 Nov 2013
 *      Author: kelcey
 */

void channel_split(float* buffer, int num_frames, float** chan_buffers, int num_channels)
{
    int i;
    int samples = num_frames * num_channels;
    for (i = 0; i < samples; i++)
    {
        chan_buffers[(i % num_channels)][i/num_channels] = buffer[i];
    }
}

void channel_join(float** chan_buffers, int num_channels, float* buffer, int num_frames)
{
    int i;
    int samples = num_frames * num_channels;
    for (i = 0; i < samples; i++)
    {
        buffer[i] = chan_buffers[i % num_channels][i / num_channels];
    }
}

void make_mono(float* inbuffer, float* outbuffer, int num_channels, int num_frames)
{
    int i;
    for (i = 0; i < num_frames; i++)
    {
        int j;
        float sum = 0.0;
        for (j = 0; j < num_channels; j++)
        {
            sum += inbuffer[(i * num_channels) + j];
        }
        sum /= (float)num_channels;
        outbuffer[i] = sum;
    }
}
