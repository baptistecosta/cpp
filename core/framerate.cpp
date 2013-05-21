/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <time.h>
	#include "framerate.h"

using namespace owl;

//---------------------------------------------------------------------------
FrameRate		FrameRate::framerate;

//---------------------------------------------------------------------------
FrameRate::FrameRate()
{
	start_time	= clock();
	oldTime	= 0;
	lastTime = 0;

	speedFactor = 0;

	fpsCount = 0;
	fps	= 0;

	frame_time = 0;
}

//---------------------------------------------------------------------------
void			FrameRate::OnUpdate()
{
	current_time = clock();

	uint tick = current_time - start_time;

	if (oldTime + 1000 < tick)
	{
		oldTime = tick;
		fps = fpsCount;
		fpsCount = 0;
	}

	speedFactor = ((tick - lastTime) / 1000.0f) * 64;

	lastTime = tick;

	fpsCount++;
}

//---------------------------------------------------------------------------
bool			FrameRate::FixUpdateRateTo(const uint fps)
{
	if ((frame_time + 1000 / fps) > current_time)
		return false;

	frame_time = current_time;
	return true;
}

