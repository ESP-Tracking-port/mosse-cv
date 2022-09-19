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

class ParallelOps : public Ops {
private:
	struct Threading {
		Port::Thread &thread;
		std::vector<std::unique_ptr<Port::Thread>> opThreads;
		// TODO ThreadedOp-s
	};
public:
	// TODO tear down threads
	ParallelOps(std::vector<std::reference_wrapper<Ops>> ops, Port::Thread &thread);
protected:
	void initImpl() override;
private:
	std::vector<std::reference_wrapper<Ops>> ops;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_OPS_PARALLELOPS_HPP_
