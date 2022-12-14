//
// Ops.cpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "embmosse/Types/Tracking.hpp"
#include "embmosse/Types/Repr.hpp"
#include "embmosse/Util/Arithm/Conv.hpp"
#include "embmosse/Util/Helper/EigenMem.hpp"
#include "embmosse/Port/MossePort.hpp"
#include "Ops.hpp"
#include "MosseApi.hpp"
#include <cmath>

namespace Mosse {
namespace Ut {

void Ops::init(Tp::Roi aRoi)
{
	mRoi = aRoi;
	initImpl();
	firstInit = false;
}

void DecomposedOps::imageCropInto(const Tp::Image &aImage, void *aBufferCplx)
{
	float sum = imageLog2Sum(aImage).f32;  // Calculating sum
	const float mean = sum / static_cast<float>(roi().area());
	float devsum = imageAbsDevLog2Sum(aImage, {mean}).f32;
	const float stddev = devsum / sqrtf(static_cast<float>(roi().area()));
	imageCropPreprocessImpl(aImage, aBufferCplx, {mean}, {stddev});
}

void DecomposedOps::imagePreprocess(void *)
{
}

const Tp::Roi &Ops::roi() const
{
	return mRoi;
}

void Ops::initImpl()
{
}

void Ops::tearDownImpl()
{
}

void Ops::roiResize(Mosse::Tp::Roi &aRoi)
{
	unsigned rows = aRoi.size(0);
	unsigned cols = aRoi.size(1);
	Mosse::getClosestWindow(rows, cols);
	aRoi.readjust({rows, cols});
}

Tp::NumVariant DecomposedOps::imageLog2Sum(const Tp::Image &aImage)
{
	float sum = 0.0f;
	auto blockImage = Ut::makeEigenBlock(aImage, roi());
	auto blockFragment = Ut::makeEigenBlock(blockImage, roiFragment());
	const float *logTable = Mosse::getLogTable8bit();

	for (unsigned row = 0; row < blockFragment.rows(); ++row) {
		for (unsigned col = 0; col < blockFragment.cols(); ++col) {
			sum += logTable[blockFragment(row, col)];
		}
	}

	return {sum};
}

Tp::NumVariant DecomposedOps::imageAbsDevLog2Sum(const Tp::Image &aImage, Tp::NumVariant mean)
{
	float devsum = 0.0f;
	auto blockImage = Ut::makeEigenBlock(aImage, roi());
	auto blockFragment = Ut::makeEigenBlock(blockImage, roiFragment());
	const float *logTable = Mosse::getLogTable8bit();

	for (unsigned row = 0; row < blockFragment.rows(); ++row) {
		for (unsigned col = 0; col < blockFragment.cols(); ++col) {
			devsum += ::abs(mean.f32 - logTable[blockFragment(row, col)]);
		}
	}

	return {devsum};
}

float DecomposedOps::bufferAtAsFloat(const void *, const Tp::PointRowCol &)
{
	mosse_assert(false);  // The inherited class either re-implements `calcPsr`, or reimplements each of its decomposed operations
}

float DecomposedOps::bufferSum(const void *, const Tp::Roi &)
{
	mosse_assert(false);  // The inherited class either re-implements `calcPsr`, or reimplements each of its decomposed operations
}

float DecomposedOps::bufferAbsDevSum(const void *, const Tp::Roi &, float)
{
	mosse_assert(false);  // The inherited class either re-implements `calcPsr`, or reimplements each of its decomposed operations
}

void DecomposedOps::imageCropPreprocessImpl(const Tp::Image &, void *, Tp::NumVariant, Tp::NumVariant)
{
}

float DecomposedOps::calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak, float aSumHint,
	Tp::PointRowCol aMask)
{
	Tp::Roi roiMask{aPeak - (aMask / 2), aMask};
	roiMask.fitShift(roi().size);
	const float sizeMasked = static_cast<float>(roi().area() - roiMask.area());
	float mean = (aSumHint - bufferSum(aComplexBuffer, roiMask)) / sizeMasked;
	float devsum = bufferAbsDevSum(aComplexBuffer, roiFragment(), mean) - bufferAbsDevSum(aComplexBuffer, roiMask, mean);
	float stddev = devsum / sqrtf(sizeMasked);
	float maxValue = bufferAtAsFloat(aComplexBuffer, aPeak);
	float psr = (maxValue - mean) / stddev;

	return psr;
}

void DecomposedOps::initImpl()
{
	if (isFirstInit()) {
		setRoiFragment({{0, 0}, roi().size});
	}
}

}  // namespace Ut
}  // namespace Mosse
