//
// Tracking.cpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Tracking.hpp"
#include "Util/Arithm/Algo.hpp"
#include "MossePort.hpp"

namespace Mosse {
namespace Tp {

std::size_t Roi::area() const
{
	return size(0) * size(1);
}

bool Roi::isInside(const PointRowCol &aPoint) const
{
	auto diff = aPoint - origin;

	return diff(0) >= 0 && diff(0) < size(0) && diff(1) >= 0 && diff(1) <= size(1);
}

void Roi::readjust(const PointRowCol &aSize)
{
	ohdebug(Roi::readjust, "before resize", aSize, origin, size);
	origin = origin + size / 2 - aSize / 2;
	size = aSize;
	ohdebug(Roi::readjust, "after resize", origin, size);
	origin = origin + size / 2 - aSize / 2;
}

void Roi::setCenter(const PointRowCol &aCenter)
{
	origin = aCenter - (size / 2);
}

void Roi::fitShift(const PointRowCol &aOuterBoundSize)
{
	for (auto c : {0, 1}) {
		assert(size(c) < aOuterBoundSize(c));  ///< The ROI must fit
		Ut::clamp(origin(c), 0, aOuterBoundSize(c) - size(c));
	}
}

bool operator==(const Roi &aLhs, const Roi &aRhs)
{
	return aLhs.origin == aRhs.origin && aLhs.size == aRhs.size;
}

bool operator!=(const Roi &aLhs, const Roi &aRhs)
{
	return !(aLhs.origin == aRhs.origin && aLhs.size == aRhs.size);
}

}  // namespace Tp
}  // namespace Mosse
