//
// DebugRawF32Ops.hpp
//
// Created on: Sep 09, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_OPS_DEBUGRAWF32OPS_HPP_)
#define MOSSE_UTIL_OPS_DEBUGRAWF32OPS_HPP_

#include "embmosse/Util/Ops.hpp"
#include <memory>

namespace Mosse {
namespace Ut {

class OpencvNativeRawF32Ops;
class RawF32Ops;
class ParallelOps;

/// \brief Decorator class for gradually reimplementing the reference CV-based MOSSE tracking algorithm implementation
/// w/ the custom one.
///
class DebugRawF32Ops : public Ops {
private:
	struct Wrapped {
		OpencvNativeRawF32Ops opencvNativeRawF32Ops;
		RawF32Ops rawF32Ops;

		struct {
			RawF32Ops th1;
			RawF32Ops th2;
			std::unique_ptr<ParallelOps> ops;
		} parallelOps;
	};
public:
	DebugRawF32Ops();
	void imageCropInto(Tp::Image aImageReal, void *aBufferComplex) override;
	void imagePreprocess(void *aCropComplex) override;
	void imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex) override;
	void fft2(void *aBufferComplex) override;
	void ifft2(void *aBufferComplex) override;
	void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPeakPos, float *sum = nullptr) override;
	float calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak, float sumHint,
		Tp::PointRowCol aMask = {11, 11}) override;
	void mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial) override;
	void matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial) override;
	void initImpl() override;
private:
	Wrapped wrapped;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_OPS_DEBUGRAWF32OPS_HPP_
