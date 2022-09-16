//
// Ops.cpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "Types/Repr.hpp"
#include "Util/Arithm/Conv.hpp"
#include "Util/Helper/EigenMem.hpp"
#include "MossePort.hpp"
#include "Ops.hpp"
#include "MosseApi.hpp"

namespace Mosse {
namespace Ut {

void Ops::init(Tp::Roi aRoi)
{
	mRoi = aRoi;
	ohdebug(Ops::init, aRoi);
	initImpl();
}

void Ops::imageCropInto(Tp::Image aImage, void *aBufferCplx)
{
	float sum = imageLog2Sum(aImage).f32;  // Calculating sum
	const float mean = sum / static_cast<float>(roi().area());
	float devsum = imageAbsDevLog2Sum(aImage, {mean}).f32;
	const float stddev = devsum / sqrt(static_cast<float>(roi().area()));
	imageCropPreprocessImpl(aImage, aBufferCplx, {mean}, {stddev});
}

void Ops::imagePreprocess(void *)
{
}

const Tp::Roi &Ops::roi() const
{
	return mRoi;
}

void Ops::initImpl()
{
	ohdebug(Ops::initImpl);
	setRoiFragment({{0, 0}, roi().size});
}

Tp::NumVariant Ops::imageLog2Sum(Tp::Image aImage)
{
	const float *logTable = Mosse::getLogTable8bit();
	auto blockImage = Ut::makeEigenBlock(aImage, roi());
	auto blockFragment = Ut::makeEigenBlock(blockImage, roiFragment());
	float sum = 0.0f;

	for (unsigned row = 0; row < blockFragment.rows(); ++row) {
		for (unsigned col = 0; col < blockFragment.cols(); ++col) {
			sum += logTable[blockImage(row, col)];
			mosseassertnotnan(CommonOps::imageLogSum, blockImage(row, col), blockImage(row, col), roi());
			mosseassertnotnan(CommonOps::imageLogSum, logTable[blockImage(row, col)], row, col,
				blockImage(row, col));
		}
	}

	return {sum};
}

Tp::NumVariant Ops::imageAbsDevLog2Sum(Tp::Image aImage, Tp::NumVariant mean)
{
	float devsum = 0.0f;
	auto blockImage = Ut::makeEigenBlock(aImage, roi());
	auto blockFragment = Ut::makeEigenBlock(blockImage, roiFragment());
	const float *logTable = Mosse::getLogTable8bit();
	ohdebugonce(Ops::imageAbsDevLog2Sum, 0, roi(), roiFragment(), blockImage.size());

	for (unsigned row = 0; row < blockFragment.rows(); ++row) {
		for (unsigned col = 0; col < blockFragment.cols(); ++col) {
			devsum += fabs(mean.f32 - logTable[blockImage(row, col)]);
		}
	}

	return {devsum};
}

void Ops::imageCropPreprocessImpl(Tp::Image, void *, Tp::NumVariant, Tp::NumVariant)
{
}

void Ops::roiResize(Mosse::Tp::Roi &aRoi)
{
	unsigned rows = aRoi.size(0);
	unsigned cols = aRoi.size(1);
	Mosse::getClosestWindow(rows, cols);
	ohdebug(Ops::roiResize, "closest window", rows, cols);
	aRoi.readjust({rows, cols});
	ohdebug(Ops::roiResize, "after resize", aRoi);
}

}  // namespace Ut
}  // namespace Mosse
