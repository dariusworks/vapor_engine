/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_STB

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/Image.h"

namespace vapor {

//-----------------------------------//

/**
 * This codec provides image decoding services using stb_image.
 */

class STB_Image_Loader : public ResourceLoader
{
public:

	STB_Image_Loader();

	// Creates the resource with no data.
	IMPLEMENT_PREPARE(Image)

	// Decode an image file to a buffer.
	virtual bool decode(const File&, Resource* res);

	// Gets the name of this codec.
	IMPLEMENT_GETTER(Name, const std::string, "STB_IMAGE")

	// Gets the list of extensions this codec can handle.
	IMPLEMENT_GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)

protected:

	// Holds all file extensions recognized by this codec.
	mutable ExtensionList extensions;
};

//-----------------------------------//

} // end namespace

#endif