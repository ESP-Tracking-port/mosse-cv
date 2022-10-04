//
// OsApi.cpp
//
// Created on: Oct 04, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "OsApi.hpp"
#include "embmosse/Port/MossePort.hpp"

namespace Mosse {
namespace Port {

static OsApi *sOsApiInstance = nullptr;

OsApi *OsApi::instance()
{
	return sOsApiInstance;
}

void OsApi::setInstance(OsApi &aInstance)
{
	mosse_assert(sOsApiInstance == nullptr);
	sOsApiInstance = &aInstance;
}

StubOsApi::StubOsApi()
{
	setInstance(*this);
}

void StubOsApi::taskYieldMinDelay()
{
}

}  // namespace Port
}  // namespace Mosse
