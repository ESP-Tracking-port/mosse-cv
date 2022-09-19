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

class ParallelOps : public Ops {
private:
	struct Threading {
		std::vector<ThreadedOps> threadedOpWrappers;
		std::vector<std::unique_ptr<Port::Thread>> opThreads;
	};
public:
	void requestStop();
	ParallelOps(std::vector<std::reference_wrapper<Ops>> ops, Port::Thread &thread);
	void imageConvFftDomain(void *aioCropFft2Complex, void *aMatrixAcomlex, void *aMatrixBcomplex) override;
protected:
	void initImpl() override;
	void fft2(void *aBufferComplex) override;
	void ifft2(void *aBufferComplex) override;
private:
	template <class C, class ...Ts>
	inline void setExec(C &&c, Ts &&...aArgs)
	{
		for (auto op : threading.threadedOpWrappers) {
			op.setExec(c, std::forward<Ts>(aArgs)...);
		}
	}
private:
	std::vector<std::reference_wrapper<Ops>> ops;
	Threading threading;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_OPS_PARALLELOPS_HPP_
