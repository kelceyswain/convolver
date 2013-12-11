/*
 * splitchans.h
 *
 *  Created on: 18 Nov 2013
 *      Author: kelcey
 */

#ifndef SPLITCHANS_H_
#define SPLITCHANS_H_

void channel_split(float* buffer, int num_frames, float** chan_buffers, int num_channels);

void channel_join(float** chan_buffers, int num_channels, float* buffer, int num_frames);

void make_mono(float* inbuffer, float* outbuffer, int num_channels, int num_frames);

#endif /* SPLITCHANS_H_ */
