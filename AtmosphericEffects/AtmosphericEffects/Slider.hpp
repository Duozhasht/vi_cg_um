#ifndef SLIDER_HPP
#define SLIDER_HPP

struct Slider
{
	void bind(float *v, float s) { value = v; stepSize = s; }

	void slideUp() { *value += stepSize; }
	void slideDown() { *value -= stepSize; }

	float *value;
	float stepSize;
};

#endif