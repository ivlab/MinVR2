/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/graphics/structure/VRViewportCalculator.h>
#include "VRTile.h"

namespace MinVR {

VRViewportCalculator::VRViewportCalculator(bool modifyTile) : m_modifyTile(modifyTile) {
}

VRViewportCalculator::~VRViewportCalculator() {
}

void VRViewportCalculator::preRender(VRRenderer& renderer,
		const VRViewport& viewport) {

	const VRViewport* currentViewport = &viewport;

	VRViewport oldViewport;
	VRViewport modifiedViewport;
	if (renderer.getState().readValue("viewport", oldViewport))
	{
		modifiedViewport = oldViewport.generateChild(viewport);
		currentViewport = &modifiedViewport;
	}

	renderer.pushState();

	if (m_modifyTile)
	{
		VRTile tile;
		if (renderer.getState().readValue("tile", tile))
		{
			tile = tile.modifyWithViewport(oldViewport, *currentViewport);
			renderer.getState().writeValue("tile", tile);
		}
	}

	renderer.getState().writeValue("viewport", *currentViewport);
}

void VRViewportCalculator::postRender(VRRenderer& renderer) {
	renderer.popState();
}

} /* namespace MinVR */