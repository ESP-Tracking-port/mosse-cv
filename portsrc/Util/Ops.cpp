//
// Ops.cpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Tracking.hpp"
#include "Util/Arithm/Conv.hpp"
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

float Ops::imageLog2Sum(Tp::Image aImage)
{
	const float *logTable = Mosse::getLogTable8bit();
	auto blockImage = aImage.block(roi().origin(0), roi().origin(1), roi().size(0), roi().size(1));
	float sum = 0.0f;

	for (unsigned row = 0; row < roi().rows(); ++row) {
		for (unsigned col = 0; col < roi().cols(); ++col) {
			sum += logTable[blockImage(row, col)];
			mosseassertnotnan(CommonOps::imageLogSum, blockImage(row, col), blockImage(row, col), roi());
			mosseassertnotnan(CommonOps::imageLogSum, logTable[blockImage(row, col)], row, col,
				blockImage(row, col));
		}
	}

	return sum;
}

float Ops::imageAbsDevLog2Sum(Tp::Image aImage, float mean)
{
	float devsum = 0.0f;
	auto blockImage = aImage.block(roi().origin(0), roi().origin(1), roi().size(0), roi().size(1));
	const float *logTable = Mosse::getLogTable8bit();

	for (unsigned row = 0; row < roi().rows(); ++row) {
		for (unsigned col = 0; col < roi().cols(); ++col) {
			devsum += fabs(mean - logTable[blockImage(row, col)]);
		}
	}

	return devsum;
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
