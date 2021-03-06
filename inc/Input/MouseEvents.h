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

struct MouseInfo;

//-----------------------------------//

/**
 * Different types of mouse buttons.
 */

struct MouseButton
{
	enum Enum
	{
		// Main mouse buttons
		Left,
		Right,
		Middle,

		// Extra mouse buttons
		Mouse4,
		Mouse5,
	};
};

//-----------------------------------//

/**
 * Different types of mouse events.
 */

struct MouseEventType
{
	enum Enum
	{
		// Occurs when the mouse wheel is scrolled
		MouseWheelMove,
		// Occurs when the mouse is moved
		MouseMove,
		// Occurs when the mouse is dragged
		MouseDrag,
		// Occurs when a mouse button is pressed
		MousePress,
		// Occurs when a mouse button is released
		MouseRelease,
		// Occurs when the mouse cursor enters the window
		MouseEnter,
		// Occurs when the mouse cursor exits the window
		MouseExit,
	};
};

//-----------------------------------//

/**
 * Generic mouse event that will be inherited by the more specific 
 * mouse event types. This will be used for communication between
 * the input manager, so it has private members and then adds friend
 * access to the Mouse class. That way the private data is hidden to
 * the public.
 */

struct API_ENGINE MouseEvent : public InputEvent
{
	friend class Mouse;
	
	MouseEvent( MouseEventType::Enum eventType );

	MouseEventType::Enum eventType;
};

//-----------------------------------//

/**
 * Mouse event that occurs when the mouse is moved and provides the
 * current mouse position on the screen in screen-coordinates.
 */

struct API_ENGINE MouseMoveEvent : public MouseEvent
{
	MouseMoveEvent();

	short x;
	short y;
};

//-----------------------------------//

/**
 * Mouse event that occurs when the mouse is dragged. Use MouseInfo
 * to get more information about which mouse buttons are pressed.
 */

struct API_ENGINE MouseDragEvent : public MouseEvent
{
	MouseDragEvent();

	short dx;
	short dy;

	short x;
	short y;

	MouseInfo* info;
};

//-----------------------------------//
 
/**
 * Mouse event that occurs when a button of the mouse is pressed.
 * It holds the button that was pressed and also the position of the 
 * mouse in the window at the time of the click.
 */

struct API_ENGINE MouseButtonEvent : public MouseEvent
{
	MouseButtonEvent( MouseEventType::Enum eventType );

	// Convenience button test methods.
	bool isLeftButton() const { return button == MouseButton::Left; }
	bool isRightButton() const { return button == MouseButton::Right; }
	bool isMiddleButton() const { return button == MouseButton::Middle; }

	short x;
	short y;

	MouseButton::Enum button;
};

//-----------------------------------//

/**
 * Mouse events that occurs when the mouse wheel is scrolled.
 * In this case the extra information is a delta value that specifies
 * the relative amount of the mouse wheel that was scrolled.
 */

struct API_ENGINE MouseWheelEvent : public MouseEvent
{
	MouseWheelEvent();

	short delta;
};

//-----------------------------------//

NAMESPACE_ENGINE_END
