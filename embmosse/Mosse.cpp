//
// Mosse.cpp
//
// Created on: Sep 06, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <Fft.h>
#include "embmosse/Util/Helper/EspDspFft2.hpp"
#include "embmosse/Util/Ops/EspDspRawF32Ops.hpp"
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

Tracker &getNaiveRadix2Fft()
{
	static Mosse::Ut::EspDspRawF32Ops ops;
	static Mosse::Ut::DynRawF32Mem mem;
	static Mosse::Ut::Port port{ops, mem};
	static Mosse::Tracker tracker{port};

	return tracker;
}

Tracker &getFp16AbRawF32BufDynAlloc()
{
	static Mosse::Ut::FpI16AbRawF32Ops ops;
	static Mosse::Ut::DynRawMem<Mosse::Ut::FpI16AbRawF32Ops::reprFlags.buffer,
		Mosse::Ut::FpI16AbRawF32Ops::reprFlags.matAb> mem;
	static Mosse::Ut::Port port{ops, mem};
	static Mosse::Tracker tracker{port};

	return tracker;
}

Tracker &getFp16AbRawF32BufDynAllocThreaded(Port::Thread &aThread, unsigned anThreads)
{
	static Ut::Arithm<Ut::FpI16AbRawF32Ops::reprFlags.buffer> bufferArithmOps;
	static Ut::MemLayout<Ut::FpI16AbRawF32Ops::reprFlags.buffer> bufferMemLayoutOps;
	static std::vector<Mosse::Ut::FpI16AbRawF32Ops> workerOps{anThreads};
	static Mosse::Ut::ParallelOps ops{makeRefs(workerOps), aThread, bufferArithmOps, bufferMemLayoutOps};
	static Mosse::Ut::DynRawMem<Mosse::Ut::FpI16AbRawF32Ops::reprFlags.buffer,
		Mosse::Ut::FpI16AbRawF32Ops::reprFlags.matAb> mem;
	static Mosse::Ut::Port port{ops, mem};
	static Mosse::Tracker tracker{port};
	mosse_assert(workerOps.size() == anThreads);

	return tracker;
}

Tracker &getFp16AbRawF32BufDynAllocThreadedSplit(Port::Thread &aThread, const std::vector<float> &aSplit) {
	mosse_assert(aSplit.size() > 0);
	const std::size_t nThreads = aSplit.size();
	static Ut::Arithm<Ut::FpI16AbRawF32Ops::reprFlags.buffer> bufferArithmOps;
	static Ut::MemLayout<Ut::FpI16AbRawF32Ops::reprFlags.buffer> bufferMemLayoutOps;
	static std::vector<Mosse::Ut::FpI16AbRawF32Ops> workerOps{nThreads};
	static Mosse::Ut::ParallelOps ops{makeRefs(workerOps), aThread, bufferArithmOps, bufferMemLayoutOps, aSplit};
	static Mosse::Ut::DynRawMem<Mosse::Ut::FpI16AbRawF32Ops::reprFlags.buffer,
		Mosse::Ut::FpI16AbRawF32Ops::reprFlags.matAb> mem;
	static Mosse::Ut::Port port{ops, mem};
	static Mosse::Tracker tracker{port};
	mosse_assert(workerOps.size() == nThreads);

	return tracker;
}

}  // namespace Mosse
