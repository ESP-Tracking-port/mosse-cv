//
// ParallelOps.hpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_OPS_PARALLELOPS_HPP_)
#define MOSSE_OPS_PARALLELOPS_HPP_

#include "Util/Ops.hpp"
#include "Util/Ops/ThreadedOps.hpp"
#include <vector>
#include <functional>
#include <memory>

namespace Mosse {
namespace Port {

class Thread;

}  // namespace Port

namespace Ut {

class ArithmBase;
class MemLayoutBase;

class ParallelOps : public DecomposedOps {
private:
	struct Threading {
		std::vector<ThreadedOps> threadedOpWrappers;
		std::vector<std::unique_ptr<Mosse::Port::Thread>> opThreads;
		void waitDone();
	};

	/// \brief Low level memory and arithmetic operations overriden for a particular implementation being used
	///
	struct LowLevelAtomics {
		struct {
			ArithmBase &arithm;
			MemLayoutBase &memLayout;
		} buffer;
	};

public:
	void requestStop();
	ParallelOps(std::vector<std::reference_wrapper<DecomposedOps>> ops, Mosse::Port::Thread &thread,
		ArithmBase &aArithmBaseBuffer, MemLayoutBase &aMemLayoutBaseBuffer);
	void imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex) override;
	void initImpl() override;
	void fft2(void *aBufferComplex) override;
	void ifft2(void *aBufferComplex) override;
	void mataUpdate(void *aMatAcomplex, const void *aImageCropFftComplex, bool aInitial) override;
	void matbUpdate(void *aMatBcomplex, const void *aImageCropFftComplex, bool aInitial) override;
	Tp::NumVariant imageLog2Sum(Tp::Image aImage) override;
	Tp::NumVariant imageAbsDevLog2Sum(Tp::Image aImage, Tp::NumVariant aMean) override;
	void imageCropPreprocessImpl(Tp::Image aImageReal, void *aBufferComplex, Tp::NumVariant aLog2Sum,
		Tp::NumVariant aAbsDevLog2Sum) override;
	void maxReal(const void *aComplexBuffer, Tp::PointRowCol &aPeakPos, float *aSum) override;
	float calcPsr(const void *aComplexBuffer, const Tp::PointRowCol &aPeak, float sumHint,
		Tp::PointRowCol aMask = {11, 11}) override;
	float bufferAtAsFloat(const void *aComplexBuffer, const Tp::PointRowCol &aPeak) override;  ///< Represents the peak value as a floating-point number
	float bufferSum(const void *aComplexBuffer, const Tp::Roi &aRoi) override;  ///< Calculates `mean` using `aSumHint` calculated in `maxReal`
	float bufferAbsDevSum(const void *aComplexBuffer, const Tp::Roi &aRoi, float aMean) override;
private:
	template <class C, class ...Ts>
	inline void setExec(C &&c, Ts &&...aArgs)
	{
		for (auto &op : threading.threadedOpWrappers) {
			op.setExec(c, std::forward<Ts>(aArgs)...);
		}

		threading.waitDone();
	}
private:
	std::vector<std::reference_wrapper<DecomposedOps>> ops;
	Threading threading;
	LowLevelAtomics lowLevelAtomics;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_OPS_PARALLELOPS_HPP_
