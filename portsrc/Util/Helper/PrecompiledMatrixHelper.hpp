//
// PrecompiledMatrixHelper.hpp
//
// Created on: Sep 09, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_PRECOMPILEDMATRIXHELPER_HPP_)
#define MOSSE_UTIL_HELPER_PRECOMPILEDMATRIXHELPER_HPP_

#include "Types/Tracking.hpp"

namespace Mosse {
namespace Ut {

template <class T>
class PrecompiledMatrixHelper {
public:
	using GetterType = T(*)(unsigned, unsigned);
private:
	struct Mat {
		const void *gauss;
		const void *hann;
	};

	struct Getter {
		GetterType getGauss;
		GetterType getHann;
	};
public:
	constexpr PrecompiledMatrixHelper(GetterType aGetterGauss, GetterType aGetterHann) :
		getter{aGetterGauss, aGetterHann}
	{
	}

	void update(const Tp::Roi &aRoi)
	{
		if (roiPrev.size != aRoi.size) {
			mat = {
				static_cast<const void *>(getter.getGauss(aRoi.rows(), aRoi.cols())),
				static_cast<const void *>(getter.getHann(aRoi.rows(), aRoi.cols()))
			};
		}
	}

	void *gauss()
	{
		return mat.gauss;
	}

	void *hann()
	{
		return mat.hann;
	}
private:
	Mat mat = {nullptr, nullptr};
	Getter getter = nullptr;
	Tp::Roi roiPrev = {{0, 0}, {0, 0}};
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_PRECOMPILEDMATRIXHELPER_HPP_
