/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/resources/Resource.h"

namespace vapor {

class State;

//-----------------------------------//

/**
 * Scripting context that can be used to execute scripts. Currently this
 * represents a Lua interpreter state.
 */

class VAPOR_API Script : public Resource
{
public:

	// Execute the script.
	void execute();

	// Gets the source code of the script.
	ACESSOR(Source, const std::string&, text)

	// Sets the state where the script will execute.
	ACESSOR(State, State* const, state)

	// Gets the associated resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum,
		ResourceGroup::Scripts)

protected:

	// Source code of the script.
	std::string text;

	// State where the script will execute.
	State* state;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Script );

//-----------------------------------//

} // end namespace

//#endif