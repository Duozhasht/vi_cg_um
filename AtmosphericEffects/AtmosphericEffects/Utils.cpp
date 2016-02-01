#include "Utils.hpp"

int FrameCounter::tickindex = 0;
float FrameCounter::ticksum = 0;
float FrameCounter::ticklist[Samples] = { 0 };