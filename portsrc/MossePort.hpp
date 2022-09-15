//
// MosseDebug.hpp
//
// Created on: Sep 07, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MOSSEDEBUG_HPP_)
#define MOSSE_UTIL_MOSSEDEBUG_HPP_

#define MOSSE_PORTABLE 0  // If 0, desktop-only features like console debug output or OpenCV modules will be compiled
#define OHDEBUG_DISABLE

#if MOSSE_PORTABLE
# define OHDEBUG_DISABLE  // "Ohdebug" special macro. Replace ohdebug API w/ stubs
#else
# define MOSSE_USE_OPENCV 1
#endif

#if !MOSSE_PORTABLE
# include <cassert>
# include <cmath>
# include <opencv2/opencv.hpp>
# include "Types/Tracking.hpp"
#endif  // MOSSE_PORTABLE
#include "Types/Tracking.hpp"
#include <OhDebug.hpp>

// Disable certain output groups

#if !MOSSE_PORTABLE

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


namespace Mosse {

inline bool isNan(float a)
{
	return std::isnan(a);
}

inline bool isNan(...)
{
	ohdebug(Mosse::isNan, "stub, fallback");
	return false;
}

}  // namespace Mosse

# define mosseassertnotnan(ctx, a, ...) ohdebugassert(ctx, !Mosse::isNan(a), ## __VA_ARGS__ )

#else
# define mosseassertnotnan(...)
#endif  // MOSSE_PORTABLE

#endif // MOSSE_UTIL_MOSSEDEBUG_HPP_
