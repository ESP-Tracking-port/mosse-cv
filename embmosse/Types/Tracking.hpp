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

using ImageBase = Eigen::Map<Eigen::Matrix<std::uint8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>;

class Image {
private:
	ImageBase imageBase;
public:
	virtual auto operator()(Eigen::Index aRow, Eigen::Index aCol) -> decltype(imageBase(aRow, aCol));
	Image(std::uint8_t *aData, Eigen::Index aHeight, Eigen::Index aWidth);

	inline auto data() -> decltype(imageBase.data())
	{
		return imageBase.data();
	}
	inline auto rows() const -> decltype(imageBase.rows())
	{
		return imageBase.rows();
	}
	inline auto cols() const -> decltype(imageBase.cols())
	{
		return imageBase.cols();
	}
	template <class ...Ts>
	inline auto block(Ts &&...aArgs) -> decltype(imageBase.block(std::forward<Ts>(aArgs)...))
	{
		return imageBase.block(std::forward<Ts>(aArgs)...);
	}
};

class PointRowCol;

struct PointRowCol : Eigen::Vector2i {
	using Eigen::Vector2i::Vector2i;
};

struct Roi {
	PointRowCol origin;
	PointRowCol size;

	int area() const;
	bool isInside(const PointRowCol &) const;

	inline auto rows() const -> decltype(size(0))
	{
		return size(0);
	}

	inline auto cols() const -> decltype(size(1))
	{
		return size(1);
	}

	/// \brief Adjust the ROI geometry for the new size preserving the ROI's center coordinates
	///
	void readjust(const PointRowCol &aSize);
	void setCenter(const PointRowCol &aCenter);  ///< Update the ROI's center preserving its size
	void fitShift(const PointRowCol &aOuterBoundSize);  ///< Fit ROI in the bounding box preserving its size
	void fitCrop(const PointRowCol &aOuterBoundSize);  ///< Fit ROI in the bounding box preserving its position

	friend bool operator==(const Roi &, const Roi&);
	friend bool operator!=(const Roi &, const Roi&);
};

struct TrackingInfo {
};

}  // namespace Tp
}  // namespace Mosse

#endif // MOSSE_TYPES_TRACKING_HPP_
