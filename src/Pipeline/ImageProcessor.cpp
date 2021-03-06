/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Pipeline/ImageProcessor.h"
#include "Core/Log.h"

#if 0
#include <nvtt/nvtt.h>
using namespace nvtt;
#endif

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(MipmapFilter)
	ENUM(Box)
	ENUM(Triangle)
	ENUM(Kaizer)
REFLECT_ENUM_END()

REFLECT_ENUM(CompressionQuality)
	ENUM(Fastest)
	ENUM(Normal)
	ENUM(Production)
	ENUM(Highest)
REFLECT_ENUM_END()

REFLECT_ENUM(CompressionFormat)
	ENUM(RGB)
	ENUM(RGBA)
	ENUM(DXT1)
	ENUM(DXT1a)
	ENUM(DXT3)
	ENUM(DXT5)
	ENUM(DXT5nm)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(ImageProcessor, ResourceProcessor)
	FIELD_ENUM(0, CompressionFormat, format)
	FIELD_ENUM(1, CompressionQuality, quality)
	FIELD_PRIMITIVE(2, bool, generateMipmaps)
	FIELD_ENUM(3, MipmapFilter, mipmapFilter)
REFLECT_CLASS_END()

//-----------------------------------//

static bool g_InitializedNVTT = false;

ImageProcessor::ImageProcessor()
	: format(CompressionFormat::DXT5)
	, quality(CompressionQuality::Normal)
	, generateMipmaps(true)
	, mipmapFilter(MipmapFilter::Box)
{
#if 0
	if( !g_InitializedNVTT )
	{
		LogInfo("Using NVIDIA Texture Tools %u", nvtt::version());
		g_InitializedNVTT = true;
	}
#endif
}

//-----------------------------------//

ImageProcessor::~ImageProcessor()
{
}

//-----------------------------------//

ExtensionMetadata* ImageProcessor::getMetadata()
{
	static ExtensionMetadata s_ExtensionMetadata =
	{
		"Image",
		"Processes an image resource."
	};

	return &s_ExtensionMetadata;
}

//-----------------------------------//

struct ImageOutputHandler : public nvtt::OutputHandler
{
	void beginImage(int size, int width, int height, int depth, int face, int miplevel);
	bool writeData(const void * data, int size);
};

bool ImageProcessor::Process(const ResourcePtr& resource)
{
	if( resource->getResourceGroup() != ResourceGroup::Images )
		return false;

	const ImagePtr& image = RefCast<Image>(resource);

#if 0
	InputOptions input;
	input.setMipmapGeneration(generateMipmaps);
	input.setMipmapFilter((nvtt::MipmapFilter) mipmapFilter);

	CompressionOptions compression;
	compression.setQuality((nvtt::Quality) quality);
	compression.setFormat((nvtt::Format) format);

	ImageOutputHandler handler;

	OutputOptions output;
	output.setOutputHandler(&handler);

	Compressor compressor;
	
	if( !compressor.process(input, compression, output) )
		return false;
#endif
	return true;
}

//-----------------------------------//

void ImageOutputHandler::beginImage(int size, int width, int height, int depth, int face, int miplevel)
{

}

//-----------------------------------//

bool ImageOutputHandler::writeData(const void * data, int size)
{
	return false;
}

//-----------------------------------//

NAMESPACE_PIPELINE_END