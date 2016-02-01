#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

class FrameCounter
{
	static const int Samples = 100;

	static int tickindex;
	static float ticksum;
	static float ticklist[Samples];

public:
	static double getFps(float newtick)
	{
		ticksum -= ticklist[tickindex];
		ticksum += newtick;              /* add new value */
		ticklist[tickindex] = newtick;   /* save new value so it can be subtracted later */
		tickindex = (tickindex + 1) % Samples;

		return 1/(ticksum / (float)Samples);
	}
};

#endif