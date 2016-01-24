#ifndef UTILS_HPP
#define UTILS_HPP

class FrameCounter
{
	static const int Samples = 100;

	static int tickindex;
	static int ticksum;
	static int ticklist[Samples];

public:
	static double getFps(int newtick)
	{
		ticksum += newtick;              /* add new value */
		ticklist[tickindex] = newtick;   /* save new value so it can be subtracted later */
		tickindex = (tickindex + 1) % Samples;

		if (!tickindex)
			ticksum -= ticklist[0];

		return 1/((double)ticksum / Samples);
	}
};

#endif