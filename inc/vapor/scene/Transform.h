/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/scene/Node.h"

#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Vector3.h"
#include "vapor/math/EulerAngles.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

class VAPOR_API Transformable : public Node
{
public:

	// Translate this node by the given parameters.
	void translate( float x, float y, float z );
	void translate( const math::Vector3& tr );

	// Gets the position of the node.
	const math::Vector3& getPosition() const;

	// Sets the position of the node.
	void setPosition( const math::Vector3 position );

	// Scale this node by the given parameters.
	void scale( float x, float y, float z );
	void scale( float uniform );
	void scale( const math::Vector3& scale );

	// Rotates this node by the given parameters.
	void rotate( float xang, float yang, float zang );
	void rotate( const math::Vector3& rot );
	
	// Resets the transformations previously applied to this node.
	void reset();

	// Gets the absolute transformation matrix.
	const math::Matrix4x3& getAbsoluteTransform() const;
	
	// Sets the absolute transformation matrix.
	void setAbsoluteTransform( const math::Matrix4x3& matrix );

	// Gets the local transformation matrix.
	math::Matrix4x3 getLocalTransform() const;

protected:

	Transformable();
	virtual ~Transformable();

	math::EulerAngles angles;
	math::Vector3 v_translate;
	math::Vector3 v_scale;

	math::Matrix4x3 transform;
	math::Matrix4x3 absoluteLocalToWorld;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Transformable );

//-----------------------------------//

} } // end namespaces