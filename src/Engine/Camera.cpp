/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Camera.h"
#include "vapor/scene/Group.h"
#include "vapor/scene/Geometry.h"

using namespace vapor::math;
using namespace vapor::render;

namespace vapor {
	namespace scene {

//-----------------------------------//

const std::string& Camera::type = "Camera";

//-----------------------------------//

Camera::Camera( render::Device* device, Projection::Enum projection )
	: renderDevice( device ), projection( projection ),
		target( nullptr ), fov(45.0f), near_( 5.0f ), far_( 1000.0f ),
		lookAtVector( Vector3( 0.0f, 0.0f, 0.0f ) ), 
		forwardVector( Vector3::UnitZ ), upVector( Vector3::UnitY )
{
	setRenderTarget( device->getRenderTarget() );
}

//-----------------------------------//

void Camera::setProjection( Projection::Enum projection )
{
	this->projection = projection;
}

//-----------------------------------//

const math::Matrix4x4& Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

//-----------------------------------//

const math::Matrix4x3& Camera::getViewMatrix() const
{
	return viewMatrix;
}

//-----------------------------------//

void Camera::setFOV( float fov )
{
	this->fov = fov;
}

//-----------------------------------//

void Camera::setNear( float near_ )
{
	this->near_ = near_;
}

//-----------------------------------//

void Camera::setFar( float far_ )
{
	this->far_ = far_;
}

//-----------------------------------//

void Camera::setRenderTarget( RenderTarget* newTarget )
{
	if( !newTarget ) return;
	
	// TODO: remove only this from the delegate
	//target->onTargetResize.clear();

	target = newTarget;
	target->onTargetResize += fd::bind( &Camera::handleTargetResize, this );

	handleTargetResize( target->getSettings() );
}

//-----------------------------------//

void Camera::handleTargetResize( const render::Settings& evt )
{
	width = evt.getWidth();
	height = evt.getHeight();

	glViewport( 0, 0, width, height );
}

//-----------------------------------//

void Camera::update( float /*delta*/ )
{
	setupProjection();
	setupView();
}

//-----------------------------------//

void Camera::setupProjection()
{
	if( projection == Projection::Perspective )
	{
		projectionMatrix = Matrix4x4::createPerspectiveProjection( 
			fov, getAspectRatio(), near_, far_ );
	}
	else
	{
		projectionMatrix = Matrix4x4::createOrthographicProjection( 
			0.0, width,
			0.0, height,
			near_, far_ );
	}

	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	//glOrtho( -target->getSettings().getWidth()/16, target->getSettings().getWidth()/16, 
	//	-target->getSettings().getHeight()/16, target->getSettings().getHeight()/16, 1.0f, 100.0f );
	////glOrtho( 0.0, target->getSettings().getWidth(), 0.0, target->getSettings().getHeight(), near_, far_ );

	//GLfloat test[16];
	//glGetFloatv( GL_PROJECTION_MATRIX, &test[0] );
}

//-----------------------------------//

void Camera::setupView()
{

}

//-----------------------------------//

float Camera::getAspectRatio() const
{
	if( height == 0) return 0.0f;
	return static_cast<float>(width) / height ;
}

//-----------------------------------//

//const math::Frustum& Camera::getFrustum( ) const
//{
//	return new math::Frustum( );
//}

//-----------------------------------//

void Camera::render( NodePtr node ) const
{
	// This will contain all nodes used for rendering.
	render::RenderQueue renderQueue;

	cull( renderQueue, node );

	renderDevice->setRenderTarget( target );
	renderDevice->render( renderQueue, this );
}

//-----------------------------------//

void Camera::render( ) const
{
	NodePtr parent = getNode()->getParent();

	if( !parent ) return;

	// Search for the root node.
	while ( parent->getParent() )
	{
		parent = parent->getParent();
	}
	  
	render( parent );
}

//-----------------------------------//

void Camera::cull( render::RenderQueue& queue, NodePtr node ) const
{
	// Let's forget culling for now. Return all renderable nodes.
	// TODO: Check if dynamic_cast is faster than a string comparison.
	
	// Try to see if this is a Group-derived node.
	GroupPtr group( std::dynamic_pointer_cast< Group >( node ) );
	
	// Yes it is.
	if( group )
	{
		// Cull the children nodes recursively.
		foreach( NodePtr child, group->getChildren() )
		{
			cull( queue, child );
		}
	}

	// If it is a renderable object, then we perform frustum culling
	// and if the node is visible, then we push it to a list of things
	// to render that will be later passed to the rendering device.
	const std::vector< GeometryPtr >& geometries = node->getGeometry();

	foreach( GeometryPtr geometry, geometries ) 
	{
		// No frustum culling is performed yet.
		geometry->appendRenderables( queue );
	}
}

//-----------------------------------//

const std::string Camera::save( int /*indent*/ )
{
	return "";
}

//-----------------------------------//

const std::string& Camera::getType() const
{
	return type;
}

//-----------------------------------//

} } // end namespaces