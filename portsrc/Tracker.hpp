//
// Tracker.hpp
//
// Created on: Aug 25, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_TRACKER_HPP_)
#define MOSSE_TRACKER_HPP_

#include <array>

namespace Mosse {

using PointRowCol = std::array<unsigned, 2>;  // row, col

struct Image {
	const void *data;
	PointRowCol size;
};

struct Roi {
	PointRowCol origin;
	PointRowCol size;
};

struct TrackingInfo {
};

class Tracker {
public:
	virtual void init(Image, Roi);
	virtual const TrackingInfo &update(Image);
};

}  // namespace Mosse

#endif // MOSSE_TRACKER_HPP_
