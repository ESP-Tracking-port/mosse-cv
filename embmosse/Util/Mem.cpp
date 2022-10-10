//
// Mem.cpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "embmosse/Types/Tracking.hpp"
#include "embmosse/Types/Repr.hpp"
#include "embmosse/Util/Mem.hpp"

namespace Mosse {
namespace Ut {

void Mem::initImageWorkingArea(const Tp::Image &, const Tp::Roi &)
{
}

void *Mem::imageWorkingArea()
{
	return nullptr;
}

void Mem::init(Tp::Roi)
{
}

}  // namespace Ut
}  // namespace Mosse
