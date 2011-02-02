/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Engine.h"
#include "vapor/scene/Skydome.h"
#include "vapor/scene/Camera.h"
#include "vapor/scene/Entity.h"
#include "vapor/render/Device.h"
#include "vapor/render/View.h"
#include "vapor/math/Noise.h"
#include "vapor/math/Helpers.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Skydome, Geometry)
	//FIELD_PRIMITIVE(Skydome, float, currentTime)
	FIELD_PRIMITIVE(Color, skyColorTop)
	FIELD_PRIMITIVE(Color, skyColorBottom)
	FIELD_PRIMITIVE(bool, showClouds)
	FIELD_PRIMITIVE(float, cloudsCover)
	FIELD_PRIMITIVE(float, cloudsSharpness)
	FIELD_PRIMITIVE(int, cloudsNoiseSeed)
	FIELD_PRIMITIVE(bool, followCamera)
	FIELD_CLASS_PTR(Image, clouds)
END_CLASS()

//-----------------------------------//

Skydome::Skydome()
	: currentTime(0)
	, followCamera(true)
	, skyColorTop(Color::Black)
	, skyColorBottom(Color::SkyBlue)
	, cloudsCover(0.95f)
	, cloudsSharpness(0.95f)
	, cloudsNoiseSeed(1343654258)
{
	generateDome();
	generateBodies();
}

//-----------------------------------//

Skydome::~Skydome()
{ }

//-----------------------------------//

void Skydome::onFieldChanged(const Field& field)
{
	setSkyLinearGradient(skyColorTop, skyColorBottom);
	generateBodies();
}

//-----------------------------------//

float Skydome::cloudsExpCurve(float v)
{
	float c = std::max<float>(v - (1 - cloudsCover), 0);
	return 1 - pow(1 - cloudsSharpness, c);
}

//-----------------------------------//

void Skydome::updateClouds()
{
	if( !clouds )
		return;

	std::vector<byte> bn;
	bn.reserve( cloudsNoise.size()*4 );

	for( uint i = 0; i < cloudsNoise.size(); i++ )
	{
		const float& n = cloudsNoise[i];
		bn.push_back( 0 );
		bn.push_back( 0 );
		bn.push_back( 0 );
		bn.push_back( cloudsExpCurve(n)*255 );
	}

	clouds->setBuffer(bn);
	//clouds->save( "clouds.png" );

	MaterialPtr material = dome->getMaterial();
	TexturePtr tex = material->getTexture(0);

	if( tex )
		tex->setImage(clouds.get());
}

//-----------------------------------//

const float DEFAULT_SPHERE_RADIUS = 5000.0f;

void Skydome::generateDome()
{
	MaterialPtr mat( new Material("Sky") );
	mat->setProgram( "Sky" );

	dome = new Sphere( false, 2, DEFAULT_SPHERE_RADIUS );
	dome->setPolygonMode( PolygonMode::Solid );
	dome->setMaterial( mat );
	
	setSkyLinearGradient(skyColorTop, skyColorBottom);

	addRenderable( dome );
}

//-----------------------------------//

static const int CLOUDS_WIDTH = 256;
static const int CLOUDS_HEIGHT = 256;

void Skydome::generateBodies()
{
	cloudsNoise.clear();

	// Clouds.
	Noise noise(cloudsNoiseSeed);
	noise.generate(cloudsNoise, CLOUDS_WIDTH, CLOUDS_HEIGHT);

	clouds = new Image(CLOUDS_WIDTH, CLOUDS_HEIGHT, PixelFormat::R8G8B8A8);
	updateClouds();
}

//-----------------------------------//

//CameraPtr getCurrentCamera()
//{
//	Engine* engine = GetEngine();
//	RenderDevice* device = engine->getRenderDevice();
//
//	if( !device )
//		return CameraPtr();
//
//	RenderTarget* target = device->getRenderTarget();
//
//	if( !target )
//		return CameraPtr();
//
//	const ViewportList& viewports = target->getViews();
//	RenderView* view = viewports.front(); 
//	return view->getCamera();
//	return CameraPtr();
//}

//-----------------------------------//

void Skydome::update( double delta )
{
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

	// Updates the current game time.
	#pragma TODO("Create a game time subsystem")

	currentTime += delta;

	if(dome)
	{
		MaterialPtr material = dome->getMaterial();
		ProgramPtr program = material->getProgram();

		if(program)
		{
			program->bind();
			program->setUniform("vp_TexScroll", float(currentTime / 100.0f));
		}

		if( showClouds )
			material->setTexture(0, clouds);
		else
			material->setTexture(0, "alpha_null.png");
	}
}

//-----------------------------------//

Vector3 Skydome::getSunPosition()
{
	// We'll use spherical coordinates to get the new position for the
	// sun. These are pretty simple to use and adapt pretty well to 
	// what we want to achieve here.

	float rho = DEFAULT_SPHERE_RADIUS*0.95f;
	float phi = Math::PI*currentTime*0.01f;
	float theta = 0;

	// We want 12 hours of virtual time to correspond to the day cycle,
	// so the sun should start at half-sphere when time reaches a given
	// sunrise time (say 6am), and go on until the sunset time.

	float x = rho*sin(phi)*cos(theta);
	float y = rho*sin(phi)*sin(theta);
	float z = rho*cos(phi);

	return Vector3( x, y, z );
}

//-----------------------------------//

void Skydome::setSunEntity( const EntityPtr& sunEntity )
{
	sun = sunEntity->getTransform();
}

//-----------------------------------//

void Skydome::setSkyColor( const Color& color )
{
	VertexBufferPtr vb = dome->getVertexBuffer();
	uint numVertices = vb->getNumVertices();

	std::vector<Vector3> colors;
	colors.resize( numVertices, Vector3( color.r, color.g, color.b ) );

	vb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

float Skydome::scale( float number )
{
	const float rMin = 0.0f;
	const float rMax = 1.0f;

	number += abs(yMin);
	return (number / ((yMax) / (rMax - rMin))) + rMin;
}

//-----------------------------------//

void Skydome::setSkyLinearGradient( const Color& c1, const Color& c2 )
{
	VertexBufferPtr vb = dome->getVertexBuffer();
	const std::vector<Vector3>& vertices = vb->getVertices();

	// Search for the max and min Y coordinate.
	yMin = Limits::FloatMaximum;
	yMax = Limits::FloatMinimum;

	for( uint i = 0; i < vertices.size(); i++ )
	{
		const Vector3& vec = vertices[i];

		if( vec.y > yMax )
			yMax = vec.y;
		else if( vec.y < yMin )
			yMin = vec.y;
	}

	yMax += abs(yMin);
	//yMin += abs(yMin);

	std::vector<Vector3> colors;

	for( uint i = 0; i < vertices.size(); i++ )
	{
		const Vector3& vec = vertices[i];

		const Color& color = getSkyVertexColor( vec );
		colors.push_back( color );
	}	

	vb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

Color Skydome::getSkyVertexColor( const Vector3& v )
{
	float s = scale( v.y );
	float t = (s <= 0.5f) ? 0.0f : s - 0.5f;

	// Linear interpolate to get the color at the vertex.
	Vector3 newColor = ((Vector3) skyColorBottom).lerp(skyColorTop, t);

	return Color( newColor.x, newColor.y, newColor.z );
}

//-----------------------------------//

} // end namespace
