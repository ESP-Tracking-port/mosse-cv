//
// Mosse.cpp
//
// Created on: Sep 06, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Mosse.hpp"
#include "Port/MossePort.hpp"
#include "Util/Ops/OpencvNativeRawF32Ops.hpp"
#include "Util/Ops/DebugRawF32Ops.hpp"

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

}  // namespace Mosse
