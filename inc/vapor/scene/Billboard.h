/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Camera.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Billboards come in different flavors. Screen aligned billboards are
 * the simplest, they are always aligned to the screen. Use these for
 * text and GUI stuff. World aligned billboards will rotate in an
 * arbitrary axis to face the camera, thus they are useful for clouds
 * and other distant objects. Axis aligned billboards can rotate only
 * with respect to a given axis. These are most useful to represent
 * trees, since you don't want trees to rotate to face the camera
 * for example, when you fly over them.
 */

namespace BillboardType
{
	enum Enum
	{
		ScreenAligned,
		WorldAligned,
		AxisAligned,
	};
}

//-----------------------------------//

/**
 * Billboards always face the direction of the camera.
 */

class Billboard : public Component
{
public:

	Billboard( BillboardType::Enum type );
	~Billboard();
  
public:

	// Billboard type and colors
	BillboardType::Enum billboardType;

	// Component type
	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Billboard );

//-----------------------------------//

} } // end namespaces