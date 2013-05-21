/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "color.h"

using namespace owl;

//-----------------------------------------------------------------------------
const float		Color::LevelPerChannel = 255.f;
const float		Color::LevelPerChannelInv = 1.f / Color::LevelPerChannel;

//-----------------------------------------------------------------------------
const Color		Color::White(1.f, 1.f, 1.f);
const Color		Color::Black(0.f, 0.f, 0.f);
const Color		Color::Red(1.f, 0.f, 0.f);
const Color		Color::Green(0.f, 1.f, 0.f);
const Color		Color::Blue(0.f, 0.f, 1.f);
const Color		Color::Candle(1.f, 0.57f, 0.16f);			// Candle light
const Color		Color::Tungst40(1.f, 0.769f, 0.558f);		// Tungsten 40W
const Color		Color::Tungst100(1.f, 0.835f, 0.664f);		// Tungsten 100W
const Color		Color::Sunlight(1.f, 1.f, 1.f);				// Direct sunlight
const Color		Color::Skyoc(0.785f, 0.882f, 1.f);			// Overcast sky
