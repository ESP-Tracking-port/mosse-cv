//
// Mem.cpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Repr.hpp"
#include "Util/Mem.hpp"

namespace Mosse {
namespace Ut {

Mem::Mem(Tp::Geometry aGeometry) : geo{aGeometry}
{
}

const Tp::Geometry &Mem::geometry() const
{
	return geo;
}

}  // namespace Ut
}  // namespace Mosse
