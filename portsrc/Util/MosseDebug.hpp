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

//static_assert(false, "");

# include "Types/Tracking.hpp"

namespace std {

template <class T>
std::ostream &operator<<(std::basic_ostream<T> &out, const Mosse::Tp::Roi &roi)
{
	return out << "Tp::Roi " << "row: " << roi.origin(0) << " col: " << roi.origin(1) << " rows: " << roi.size(0)
		<< " cols: " << roi.size(1);
}

}  // namespace std

#endif  // OH_DEBUG

#endif // MOSSE_UTIL_MOSSEDEBUG_HPP_
