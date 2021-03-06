/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/API.h"
#include "Resources/Resource.h"
#include "Math/BoundingBox.h"
#include "Math/Vector.h"
#include "Geometry/GeometryBuffer.h"

FWD_DECL_INTRUSIVE(Animation)
FWD_DECL_INTRUSIVE(Skeleton)
FWD_DECL_INTRUSIVE(GeometryBuffer)

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

struct MeshMaterial
{
	String name;
	String texture;
	bool alpha;
};

struct MeshGroup
{
	std::vector<uint16> indices;
	MeshMaterial material;
};

//-----------------------------------//

/**
 * Mesh resources contain 3D geometry data.
 */

REFLECT_DECLARE_CLASS(Mesh)

class API_RESOURCE Mesh : public Resource
{
	REFLECT_DECLARE_OBJECT(Mesh)

public:

	Mesh();
	~Mesh();

	// Gets if the mesh is built.
	bool isBuilt() const;

	// Gets if the mesh is animated.
	bool isAnimated() const;

	// Gets the skeleton of the mesh.
	GETTER(Skeleton, SkeletonPtr, skeleton)

	// Gets the bind pose animation of the mesh.
	GETTER(BindPose, AnimationPtr, bindPose)

	// Finds an animation from the mesh.
	Animation* findAnimation( const String& name );

	// Gets the bounding box of the mesh.
	GETTER(BoundingVolume, const BoundingBox&, boundingVolume)

	// Gets/sets the mesh geometry buffer.
	ACESSOR(GeometryBuffer, GeometryBufferPtr, geometryBuffer)

	// Gets the animations of the mesh.
	GETTER(Animations, const std::vector<AnimationPtr>&, animations)

	// Gets the resource group of this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

public:

	// Sets up the initial vertices in joint-space.
	void setupInitialVertices();

	// Builds the mesh bounding volume.
	void buildBounds();

	// Keeps track if the mesh is animated.
	bool animated;

	// Skeleton of the mesh.
	SkeletonPtr skeleton;

	// Bind pose animation of the mesh.
	AnimationPtr bindPose;

	// Animations of the mesh.
	std::vector<AnimationPtr> animations;

	// Groups of the mesh.
	std::vector<MeshGroup> groups;

	// Bounding volume of the mesh.
	BoundingBox boundingVolume;

	// Geometry of the mesh.
	GeometryBufferPtr geometryBuffer;

	// Keeps track if the mesh has been built.
	bool built;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Mesh );

//-----------------------------------//

NAMESPACE_RESOURCES_END