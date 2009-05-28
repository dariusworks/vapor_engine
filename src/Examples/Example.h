/************************************************************************
*
* vaporEngine by triton � (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>
#include <vapor/math/Math.h>

using namespace vapor;

class Example : public Framework
{
	// Initializes the app.
	virtual void onInit();	

	// Sets up all the needed resources.
	virtual void onSetupResources();

	// Called to update stuff
	virtual void onUpdate();

	// Renders the scene
	virtual void onRender();
};