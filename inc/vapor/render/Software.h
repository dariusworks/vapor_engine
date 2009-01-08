/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* Version 2, December 2004
* 
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
* 
* 0. You just DO WHAT THE FUCK YOU WANT TO.
*
************************************************************************/

#pragma once

#include "vapor/RenderDevice.h"

namespace vapor {
	namespace render {

class SoftwareDevice : public Device
{
	
/////////////////////////////////////////////////////////////////////////////
// Geometry stage
/////////////////////////////////////////////////////////////////////////////
	
	// Transform coordinates from Model space to World space
	void transformModelWorld ();

	// Transform coordinates from World space to Eye space
	void transformWorldCamera ();

	// Normalize the normal vectors
	void normalizeNormals ();

	// Compute Vertex lighting
	void computeLighting ();

	// Transform Texture coordinates
	void transformTexture ();

	// Transform from Camera space to Clip space (Projective transform)
	void transformCameraClip ();

	// Clip primitives outside Camera's view
	void clipPrimitives ();

	// Run Geometry stage
	void runGeometryStage ();

/////////////////////////////////////////////////////////////////////////////
// Rasterization stage
/////////////////////////////////////////////////////////////////////////////
	
	// Transform from Clip space to Viewport space
	void transformClipViewport ();

	// Sampling to fragments
	void sampleFragments ();

	// Run Rasterization stage
	void runRasterizationStage ();
};

} } // end namespaces