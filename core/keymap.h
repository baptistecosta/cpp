/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __KEYMAP__
#define __KEYMAP__

namespace owl {

//!
struct	KeyPress
{
static	const bool			leftCtrl();
static	const bool			rightCtrl();
static	const bool			leftShift();
static	const bool			rightShift();
};

//!
struct	KeyMap
{
	enum
	{
		Up			=	0xa000,
		Down		=	0xa001,
		Left		=	0xa002,
		Right		=	0xa003,

		A			=	0xb000,
		B			=	0xb001,
		C			=	0xb002,
		D			=	0xb003,
		E			=	0xb004,
		F			=	0xb005,
		G			=	0xb006,
		H			=	0xb007,
		I			=	0xb008,
		J			=	0xb009,
		K			=	0xb010,
		L			=	0xb011,
		M			=	0xb012,
		N			=	0xb013,
		O			=	0xb014,
		P			=	0xb015,
		Q			=	0xb016,
		R			=	0xb017,
		S			=	0xb018,
		T			=	0xb019,
		U			=	0xb020,
		V			=	0xb021,
		W			=	0xb022,
		X			=	0xb023,
		Y			=	0xb024,
		Z			=	0xb025,

		Kp_0		=	0xc000,
		Kp_1		=	0xc001,
		Kp_2		=	0xc002,
		Kp_3		=	0xc003,
		Kp_4		=	0xc004,
		Kp_5		=	0xc005,
		Kp_6		=	0xc006,
		Kp_7		=	0xc007,
		Kp_8		=	0xc008,
		Kp_9		=	0xc009,

		Insert		=	0xd000,
		Del			=	0xd001,
		Home		=	0xd002,
		End			=	0xd003,
		PageUp		=	0xd004,
		PageDown	=	0xd005
	};
};

}		//	owl
#endif	//	__KEYMAP__