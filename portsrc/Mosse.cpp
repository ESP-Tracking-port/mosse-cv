//
// Mosse.cpp
//
// Created on: Sep 06, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Mosse.hpp"
#include "Util/MosseDebug.hpp"

namespace Mosse {

Tracker &getNaive()
{
	static Mosse::Ut::RawF32Ops ops;
	static Mosse::Ut::DynRawF32Mem mem;
	static Mosse::Ut::Port port{ops, mem};
	static Mosse::Tracker tracker{port};

	return tracker;
}

}  // namespace Mosse
