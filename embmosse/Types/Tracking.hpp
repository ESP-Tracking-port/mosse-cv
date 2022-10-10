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

/// \brief 2D point
///
struct PointRowCol : Eigen::Vector2i {
	using Eigen::Vector2i::Vector2i;
};

/// \brief Region of interest encapsulation. ((ORIGIN_ROW, ORIGIN_COL), (N_ROWS, N_COLS))
///
struct Roi {
	PointRowCol origin;
	PointRowCol size;

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
	int area() const;
	bool isInside(const PointRowCol &) const;
	void setCenter(const PointRowCol &aCenter);  ///< Update the ROI's center preserving its size
	void fitShift(const PointRowCol &aOuterBoundSize);  ///< Fit ROI in the bounding box preserving its size
	void fitCrop(const PointRowCol &aOuterBoundSize);  ///< Fit ROI in the bounding box preserving its position
	void fitShiftRoi(const Roi &);  ///< Fit ROI into another ROI preserving its size
	friend bool operator==(const Roi &, const Roi&);
	friend bool operator!=(const Roi &, const Roi&);
};

using ImageBase = Eigen::Map<Eigen::Matrix<std::uint8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>;

/// \brief Wrapper that translates calls to the underlying wrapped instance of `ImageBase`. It is used due to
/// necessity to swap `ImageBase` with an offsetting wrapper.
///
class Image {
protected:
	ImageBase imageBase;
public:
	virtual ~Image() = default;
	virtual auto operator()(Eigen::Index aRow, Eigen::Index aCol) -> decltype(imageBase(aRow, aCol));
	virtual auto operator()(Eigen::Index aRow, Eigen::Index aCol) const -> decltype(imageBase(aRow, aCol));
	Image(std::uint8_t *aData, Eigen::Index aHeight, Eigen::Index aWidth);
	virtual auto block(Eigen::Index aRow, Eigen::Index aCol, Eigen::Index anRows, Eigen::Index anCols)
		-> decltype(imageBase.block(aRow, aCol, anRows, anCols)) const;
	virtual Eigen::Index rows() const;
	virtual Eigen::Index cols() const;

	inline auto data() -> decltype(imageBase.data())
	{
		return imageBase.data();
	}
	inline auto data() const -> decltype(imageBase.data())
	{
		return imageBase.data();
	}
};

/// \brief Maps a chunk of an image
///
class OffsetImage : public Image {
private:
private:
	Roi roi;
	PointRowCol virtSize;
public:
	OffsetImage(const Roi &aRoi, std::uint8_t *aData);
	auto operator()(Eigen::Index aRow, Eigen::Index aCol) -> decltype(imageBase(aRow, aCol)) override;
	auto operator()(Eigen::Index aRow, Eigen::Index aCol) const -> decltype(imageBase(aRow, aCol)) override;
	Eigen::Index rows() const override;
	Eigen::Index cols() const override;
	auto block(Eigen::Index aRow, Eigen::Index aCol, Eigen::Index anRows, Eigen::Index anCols)
		-> decltype(imageBase.block(aRow, aCol, anRows, anCols)) const override;
};

}  // namespace Tp
}  // namespace Mosse

#endif // MOSSE_TYPES_TRACKING_HPP_
