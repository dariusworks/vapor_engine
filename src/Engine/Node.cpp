/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Node.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Node::Node( NodePtr parent )
	: parent( parent )
{

}

//-----------------------------------//

Node::~Node()
{

}

//-----------------------------------//

NodePtr Node::getParent( ) const
{
	NodePtr ret( parent.lock() );
	return ret;
}

//-----------------------------------//

void Node::setParent( NodePtr parent )
{
	this->parent = parent;
}

//-----------------------------------//

const std::string Node::name() const
{
	return "Node";
}

//-----------------------------------//

} } // end namespaces