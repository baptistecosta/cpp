/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __COLOR__
#define __COLOR__

namespace owl {

//!
struct	Color
{
public:

		float			m_r, m_g, m_b, m_a;

		Color			()																	{	m_r = 0.f, m_g = 0.f, m_b = 0.f, m_a = 1.f;	}
		Color			(const float r, const float g, const float b)						{	m_r = r, m_g = g, m_b = b, m_a = 1.f;	}
		Color			(const float r, const float g, const float b, const float a)		{	m_r = r, m_g = g, m_b = b, m_a = a;	}
		Color			(const int r, const int g, const int b)								{	float q = LevelPerChannelInv; m_r = r * q, m_g = g * q, m_b = b * q, m_a = 1.f;	}
		Color			(const int r, const int g, const int b, const int a)				{	float q = LevelPerChannelInv; m_r = r * q, m_g = g * q, m_b = b * q, m_a = a * q;	}

		Color			operator +	(const Color& c)										{	return Color(m_r + c.m_r, m_g + c.m_g, m_b + c.m_b, 1.f);   }
		Color			operator +	(const float& k)										{	return Color(m_r + k, m_g + k, m_b + k, 1.f);   }
		Color			operator +	(int& k)												{	float fk = k * LevelPerChannelInv; return Color(m_r + fk, m_g + fk, m_b + fk, 1.f);   }
		Color			operator -	(const Color& c)										{	return Color(m_r - c.m_r, m_g - c.m_g, m_b - c.m_b, 1.f);   }
		Color			operator -	(const float& k)										{	return Color(m_r - k, m_g - k, m_b - k, 1.f);   }
		Color			operator -	(int& k)												{	float fk = k * LevelPerChannelInv; return Color(m_r - fk, m_g - fk, m_b - fk, 1.f);   }

static	const float		LevelPerChannel,
						LevelPerChannelInv;

static	const Color		White,
						Black,
						Red,
						Green,
						Blue,
						Candle,
						Tungst40,
						Tungst100,
						Sunlight,
						Skyoc;
};

}		// owl
#endif	// __COLOR__
