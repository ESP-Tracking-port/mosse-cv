//
// Tracking.cpp
//
// Created on: Aug 30, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Tracking.hpp"
#include "embmosse/Util/Arithm/Algo.hpp"
#include "embmosse/Port/MossePort.hpp"

namespace Mosse {
namespace Tp {

int Roi::area() const
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
	origin = origin + size / 2 - aSize / 2;
	size = aSize;
	origin = origin + size / 2 - aSize / 2;
}

void Roi::setCenter(const PointRowCol &aCenter)
{
	origin = aCenter - (size / 2);
}

void Roi::fitShift(const PointRowCol &aOuterBoundSize)
{
	for (auto c : {0, 1}) {
		mosse_assert(size(c) <= aOuterBoundSize(c));  ///< The ROI must fit
		Ut::clamp(origin(c), 0, aOuterBoundSize(c) - size(c) - 1);
	}
}

void Roi::fitCrop(const PointRowCol &aOuterBoundSize)
{
	for (auto c : {0, 1}) {
		mosse_assert(origin(c) < aOuterBoundSize(c) - 2 && origin(c) >= 0);
		Ut::clamp(size(c), aOuterBoundSize(c) - origin(c));
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

auto Image::operator()(Eigen::Index aRow, Eigen::Index aCol) -> decltype(imageBase(aRow, aCol))
{
	return imageBase(aRow, aCol);
}

auto Image::operator()(Eigen::Index aRow, Eigen::Index aCol) const -> decltype(imageBase(aRow, aCol))
{
	return imageBase(aRow, aCol);
}

auto Image::block(Eigen::Index aRow, Eigen::Index aCol, Eigen::Index anRows, Eigen::Index anCols)
	-> decltype(imageBase.block(aRow, aCol, anRows, anCols)) const
{
	return imageBase.block(aRow, aCol, anRows, anCols);
}

Eigen::Index Image::rows() const
{
	return imageBase.rows();
}

Eigen::Index Image::cols() const
{
	return imageBase.cols();
}

Image::Image(uint8_t *aData, Eigen::Index aHeight, Eigen::Index aWidth) : imageBase{aData, aHeight, aWidth}
{
}

OffsetImage::OffsetImage(const Roi &aRoi, uint8_t *aData) : roi{aRoi}, virtSize{roi.origin + roi.size},
	Image{aData, aRoi.size(0), aRoi.size(1)}
{
}

auto OffsetImage::operator()(Eigen::Index aRow, Eigen::Index aCol) -> decltype(imageBase(aRow, aCol))
{
	ohdebug("OffsetImage::operator()", aRow, aCol, roi);
	mosse_assert(roi.isInside({aRow, aCol}));

	return imageBase(aRow - roi.origin(0), aCol - roi.origin(1));
}

auto OffsetImage::operator()(Eigen::Index aRow, Eigen::Index aCol) const -> decltype(imageBase(aRow, aCol))
{
	ohdebug("OffsetImage::operator() const", aRow, aCol, roi);
	mosse_assert(roi.isInside({aRow, aCol}));

	return imageBase(aRow - roi.origin(0), aCol - roi.origin(1));
}

Eigen::Index OffsetImage::rows() const
{
	return virtSize(0);
}

Eigen::Index OffsetImage::cols() const
{
	return virtSize(1);
}

auto OffsetImage::block(Eigen::Index aRow, Eigen::Index aCol, Eigen::Index anRows, Eigen::Index anCols)
	-> decltype(imageBase.block(0, 0, 0, 0)) const
{
	ohdebug("roi.originImage::block() const", aRow, aCol, roi);
	mosse_assert(roi.isInside({aRow, aCol}));

	return imageBase.block(aRow - roi.origin(0), aCol - roi.origin(1), anRows, anCols);
}

}  // namespace Tp
}  // namespace Mosse
