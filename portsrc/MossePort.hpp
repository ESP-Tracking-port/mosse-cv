//
// MosseDebug.hpp
//
// Created on: Sep 07, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MOSSEDEBUG_HPP_)
#define MOSSE_UTIL_MOSSEDEBUG_HPP_

#include "Types/Tracking.hpp"
#include <OhDebug.hpp>

#if defined(OHDEBUG)

# include "Types/Tracking.hpp"

namespace std {

template <class T>
std::ostream &operator<<(std::basic_ostream<T> &out, const Mosse::Tp::Roi &roi)
{
	return out << "Tp::Roi " << "row: " << roi.origin(0) << " col: " << roi.origin(1) << " rows: " << roi.size(0)
		<< " cols: " << roi.size(1);
}

template <class T>
std::ostream &operator<<(std::basic_ostream<T> &out, const Mosse::Tp::PointRowCol &point)
{
	return out << "Tp::PointRowCol " << "row: " << point(0) << " col: " << point(1);
}

}  // namespace std

#endif  // OH_DEBUG

#define MOSSE_USE_OPENCV 1  // There is a testing implementation of MOSSE that uses Open CV, particularly, Open CV's DFT (FFT) procedure. Set to 0 to enable portability

#endif // MOSSE_UTIL_MOSSEDEBUG_HPP_