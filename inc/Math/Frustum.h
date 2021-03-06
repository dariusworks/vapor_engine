/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Math/Vector.h"
#include "Math/Matrix4x4.h"
#include "Math/Plane.h"
#include "Math/BoundingBox.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

/**
 * Projection types.
 */

enum_class_begin(FrustumProjection)
	Orthographic,
	Perspective
enum_class_end

REFLECT_DECLARE_ENUM(Projection);

//-----------------------------------//

/**
 * Frustum is a shape similiar to a pyramid with the top truncated.
 */

REFLECT_DECLARE_CLASS(Frustum)

struct API_CORE Frustum
{
	//REFLECT_DECLARE_OBJECT(Frustum)

	Frustum();
	Frustum( const Frustum& rhs );

	// Updates the projection matrix.
	void updateProjection();

	// Updates the frustum planes.
	void updatePlanes( const Matrix4x3& matView );

	// Updates the frustum corners.
	void updateCorners( const Matrix4x3& matView );

	// Checks if the box is inside the frustum.
	bool intersects( const BoundingBox& box ) const;

public:

	// Projection type.
	enum_class(FrustumProjection) projection;

	// Projection matrix.
	Matrix4x4 matProjection;

	// Field of view.
	float fieldOfView;

	// Near clipping plane.
	float nearPlane;

	// Far clipping plane.
	float farPlane;

	// Aspect ratio of the frustum.
	float aspectRatio;

	// Ortographic view size.
	Vector3 orthoSize;

	// Stores the planes that make up the frustum.
	// Order: Left, Right, Top, Bottom, Near, Far.
	Plane planes[6];

	// Frustum corner points.
	Vector3 corners[8];
};

//-----------------------------------//

NAMESPACE_EXTERN_END