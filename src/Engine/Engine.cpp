/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Engine.h"
#include "ResourceLoaders.h"

#include "vapor/vfs/VFS.h"
#include "vapor/render/Device.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/scene/Scene.h"
#include "vapor/audio/Device.h"
#include "vapor/input/InputManager.h"
#include "vapor/script/State.h"
//#include "vapor/physics/Physics.h"
#include "vapor/TaskManager.h"

using namespace vapor::audio;
using namespace vapor::scene;
//using namespace vapor::physics;
using namespace vapor::render;

namespace vapor {

//-----------------------------------//

Engine::Engine()
	: renderDevice(nullptr),
	vfs(nullptr),
	resourceManager(nullptr),
	/*physicsManager(nullptr),*/
	log(nullptr),
	scriptState(nullptr),
	audioDevice(nullptr)
{ }

//-----------------------------------//

void Engine::create(const std::string& _app, const char** _argv, bool autoInit)
{
	app = _app;
	argv = _argv;

	if( autoInit )
		init();
}

//-----------------------------------//

Engine::~Engine()
{
	sceneManager.reset();

	foreach( SubsystemPtr sub, subsystems )
		delete sub;

	delete audioDevice;
	//delete physicsManager;
	delete scriptState;
	delete renderDevice;
	delete resourceManager;
	delete vfs;
	delete log;
}

//-----------------------------------//

void Engine::init( bool createWindow )
{
	// setup the global logger
	setupLogger();

	info( "engine", "Starting vaporEngine version '%s'", VAPOR_ENGINE_VERSION );

	// create the virtual filesystem
	vfs = new VFS(app, argv ? argv[0] : nullptr);

	taskManager = new TaskManager();
	subsystems.push_back( taskManager );

	// create the resource manager
	resourceManager = new ResourceManager();
	
	// connect the resource manager and filesystem watcher
	vfs->getWatcher()->onWatchEvent += 
		fd::bind(&ResourceManager::handleWatchResource, resourceManager);

	// create the physics manager
	//physicsManager = PhysicsManager::getInstancePtr();

	// register default codecs
	setupResourceLoaders();

	// create a rendering and audio device
	setupDevices( createWindow );

	// create the root scene node
	sceneManager.reset( new scene::Scene() );

#ifdef VAPOR_SCRIPTING_LUA
	// Initialize the scripting
	scriptState = new script::State();
	scriptState->bind( this );
#endif
}

//-----------------------------------//

void Engine::setupLogger()
{
	// get a suitabfle log filename
	std::string file = app + ".html";
	
	// create and set a new logger
	log = new Log(app, file);
}

//-----------------------------------//

void Engine::setupDevices( bool createWindow )
{
	// create render device
	renderDevice = new render::Device();

	if( createWindow )
	{
		// create a window and set the title
		renderDevice->createWindow();

		// init the render device now that it has a context
		renderDevice->init();
		
		// creates the default input devices
		setupInput();
	}

#ifdef VAPOR_AUDIO_OPENAL

	// create the audio device
	audioDevice = new audio::Device();

#endif
}

//-----------------------------------//

void Engine::setupInput()
{
	WindowPtr window = renderDevice->getWindow();
	InputManager& im = window->getInputManager();

	// Let's register some input devices.
	im.addDevice( new Keyboard() );
	im.addDevice( new Mouse() );
}

//-----------------------------------//

void Engine::setupResourceLoaders()
{
	assert( resourceManager != nullptr );

	std::vector<ResourceLoaderPtr> loaders;

	// register default compiled codecs
	#ifdef VAPOR_IMAGE_PICOPNG
		loaders.push_back( new PNG_Pico_Loader() );
	#endif

	#ifdef VAPOR_MESH_MILKSHAPE3D
		loaders.push_back( new MS3D_Loader() );
	#endif

	#ifdef VAPOR_AUDIO_OGG
		loaders.push_back( new OGG_Loader() );
	#endif

	#ifdef VAPOR_SHADER_GLSL
		loaders.push_back( new GLSL_Loader() );
	#endif

	#ifdef VAPOR_SCRIPTING_LUA
		loaders.push_back( new Lua_Loader() );
	#endif

	#ifdef VAPOR_IMAGE_STB
		loaders.push_back( new STB_Image_Loader() );
	#endif
	
	#ifdef VAPOR_FONT_BITMAP
		loaders.push_back( new Font_Loader() );
	#endif

	foreach( const ResourceLoaderPtr& loader, loaders )
	{
		resourceManager->registerLoader( loader );
	}
}

//-----------------------------------//

void Engine::update( double delta )
{
	foreach( Subsystem* subsystem, subsystems )
		subsystem->update( delta );

	vfs->update( delta );
	resourceManager->update( delta );
	sceneManager->update( delta );

#ifdef VAPOR_SCRIPTING_LUA
	this->getScriptState()->update( delta );
#endif

	//this->getPhysicsManager()->update( delta );
}

//-----------------------------------//

InputManager* const Engine::getInputManager() const
{
	return &(renderDevice->getWindow()->getInputManager());
}

//-----------------------------------//

} // end namespace