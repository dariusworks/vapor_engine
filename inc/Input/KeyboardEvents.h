/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "input/Device.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//	

/**
 * Represents all the available keyboard keys with a unique key code.
 * When interacting with the Keyboard class you should always use
 * these values.
 */

struct Keys
{
	enum Enum
	{
		// Alphabet keys
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',
		
		// Numeric keys
		Num0 = '0',
		Num1 = '1',
		Num2 = '2',
		Num3 = '3',
		Num4 = '4',
		Num5 = '5',
		Num6 = '6',
		Num7 = '7',
		Num8 = '8',
		Num9 = '9', 
		
		// Special keys
		Escape = 256,
		LControl,
		LShift,
		LAlt,
		LSuper,
		RControl,
		RShift,
		RAlt,
		RSuper,
		
		Menu,
		LBracket,
		RBracket,
		SemiColon,
		Comma,
		Period,
		Quote,
		Slash,
		BackSlash,
		Tilde,
		Equal,
		Dash,
		Space,
		Return,
		Back,
		Tab,
		
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		
		// Numeric operations
		Add,
		Subtract,
		Multiply,
		Divide,
		
		// Arrows
		Left,
		Right,
		Up,
		Down,
		
		// Numpad numbers
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		
		// F-keys
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		
		Pause,
		MAX
	};
};

//-----------------------------------//

/**
 * Different types of keyboard events.
 */

struct KeyboardEventType
{
	enum Enum
	{
		// Occurs when a keyboard key is pressed
		KeyPressed,
		
		// Occurs when a keyboard key is released
		KeyReleased,
		
		// Occurs when text is input from the keyboard.
		KeyText
	};
};

//-----------------------------------//

/**
 * Main keyboard event. Holds the related keycode, and whether some 
 * special keys are pressed (handy for testing shortcuts). Finally
 * it also needs to know if it's a key press or release event.
 */

struct API_ENGINE KeyEvent : public InputEvent
{
	friend class Keyboard;
	
	KeyEvent(KeyboardEventType::Enum eventType);
	
	KeyboardEventType::Enum eventType;

	Keys::Enum keyCode;
	bool altPressed, shiftPressed, ctrlPressed;
	uint32 unicode;
};

//-----------------------------------//

NAMESPACE_ENGINE_END
