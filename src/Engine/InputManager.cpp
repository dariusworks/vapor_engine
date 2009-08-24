/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/input/InputManager.h"

namespace vapor {
	namespace input {

//-----------------------------------//

InputManager::InputManager()
{

}

//-----------------------------------//

InputManager::~InputManager()
{
	foreach( input::Device* device, devices )
	{
		delete device;
	}
}

//-----------------------------------//

void InputManager::addDevice( input::Device* device )
{
	if( !device )
	{
		warn( "input", "Tried to add an invalid input device" );
		return;
	}
	
	devices.push_back( device );

	info( "input", "Registered a new input device ('%s')", 
		DeviceType::getString( device->getType() ) );
}

//-----------------------------------//

Keyboard* InputManager::getKeyboard()
{
	foreach( input::Device* device, devices )
	{
		if( device->getType() == DeviceType::Keyboard )
		{
			return static_cast< Keyboard* > ( device );
		}
	}

	return nullptr;
}

//-----------------------------------//

Mouse* InputManager::getMouse()
{
	foreach( input::Device* device, devices )
	{
		if( device->getType() == DeviceType::Mouse )
		{
			return static_cast< Mouse* > ( device );
		}
	}

	return nullptr;
}

//-----------------------------------//

void InputManager::processEvent( const input::Event& event )
{
	foreach( input::Device* device, devices )
	{
		device->processEvent( event );
	}
}

//-----------------------------------//

} } // end namespaces