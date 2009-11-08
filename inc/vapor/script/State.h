/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_SCRIPTING_LUA

struct lua_State;

#include "vapor/Platform.h"
#include "vapor/script/Script.h"

namespace vapor {

	class Engine;

	namespace script {

//-----------------------------------//

/**
 * Scripting context that can be used to execute scripts. Currently this
 * represents a Lua interpreter state. Executes a collection of scripts.
 */

class VAPOR_API State : private boost::noncopyable
{
public:

	/// Constructs a new scripting VM.
	State();
	
	/// Destructs this scripting VM.
	~State();

	// Creates a new script.
	//ScriptPtr createScript();

	/// Executes a piece of source code.
	void execute( const std::string& source );

	/// Executes a script resource.
	void execute( Script* script );

	/// Bind the engine API to Lua.
	void bind( Engine* engine );

	/// Registers a script into this state.
	void registerScript( ScriptPtr script );

	/// Handles the Lua error states.
	void handleError();

	/// Executes all the registered scripts.
	void update( float deltaTime );

protected:

	/// Lua VM state.
	lua_State* luaState;

	/// Holds Lua scripts.
	std::vector< ScriptPtr > scripts;
	typedef std::vector< ScriptPtr >::iterator scriptsIterator;
};

//-----------------------------------//

//TYPEDEF_SHARED_POINTER_FROM_CLASS( Context );

//-----------------------------------//

} } // end namespaces

#endif