//
// Tracking.cpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Tracking.hpp"

namespace Mosse {
namespace Tp {

std::size_t Roi::countElements()
{
	return size(0) * size(1);
}

}  // namespace Tp
}  // namespace Mosse
