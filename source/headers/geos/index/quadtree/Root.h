/**********************************************************************
 * $Id$
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.refractions.net
 *
 * Copyright (C) 2006 Refractions Research Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation. 
 * See the COPYING file for more information.
 *
 **********************************************************************
 *
 * Last port: index/quadtree/Root.java rev 1.7 (JTS-1.10)
 *
 **********************************************************************/

#ifndef GEOS_IDX_QUADTREE_ROOT_H
#define GEOS_IDX_QUADTREE_ROOT_H

#include <geos/index/quadtree/NodeBase.h> // for inheritance
#include <geos/geom/Coordinate.h> // for composition

// Forward declarations
namespace geos {
	namespace geom {
		class Envelope;
	}
	namespace index {
		namespace quadtree {
			class Node;
		}
	}
}

namespace geos {
namespace index { // geos::index
namespace quadtree { // geos::index::quadtree

/**
 * \class Root indexQuadtree.h geos/indexQuadtree.h
 *
 * \brief
 * QuadRoot is the root of a single Quadtree.  It is centred at the origin,
 * and does not have a defined extent.
 */
class Root: public NodeBase {
//friend class Unload;

private:

	static const geom::Coordinate origin;

	/**
	 * insert an item which is known to be contained in the tree rooted at
	 * the given QuadNode root.  Lower levels of the tree will be created
	 * if necessary to hold the item.
	 */
	void insertContained(Node *tree, const geom::Envelope *itemEnv,
	                     void* item);

public:

	Root() {}

	virtual ~Root() {}

	/**
	 * Insert an item into the quadtree this is the root of.
	 */
	void insert(const geom::Envelope *itemEnv, void* item);

protected:

	bool isSearchMatch(const geom::Envelope& /* searchEnv */) const {
		return true;
	}

};

} // namespace geos::index::quadtree
} // namespace geos::index
} // namespace geos

#endif // GEOS_IDX_QUADTREE_ROOT_H

/**********************************************************************
 * $Log$
 * Revision 1.2  2006/06/12 17:15:29  strk
 * Removed unused parameters warning
 *
 * Revision 1.1  2006/03/22 12:22:50  strk
 * indexQuadtree.h split
 *
 **********************************************************************/

