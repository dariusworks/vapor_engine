/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Texture.h"
#include "Resources/Image.h"

namespace vapor {

//-----------------------------------//

class ResourceManager;
struct ResourceEvent;

/**
 * Manages a set of textures. The rest of the engine, when needing a 
 * texture will ask the texture manager for the texture for a given image.
 * This way the engine will be able to provide, for example, fallback 
 * texture when it is not possible to load the requested image file.
 * This will be pretty useful when resource auto-loading is added, if a 
 * given file is corrupted we can safely render the scene, falling back
 * for default textures.
 */

class VAPOR_API TextureManager
{
public:

	TextureManager();
	~TextureManager();

	// Gets a texture given a name identifier.
	TexturePtr getTexture( const std::string& tex );

	// Gets a texture given an image.
	TexturePtr getTexture( const ImagePtr& img );

	// Removes the texture backed by the image.
	void removeTexture(const ImagePtr& image);

	// Gets the memory usage of the textures loaded (in kbytes).
	uint getMemoryUsage();

	// Updates the texture manager.
	virtual void update( float delta );

protected:

	// Populates a texture when the image is loaded.
	void onLoaded( const ResourceEvent& evt );

	// Removes a texture when the image file is unloaded.
	void onUnloaded( const ResourceEvent& evt );

	// Reloads a texture when the image file changes.
	void onReloaded( const ResourceEvent& evt );

	// Switches the image of the texture backed by the first image.
	void switchImage( const ImagePtr&, const ImagePtr& );

	// Maps the identifiers to the textures.
	typedef std::map< Image*, TexturePtr > TextureMap;
	typedef std::pair< Image*, TexturePtr > TextureMapPair;
	TextureMap textures;
	
	ResourceManager* rm;
};

//-----------------------------------//

} // end namespace