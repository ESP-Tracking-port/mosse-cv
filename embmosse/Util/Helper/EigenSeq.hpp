//
// EigenSeq.hpp
//
// Created on: Sep 14, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_EIGENSEQ_HPP_)
#define MOSSE_UTIL_HELPER_EIGENSEQ_HPP_

#include <cstdint>

namespace Mosse {
namespace Tp {

class PointRowCol;

}  // namespace Tp

namespace Ut {

// Diagonal approximation

/// \brief Approximates diagonal view for a non-square matrix
///
class DiagonalRowsSeq {
public:
	DiagonalRowsSeq(const Tp::PointRowCol &aSize);

	inline std::size_t size() const
	{
		return sz;
	}

	inline std::size_t operator[](std::size_t index) const
	{
		return index;
	}
private:
	std::size_t sz;
};

class DiagonalColsSeq {
public:
	DiagonalColsSeq(const Tp::PointRowCol &aSize, bool mainDiagonal);
	std::size_t operator[](std::size_t index) const;

	inline std::size_t size() const
	{
		return sz(0);
	}
private:
	Tp::PointRowCol sz;
	bool main;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_EIGENSEQ_HPP_
