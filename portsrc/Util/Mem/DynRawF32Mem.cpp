//
// DynRawF32Mem.cpp
//
// Created on: Sep 05, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Types/Repr.hpp"
#include "Types/Tracking.hpp"
#include "DynRawF32Mem.hpp"

namespace Mosse {
namespace Ut {

constexpr auto kDynRawflReprBuffer = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw | Tp::Repr::CplxRe1Im1;
constexpr auto kDynRawflReprHannMatrix = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kDynRawflReprAb = Tp::Repr::StorageI16 | Tp::Repr::ReprLog2;
constexpr auto kDynRawflReprAbDivIntermediary = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;
constexpr auto kDynRawflReprGauss = Tp::Repr::StorageF32 | Tp::Repr::ReprRaw;

DynRawF32Mem::DynRawF32Mem() : Mem{}, roiSizePrev{{0, 0}, {0, 0}}, mata{}, matb{}, buf{}
{
}

void *DynRawF32Mem::matA()
{
	return mata.data();
}

void *DynRawF32Mem::matB()
{
	return matb.data();
}

void *DynRawF32Mem::buffer()
{
	return buf.data();
}

void DynRawF32Mem::init(Tp::Roi aRoi)
{
	if (roiSizePrev != aRoi.size) {
		mata.reserve(sizeof(float) * 2 * aRoi.area());
		mata.reserve(sizeof(float) * 2 * aRoi.area());
		buf.reserve(sizeof(float) * 2 * aRoi.area());
		roiSizePrev = aRoi.size;
	}
}

}  // namespace Ut
}  // namespace Mosse
