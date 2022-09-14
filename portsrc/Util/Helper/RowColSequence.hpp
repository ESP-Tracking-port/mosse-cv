//
// RowColSequence.hpp
//
// Created on: Sep 14, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_HELPER_ROWCOLSEQUENCE_HPP_)
#define MOSSE_HELPER_ROWCOLSEQUENCE_HPP_

#include <cstdint>

namespace Mosse {
namespace Tp {

class PointRowCol;

}  // namespace Tp

namespace Ut {

class RowColSequence {
public:
	class Iterator {
		friend class RowColSequence;
		friend bool operator==(const Iterator &aLhs, const Iterator &aRhs);
	public:
		Iterator &operator++();
		Tp::PointRowCol *operator*();
	private:
		Iterator(RowColSequence &);
		Iterator();
	private:
		RowColSequence *owner;
		std::size_t index;
		Tp::PointRowCol point;
	};
public:
	Iterator begin();
	static Iterator end();
private:
	virtual void pointInit(Tp::PointRowCol &prev) = 0;
	virtual bool pointAdvance(std::size_t index, Tp::PointRowCol &prev) = 0;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_HELPER_ROWCOLSEQUENCE_HPP_
