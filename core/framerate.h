/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __FRAMERATE__
#define __FRAMERATE__

	#include "typedefs.h"

namespace owl {

//!
class	FrameRate
{
public:

static	FrameRate		framerate;

private:

		uint			start_time;
		uint			current_time;

		uint			oldTime;
		uint			lastTime;

		float			speedFactor;

		int				fpsCount;
		int				fps;

		uint			frame_time;

public:

		FrameRate();

		void			OnUpdate();
		bool			FixUpdateRateTo(const uint fps);

inline	int&			GetFps()												{	return fps;   }
inline	float&			GetSpeedFactor()										{	return speedFactor;   }
inline	uint			GetCurrentTimeMsec()									{	return current_time;   }
};

}		//	owl
#endif	//	__FRAMERATE__
