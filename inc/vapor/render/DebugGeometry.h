/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/BoundingBox.h"
#include "vapor/math/Frustum.h"

FWD_DECL_SHARED(Node)
FWD_DECL_INTRUSIVE(Renderable)

namespace vapor {

//-----------------------------------//

// Builds debug geometry representing a bounding box.
RenderablePtr VAPOR_API buildBoundingRenderable( const BoundingBox& box );

// Builds debug geometry representing a ray.
NodePtr VAPOR_API buildRay( const Ray& pickRay, const Vector3& outFar );

// Builds debug geometry representing a bounding box.
RenderablePtr VAPOR_API buildFrustum( const Frustum& box );

//-----------------------------------//

} // end namespace