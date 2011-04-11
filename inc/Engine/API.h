/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Core Includes
//---------------------------------------------------------------------//

#include "Core/API.h"
#include "Resources/API.h"

//---------------------------------------------------------------------//
// Engine APIs
//---------------------------------------------------------------------//

// Enables visual styled controls on Windows
//#define	VAPOR_USE_COMMON_CONTROLS

//#define	VAPOR_WINDOWING_SFML
#define	VAPOR_WINDOWING_WIN32
//#define	VAPOR_INPUT_SFML

#define		VAPOR_RENDERER_OPENGL
//#define	VAPOR_RENDERER_DIRECTX
#define		VAPOR_SHADER_GLSL

#define		VAPOR_AUDIO_OPENAL
#define		VAPOR_PHYSICS_BULLET

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(VAPOR_API_DLL) && defined(VAPOR_API_DLL_EXPORT)
	#ifdef VAPOR_PLATFORM_WINDOWS
		#define VAPOR_API __declspec( dllexport )
	#else
		#define VAPOR_API __attribute__ ((visibility("default")))
	#endif
#elif defined(VAPOR_API_DLL) && defined(VAPOR_PLATFORM_WINDOWS)
	#define VAPOR_API __declspec( dllimport )
#else
	#define VAPOR_API
#endif

//---------------------------------------------------------------------//
// Pointer wrappers
//---------------------------------------------------------------------//

#if defined(VAPOR_MEMORY_TR1_VENDOR)
	#if defined(VAPOR_COMPILER_MSVC)
		#include <memory>
		#include <functional>
	#elif defined(VAPOR_COMPILER_GCC)
		#include <tr1/memory>
	#endif
#elif defined(VAPOR_MEMORY_TR1_BOOST)
	#include <boost/tr1/memory.hpp>
#endif	

#if VAPOR_COMPILER_MSVC != VAPOR_COMPILER_MSVC_2010 
	namespace std { using namespace std::tr1; }
#endif

//---------------------------------------------------------------------//
// Logging. Use and abuse it.
//---------------------------------------------------------------------//

#include "Core/Log.h"