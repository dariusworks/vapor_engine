/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/BoundingBox.h"

FWD_DECL_INTRUSIVE(Renderable)
FWD_DECL_SHARED(Node)

namespace vapor {

//-----------------------------------//

// Builds debug geometry representing a bpunding box.
RenderablePtr VAPOR_API buildBoundingRenderable( const AABB& aabb );

// Builds debug geometry representing a ray.
NodePtr VAPOR_API buildRay( const Ray& pickRay, const Vector3& outFar );

//-----------------------------------//

} // end namespace