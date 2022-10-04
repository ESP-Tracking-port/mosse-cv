//
// MosseDebug.hpp
//
// Created on: Sep 07, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_MOSSEDEBUG_HPP_)
#define MOSSE_UTIL_MOSSEDEBUG_HPP_

#undef OHDEBUG_DISABLE

#if MOSSE_PORTABLE
# define OHDEBUG_DISABLE  // "Ohdebug" special macro. Replace ohdebug API w/ stubs
#else
# define MOSSE_USE_OPENCV 1
#endif

#if !MOSSE_PORTABLE
# include "embmosse/Types/Tracking.hpp"
# include <cmath>
# include <opencv2/opencv.hpp>
# include <MallocCounter.hpp>
#endif  // MOSSE_PORTABLE
#include "embmosse/Types/Tracking.hpp"
#include <OhDebug.hpp>

#if MOSSE_PORT_USE_CASSERT
# include <cassert>
# define mosse_assert(...) assert(__VA_ARGS__)
#else
# define mosse_assert(...)
#endif

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
	return false;
}

}  // namespace Mosse

# define mosseassertnotnan(ctx, a, ...) ohdebugassert(ctx, !Mosse::isNan(a), ## __VA_ARGS__ )

#else
# define mosseassertnotnan(...)

struct MallocCounter {
	static constexpr unsigned getPeakCount()
	{
		return 0;
	}
};

#endif  // MOSSE_PORTABLE

#endif // MOSSE_UTIL_MOSSEDEBUG_HPP_
