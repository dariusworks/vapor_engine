/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#include "Scene/Skydome.h"
#include "Scene/Camera.h"
#include "Scene/Entity.h"

#include "Math/Noise.h"
#include "Math/Helpers.h"

#include "Render/Device.h"
#include "Render/View.h"

#include "Resources/ResourceManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Skydome, Geometry)
	FIELD_PRIMITIVE_SETTER(Color, skyBaseColor, SkyColorBase)
	FIELD_PRIMITIVE_SETTER(Color, skyExtraColor, SkyColorExtra)
	FIELD_PRIMITIVE_SETTER(bool, useLinearGradient, SkyLinearGradient)
	FIELD_PRIMITIVE_SETTER(bool, showClouds, CloudsVisible)
	FIELD_PRIMITIVE(float, cloudsCover)
	FIELD_PRIMITIVE(float, cloudsSharpness)
	FIELD_PRIMITIVE(int32, cloudsNoiseSeed)
	FIELD_PRIMITIVE(bool, followCamera)
	//FIELD_CLASS_PTR(Image, clouds)
REFLECT_CLASS_END()

//-----------------------------------//

Skydome::Skydome()
	: currentTime(0)
	, followCamera(true)
	, skyBaseColor(Color::SkyBlue)
	, skyExtraColor(Color::Black)
	, needsRegeneration(true)
	, cloudsCover(0.95f)
	, cloudsSharpness(0.95f)
	, cloudsNoiseSeed(1343654258)
{

}

//-----------------------------------//

void Skydome::setSunEntity( const EntityPtr& sunEntity )
{
	sun = sunEntity->getTransform();
}

//-----------------------------------//

void Skydome::setSkyColorBase( const Color& color )
{
	skyBaseColor = color;
	needsRegeneration = true;
}

//-----------------------------------//

void Skydome::setSkyColorExtra( const Color&  color)
{
	skyExtraColor = color;
	needsRegeneration = true;
}

//-----------------------------------//

void Skydome::setSkyLinearGradient( bool isLinearGradient )
{
	useLinearGradient = isLinearGradient;
	needsRegeneration = true;
}

//-----------------------------------//

void Skydome::setCloudsVisible( bool clouds )
{
	showClouds = clouds;
}

//-----------------------------------//

static float FloatExpCurve(float v, float cover, float sharpness)
{
	float c = std::max<float>(v - (1 - cover), 0);
	return 1 - pow(1 - sharpness, c);
}

//-----------------------------------//

float FloatScale( float number, float minY, float maxY )
{
	const float rMin = 0.0f;
	const float rMax = 1.0f;

	number += abs(minY);
	return (number / ((maxY) / (rMax - rMin))) + rMin;
}

//-----------------------------------//

void Skydome::generateClouds()
{
	if( !clouds ) return;

	std::vector<byte> bn;
	bn.reserve( cloudsNoise.size()*4 );

	for( size_t i = 0; i < cloudsNoise.size(); i++ )
	{
		const float& n = cloudsNoise[i];
		bn.push_back( 0 );
		bn.push_back( 0 );
		bn.push_back( 0 );
		bn.push_back( FloatExpCurve(n, cloudsCover, cloudsSharpness)*255 );
	}

	Image* image = clouds.Resolve();
	image->setBuffer(bn);

	MaterialHandle materialHandle = dome->getMaterial();
	
	Material* material = materialHandle.Resolve();
	material->setTexture(0, clouds);
}

//-----------------------------------//

const float DEFAULT_SPHERE_RADIUS = 5000.0f;

void Skydome::generateDome()
{
	MaterialHandle materialHandle = MaterialCreate( AllocatorGetHeap(), "Sky");
	
	Material* material = materialHandle.Resolve();
	material->setProgram( "Sky" );

	dome = Allocate(Sphere, AllocatorGetHeap(), false, 2, DEFAULT_SPHERE_RADIUS);
	dome->setPolygonMode( PolygonMode::Solid );
	dome->setMaterial( materialHandle );
	
	addRenderable( dome );
}

//-----------------------------------//

void Skydome::generateBodies()
{
	static const uint16 CLOUDS_WIDTH = 256;
	static const uint16 CLOUDS_HEIGHT = 256;

	cloudsNoise.clear();

	// Generate clouds noise.
	Noise noise(cloudsNoiseSeed);
	noise.generate(cloudsNoise, CLOUDS_WIDTH, CLOUDS_HEIGHT);

	clouds = ImageCreate(AllocatorGetHeap(), CLOUDS_WIDTH, CLOUDS_HEIGHT, PixelFormat::R8G8B8A8);
	generateClouds();
}

//-----------------------------------//

void Skydome::generateColors()
{
	VertexBufferPtr vb = dome->getVertexBuffer();
	const std::vector<Vector3>& vertices = vb->getVertices();

	std::vector<Vector3> colors;

	if(!useLinearGradient)
	{
		colors.resize( vertices.size(), skyBaseColor );
	}
	else
	{
		// Search for the max and min Y coordinate.
		float minY = LimitsFloatMaximum;
		float maxY = LimitsFloatMinimum;

		for( size_t i = 0; i < vertices.size(); i++ )
		{
			const Vector3& vec = vertices[i];

			minY = std::min(vec.y, minY);
			maxY = std::max(vec.y, maxY);

			//if( vec.y > maxY )
			//	maxY = vec.y;
			//else if( vec.y < minY )
			//	minY = vec.y;
		}

		maxY += abs(minY);
		//minY += abs(minY);

		for( size_t i = 0; i < vertices.size(); i++ )
		{
			const Vector3& vec = vertices[i];

			const Color& color = getSkyVertexColor( vec, minY, maxY );
			colors.push_back( color );
		}	
	}

	vb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

Color Skydome::getSkyVertexColor( const Vector3& v, float minY, float maxY )
{
	float s = FloatScale( v.y, minY, maxY );
	float t = (s <= 0.5f) ? 0.0f : s - 0.5f;

	// Linear interpolate to get the color at the vertex.
	Vector3 newColor = ((Vector3) skyBaseColor).lerp(skyExtraColor, t);

	return Color( newColor.x, newColor.y, newColor.z );
}

//-----------------------------------//

void Skydome::update( float delta )
{
	if( renderables.empty() )
	{
		generateDome();
		generateBodies();
	}

	if(needsRegeneration)
	{
		generateColors();
		needsRegeneration = false;
	}

	// Updates the current game time.
	#pragma TODO("Create a game time subsystem")

	currentTime += delta;
	
	if(!dome) return;
		
#if 0
	MaterialPtr material = dome->getMaterial();
	ProgramPtr program = material->getProgram();

	if(program)
	{
		program->bind();
		program->setUniform("vp_TexScroll", float(currentTime / 100.0f));
	}
#endif

	//if( followCamera )
	//{
	//	CameraPtr camera = getCurrentCamera();

	//	// Gets the current camera position.
	//	TransformPtr transCamera = camera->getEntity()->getTransform();
	//	Vector3 cameraPosition = transCamera->getPosition();

	//	// Sets the sky position to the camera position.
	//	EntityPtr nodeSky = getEntity();
	//	TransformPtr transSky = nodeSky->getTransform();
	//	transSky->setPosition( cameraPosition );
	//}

}

//-----------------------------------//

Vector3 Skydome::getSunPosition()
{
	// We'll use spherical coordinates to get the new position for the
	// sun. These are pretty simple to use and adapt pretty well to 
	// what we want to achieve here.

	float rho = DEFAULT_SPHERE_RADIUS * 0.95f;
	float phi = PI * currentTime * 0.01f;
	float theta = 0;

	// We want 12 hours of virtual time to correspond to the day cycle,
	// so the sun should start at half-sphere when time reaches a given
	// sunrise time (say 6am), and go on until the sunset time.

	float x = rho*sin(phi)*cos(theta);
	float y = rho*sin(phi)*sin(theta);
	float z = rho*cos(phi);

	return Vector3(x, y, z);
}

//-----------------------------------//

NAMESPACE_ENGINE_END