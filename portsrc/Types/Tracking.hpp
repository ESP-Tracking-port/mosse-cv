//
// Tracking.hpp
//
// Created on: Aug 29, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_TYPES_TRACKING_HPP_)
#define MOSSE_TYPES_TRACKING_HPP_

#include <array>
#include <Eigen/Core>

namespace Mosse {
namespace Tp {

using ImageBase = Eigen::Map<Eigen::Matrix<std::uint8_t, Eigen::Dynamic, Eigen::Dynamic>>;

struct Image : ImageBase {
	using ImageBase::ImageBase;
};

struct PointRowCol : Eigen::Vector2i {
	using Eigen::Vector2i::Vector2i;
};

struct Roi {
	PointRowCol origin;
	PointRowCol size;

	std::size_t area() const;
	bool isInside(const PointRowCol &) const;

	inline auto rows() -> decltype(size(0))
	{
		return size(0);
	}

	inline auto cols() -> decltype(size(1))
	{
		return size(1);
	}
};

struct TrackingInfo {
};

}  // namespace Tp
}  // namespace Mosse

#endif // MOSSE_TYPES_TRACKING_HPP_
