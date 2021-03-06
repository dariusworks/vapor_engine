/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Gizmo.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class GizmoTranslate : public Gizmo
{
public:

	GizmoTranslate( const EntityPtr&, const CameraWeakPtr& );

	// Builds the gizmo geometry.
	void buildGeometry() OVERRIDE;

	// Converts a color to a specific gizmo axis.
	GizmoAxis::Enum getAxis(Color&) OVERRIDE;

protected:

	// Highlights an axis to provide visual feedback of handle selection.
	void highlightAxis( GizmoAxis::Enum, bool highlight ) OVERRIDE;

	// Generates the cone geometry.
	GeometryBufferPtr generateCones();

	// Generates solid cone geometry for the gizmo arrows.
	void generateSolidCone( float base, float height, uint slices,
		std::vector<Vector3>& pos );

	// Generate nice colors for the gizmo.
	void generateColors( std::vector<Vector3>& colors, const Color& c );

	// Lines geometry.
	GeometryBufferPtr lines;

	// Cones geometry.
	GeometryBufferPtr cones;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GizmoTranslate );

//-----------------------------------//

NAMESPACE_EDITOR_END