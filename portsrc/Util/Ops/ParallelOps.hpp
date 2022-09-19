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

namespace Mosse {
namespace Ut {

class ParallelOps : public Ops {
public:
	ParallelOps(std::vector<std::reference_wrapper<Ops>> ops);
protected:
	void initImpl() override;
private:
	std::vector<std::reference_wrapper<Ops>> ops;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_OPS_PARALLELOPS_HPP_
