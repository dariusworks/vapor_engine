
/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"
#include "Input/InputManager.h"
#include "Input/Mouse.h" 
#include "Input/Keyboard.h"

FWD_DECL_INTRUSIVE(Resource)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;
class Document;

class ToolExtension;

/**
 * Plugins can subscribe to these events and get notified when things
 * happen in the editor. For example, the select tool needs to know
 * when an entity is selected to show its bounding box.
 */

typedef std::vector<EditorPlugin*> EventListeners;

class API_EDITOR EventManager : public wxEvtHandler
{
public:

	EventManager();
	virtual ~EventManager();

	void disconnectPluginListeners();

	// Add/remove a plugin as an event listener.
	void addEventListener( EditorPlugin* plugin );
	void removeEventListener( EditorPlugin* plugin );

	// Document events.
	void onDocumentCreate( Document& document );
	void onDocumentDestroy( Document& document );
	void onDocumentSelect( Document& document );
	void onDocumentUnselect( Document& document );

	// Entity selection event.
	void onEntitySelect( const EntityPtr& );
	void onEntityUnselect( const EntityPtr& );

	// Component selection event.
	void onComponentSelect( const ComponentPtr& );
	void onComponentUnselect( const ComponentPtr& );

	// Resource selection event.
	void onResourceSelect( const ResourcePtr& );
	void onResourceUnselect( const ResourcePtr& );

	// Mouse input events.
	void onMouseMove( const MouseMoveEvent& );
	void onMouseDrag( const MouseDragEvent& );
	void onMousePress( const MouseButtonEvent& );
	void onMouseRelease( const MouseButtonEvent& );
	void onMouseEnter();
	void onMouseLeave();
	
	// Keyboard input events.
	void onKeyPress( const KeyEvent& );
	void onKeyRelease( const KeyEvent& );

	// Scene load event.
	void onSceneLoad( const ScenePtr& scene );
	void onSceneUnload( const ScenePtr& scene );
	void onSceneUpdate();

	// Server callbacks.
	void onServerConnect(const SessionPtr&);
	void onServerDisconnect(const SessionPtr&);

	// Undo/redo operation events.
	void onUndoOperation(const UndoOperationPtr&);
	void onRedoOperation(const UndoOperationPtr&);

	// Gets the current tool.
	ACESSOR(CurrentTool, ToolExtension*, currentTool)

	// Gets/sets the current plugin.
	ACESSOR(CurrentPlugin, EditorPlugin*, currentPlugin)

	// Gets the current tool id.
	int getCurrentToolId();

	// Switches the current plugin.
	void setTool(ToolExtension* tool);

	// Unsets the current tool.
	void unsetCurrentTool();

	// Toggles the current tool. Pass null to restore original tool.
	void toggleTool(ToolExtension* tool);

protected:

	// Handles plugin events.
	void onPluginEnableEvent(Plugin* plugin);
	void onPluginDisableEvent(Plugin* plugin);

	// Handles toolbar button clicks.
	void onToolbarButtonClick(wxCommandEvent& event);

	// Registers to the input events.
	void registerInputCallbacks();

	// Pre-handling of events from the toolbar.
	bool TryBefore( wxEvent& event ) OVERRIDE;

	// Current tool id.
	ToolExtension* currentTool;

	// Current plugin.
	EditorPlugin* currentPlugin;

	// Keeps the original tool while current is toggled.
	ToolExtension* originalTool;

	// Global event listener plugins.
	EventListeners eventListeners;
};

//-----------------------------------//

NAMESPACE_EDITOR_END
