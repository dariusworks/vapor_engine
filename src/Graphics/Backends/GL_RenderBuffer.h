/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Graphics/RenderBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Render buffers are also known as FBOs (frame buffers objects) in OpenGL.
 */

class API_RENDER GL_RenderBuffer : public RenderBuffer
{
public:

	GL_RenderBuffer(const Settings&);
	virtual ~GL_RenderBuffer();

	// Binds/unbinds the FBO.
	void bind() OVERRIDE;
	void unbind() OVERRIDE;

	// Checks if the FBO is complete.
	bool check() OVERRIDE;

	// Reads the contents of the render buffer.
	void read(int8 attachment, std::vector<uint8>& data) OVERRIDE;

	// Updates the render target (usually swaps buffers).
	void update() OVERRIDE;

	// Creates a new render buffer with the given components.
	void createRenderBuffer( int bufferComponents = RenderBufferType::Color | RenderBufferType::Depth ) OVERRIDE;

	// Creates a render texture to this FBO.
	TexturePtr createRenderTexture( RenderBufferType::Enum = RenderBufferType::Color ) OVERRIDE;

	// Attaches a render texture to this FBO.
	void attachRenderTexture(const TexturePtr& tex) OVERRIDE;

	// Gets the settings of this render target.
	GETTER(Settings, const Settings&, settings)

protected:

	void createRenderBufferStorage(int buffer, int type, int attachment);
	void setBufferState();
	bool checkSize();

	uint32 id;
	std::vector< uint32 > renderBuffers;
	std::vector< TexturePtr > textureBuffers;

	bool bound;
	bool valid;
	bool colorAttach;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END