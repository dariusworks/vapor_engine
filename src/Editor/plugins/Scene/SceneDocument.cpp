/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "SceneDocument.h"
#include "RenderControl.h"
#include "Editor.h"
#include "ResourceDrop.h"
#include "Utilities.h"
#include "Events.h"
#include "EditorCameraController.h"

namespace vapor { namespace editor {

//-----------------------------------//

SceneDocument::SceneDocument()
	: viewFrame(nullptr)
{
	createView();
	createScene();

	ResourceManager* res = GetResourceManager();
	res->loadQueuedResources();

	// Update at least once before rendering.
	onUpdate(0);
}

//-----------------------------------//

SceneDocument::~SceneDocument()
{
}

//-----------------------------------//

wxWindow* SceneDocument::getWindow()
{
	return viewFrame;
}

//-----------------------------------//

void SceneDocument::setupRenderWindow()
{
	RenderDevice* renderDevice = GetEngine()->getRenderDevice();

	RenderControl* control = viewFrame->getControl();
	Window* window = (Window*) control->getRenderWindow();

	renderDevice->setWindow( window );
	renderDevice->setRenderTarget( window );

	renderDevice->init();
}

//-----------------------------------//

void SceneDocument::onDocumentSelect()
{
	setupRenderWindow();

	RenderControl* control = viewFrame->getControl();
	control->startFrameLoop();

	GetEditor().getAUI()->GetPane("Hierarchy").Show();
	GetEditor().getAUI()->GetPane("Properties").Show();
	GetEditor().getAUI()->Update();

	GetEditor().getEventManager()->onSceneLoad(scene);
}

//-----------------------------------//

void SceneDocument::onDocumentUnselect()
{
	RenderControl* control = viewFrame->getControl();
	control->stopFrameLoop();

	GetEditor().getAUI()->GetPane("Hierarchy").Hide();
	GetEditor().getAUI()->GetPane("Properties").Hide();
	GetEditor().getAUI()->Update();
}

//-----------------------------------//

void SceneDocument::createView()
{
	viewFrame = new Viewframe( &GetEditor() );

#if 0
	viewSplitter->SetWindow( 0, viewFrame );
	viewSplitter->SetExpanded( viewFrame );
#endif

	setupRenderWindow();

	RenderControl* control = viewFrame->getControl();
	control->onRender.Connect( this, &SceneDocument::onRender );
	control->onUpdate.Connect( this, &SceneDocument::onUpdate );
	control->SetDropTarget( new ResourceDropTarget( &GetEditor() ) );
	control->SetFocus();

	RenderView* view = viewFrame->createView();
	view->setClearColor( Color(0.0f, 0.10f, 0.25f) );
}

//-----------------------------------//

void SceneDocument::createScene()
{
	scene.reset( new Scene() );
	editorScene.reset( new Scene() );
	
	// Create a nice grid for the editor.
	EntityPtr nodeGrid( new Entity("Grid") );
	nodeGrid->addTransform();
	nodeGrid->addComponent( GridPtr( new Grid() ) );
	nodeGrid->setTag( Tags::NonPickable, true );
	editorScene->add( nodeGrid );

	Vector3 initialPosition(0, 20, -65);
	EntityPtr nodeCamera( createCamera() );
	nodeCamera->getTransform()->setPosition(initialPosition);
	editorScene->add( nodeCamera );

	CameraPtr camera = nodeCamera->getComponent<Camera>();
	viewFrame->setMainCamera(camera);
	viewFrame->switchToDefaultCamera();

	//eventManager->onSceneLoad(scene);
}

//-----------------------------------//

EntityPtr SceneDocument::createCamera()
{
	// So each camera will have unique names.
	static byte i = 0;

	// Create a new first-person camera for our view.
	// By default it will be in perspective projection.
	CameraPtr camera( new Camera() );

#if 0
	ComponentPtr cameraController( new EditorCameraController() );
#else
	ComponentPtr cameraController( new FirstPersonController() );
#endif

	Frustum& frustum = camera->getFrustum();
	frustum.farPlane = 10000;

	// Generate a new unique name.
	std::string name( "EditorCamera"+StringFromNumber(i++) );

	EntityPtr entityCamera( new Entity(name) );
	entityCamera->addTransform();
	entityCamera->addComponent( camera );
	entityCamera->addComponent( cameraController );

#ifdef VAPOR_AUDIO_OPENAL
	ComponentPtr listener( new Listener() );
	entityCamera->addComponent( listener );
#endif

	return entityCamera;
}

//-----------------------------------//

void SceneDocument::onRender()
{
	RenderView* view = viewFrame->getView();

	if( !view->getCamera() )
		viewFrame->switchToDefaultCamera();

	const CameraPtr& camera = view->getCamera();
	if( !camera ) return;
	
	camera->setView( view );

	RenderBlock block;
	camera->cull( block, scene );
	camera->cull( block, editorScene );
	camera->render(block);

#if 0
#ifdef VAPOR_PHYSICS_BULLET
	PhysicsManager* physics = engine->getPhysicsManager();
	physics->drawDebugWorld();
#endif
#endif
}

//-----------------------------------//

void SceneDocument::onUpdate( float delta )
{
	GetEngine()->update( delta );
	
	if(editorScene)
		editorScene->update( delta );
	//eventManager->onSceneUpdate();
}

//-----------------------------------//

} } // end namespaces