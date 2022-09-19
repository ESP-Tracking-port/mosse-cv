//
// ParallelOps.hpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_OPS_PARALLELOPS_HPP_)
#define MOSSE_OPS_PARALLELOPS_HPP_

#include "Util/Ops.hpp"
#include <vector>
#include <functional>
#include <memory>

namespace Mosse {
namespace Port {

class Thread;

}  // namespace Port

namespace Ut {

class ThreadedOps;

class ParallelOps : public Ops {
private:
	struct Threading {
		std::vector<ThreadedOps> threadedOpWrappers;
		std::vector<std::unique_ptr<Port::Thread>> opThreads;
	};
public:
	void requestStop();
	ParallelOps(std::vector<std::reference_wrapper<Ops>> ops, Port::Thread &thread);
protected:
	void initImpl() override;
private:
	std::vector<std::reference_wrapper<Ops>> ops;
	Threading threading;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_OPS_PARALLELOPS_HPP_
