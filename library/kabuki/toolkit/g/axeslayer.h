/*
	Copyright 2009 Daniel Stahlke

	This file is part of scopemm.
	
	scopemm is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	scopemm is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with scopemm.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SCOPEMM_AXES_H
#define SCOPEMM_AXES_H

#include "scopemm/plotcanvas.h"

namespace scopemm {

class AxesLayerImpl;

class AxesLayer : public PlotLayerSub<AxesLayerImpl> {
public:
	AxesLayer();
	~AxesLayer() { }

	AxesLayer &setDrawXAxis(bool state);
	AxesLayer &setDrawYAxis(bool state);
};

} // namespace scopemm

#endif // SCOPEMM_AXES_H