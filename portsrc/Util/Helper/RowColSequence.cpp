//
// RowColSequence.cpp
//
// Created on: Sep 14, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "RowColSequence.hpp"
#include <limits>
#include <type_traits>

namespace Mosse {
namespace Ut {

using PointValueType = typename std::remove_reference<decltype(Tp::PointRowCol{}(0))>::type;
static const Tp::PointRowCol kPointMarkerEnd{std::numeric_limits<PointValueType>::max(),
	std::numeric_limits<PointValueType>::max()};

RowColSequence::Iterator RowColSequence::begin()
{
	return {*this};
}

RowColSequence::Iterator RowColSequence::end()
{
	return {};
}

RowColSequence::Iterator::Iterator(RowColSequence &aOwner) : index{0}, point{kPointMarkerEnd}, owner{&aOwner}
{
	owner->pointInit(point);
}

RowColSequence::Iterator::Iterator() : index{0}, point{kPointMarkerEnd}, owner{nullptr}
{
}

RowColSequence::Iterator &RowColSequence::Iterator::operator++()
{
	if (!owner->pointAdvance(index, point)) {
		point = kPointMarkerEnd;
	}

	++index;

	return *this;
}

Tp::PointRowCol *RowColSequence::Iterator::operator*()
{
	return &point;
}

bool operator==(const RowColSequence::Iterator &aLhs, const RowColSequence::Iterator &aRhs)
{
	return aLhs.point == aRhs.point;
}

}  // namespace Ut
}  // namespace Mosse
