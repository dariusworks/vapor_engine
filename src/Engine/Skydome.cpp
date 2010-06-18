/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Skydome.h"
#include "vapor/scene/Node.h"
#include "vapor/render/Quad.h"
#include "vapor/math/Math.h"

namespace vapor {

//-----------------------------------//

const std::string& Skydome::type = "Skydome";

//-----------------------------------//

Skydome::Skydome( /*const MaterialPtr& mat*/ )
	: currentTime( 0.0 )
{
	generateDome();
	generateBodies();
}

//-----------------------------------//

const float DEFAULT_SPHERE_RADIUS = 1000.0f;

void Skydome::generateDome()
{
	MaterialPtr mat( new Material( "SkydomeSimple" ) );
	//mat->setDepthWrite( false );
	// TODO:
	mat->setProgram( "diffuse" );

	dome = new Sphere( false, 2, DEFAULT_SPHERE_RADIUS );
	dome->setMaterial( mat );
	dome->setPolygonMode( PolygonMode::Solid );

	setSkyLinearGradient( Color::SkyBlue, Color::Black );
	//setSkyLinearGradient( Color( 50, 90, 0 ), Color::Black );
	//setSkyLinearGradient( Color( 65, 75, 75 ), Color::Black );
	//setSkyLinearGradient( Color( 30, 40, 60 ), Color( 65, 75, 75 ) );

	addRenderable( dome );
}

//-----------------------------------//

void Skydome::generateBodies()
{
	//sun = new Quad( 100.0f, 100.0f );
	//MaterialPtr matSun( new Material( "SunBlend" ) );
	//matSun->setProgram( "tex" );
	//matSun->setTexture( 0, "sun.png" );
	//matSun->setBlending( BlendingSource::SourceAlpha,
	//	BlendingDestination::OneMinusSourceAlpha );
	//matSun->setBackfaceCulling( false );
	//sun->setMaterial( matSun );
	//addRenderable( sun );

	//MaterialPtr mat( new Material( "MoonBlend" ) );
	//mat->setProgram( "tex" );
	//mat->setTexture( 0, "moon.png" );
	//mat->setBlending( BlendingSource::SourceAlpha, BlendingDestination::OneMinusSourceAlpha );
	//mat->setBackfaceCulling( false );

	//RenderablePtr moon( new Quad(25.0f, 25.0f) );
	//moon->setMaterial( mat );
	//addRenderable( moon );
}

//-----------------------------------//

const ushort TIME_SCALE = 10u;

void Skydome::update( double delta )
{
	currentTime += delta;

	// We'll update the sun position each hour.
	if( currentTime > 1*TIME_SCALE )
	{
		if( sun )
			sun->setPosition( getSunPosition() );
	}

	// A full day is 24 hours, so we need to reset the time.
	if( currentTime >= 24*TIME_SCALE )
	{
		currentTime = 0;
	}
}

//-----------------------------------//

Vector3 Skydome::getSunPosition()
{
	// We'll use spherical coordinates to get the new position for the
	// sun. These are pretty simple to use and adapt pretty well to 
	// what we want to achieve here.

	float rho = DEFAULT_SPHERE_RADIUS*0.95f;
	float phi = float(PI*currentTime*0.01f);
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

void Skydome::setSunNode( const NodePtr& sunNode )
{
	sun = sunNode->getTransform();
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
	colorBottom = Vector3( c1.r, c1.g, c1.b );
	colorTop = Vector3( c2.r, c2.g, c2.b );

	VertexBufferPtr vb = dome->getVertexBuffer();
	const std::vector<Vector3>& vertices = vb->getVertices();

	// Search for the max and min Y coordinate.
	yMin = std::numeric_limits<float>::max();
	yMax = std::numeric_limits<float>::min();

	foreach( const Vector3& vec, vertices )
	{
		if( vec.y > yMax ) 
			yMax = vec.y;
		else if( vec.y < yMin ) 
			yMin = vec.y;
	}

	yMax += abs(yMin);
	//yMin += abs(yMin);

	std::vector<Vector3> colors;
	foreach( const Vector3& vec, vertices )
	{
		const Color& c = getSkyVertexColor( vec );
		colors.push_back( Vector3( c.r, c.g, c.b ) );
	}	

	vb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

Color Skydome::getSkyVertexColor( const Vector3& v )
{
	float s = scale( v.y );
	float t = (s <= 0.5f) ? 0.0f : s - 0.5f;

	// Linear interpolate to get the color at the vertex.
	Vector3 newColor = colorBottom + (colorTop - colorBottom) * t;

	return Color( newColor.x, newColor.y, newColor.z );
}

//-----------------------------------//

} // end namespace
