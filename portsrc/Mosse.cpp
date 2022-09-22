//
// Mosse.cpp
//
// Created on: Sep 06, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Mosse.hpp"
#include <vector>

namespace Mosse {

Tracker &getNaive()
{
	static Mosse::Ut::RawF32Ops ops;
	static Mosse::Ut::DynRawF32Mem mem;
	static Mosse::Ut::Port port{ops, mem};
	static Mosse::Tracker tracker{port};

	return tracker;
}

Tracker &getDebugStub()
{
	static Mosse::Ut::DebugRawF32Ops ops;
	static Mosse::Ut::DynRawF32Mem mem;
	static Mosse::Ut::Port port{ops, mem};
	static Mosse::Tracker tracker{port};

	return tracker;
}

template <class T>
static std::vector<std::reference_wrapper<Mosse::Ut::DecomposedOps>> makeRefs(T &aOpsContainer)
{
	std::vector<std::reference_wrapper<Mosse::Ut::DecomposedOps>> ret;

	for (auto &ops : aOpsContainer) {
		ret.push_back(std::ref<Mosse::Ut::DecomposedOps>(ops));
	}

	return ret;
}

Tracker &getNaiveMultithreaded(Port::Thread &aThread, unsigned anThreads)
{
	static Ut::Arithm<Ut::RawF32Ops::reprFlags.buffer> arithmOpsBuffer;
	static Ut::MemLayout<Ut::RawF32Ops::reprFlags.buffer> memLayoutOpsBuffer;
	static std::vector<Mosse::Ut::RawF32Ops> workerOps{anThreads};
	static Mosse::Ut::ParallelOps ops{makeRefs(workerOps), aThread, arithmOpsBuffer, memLayoutOpsBuffer};
	static Mosse::Ut::DynRawF32Mem mem;
	static Mosse::Ut::Port port{ops, mem};
	static Mosse::Tracker tracker{port};

	return tracker;
}

}  // namespace Mosse
