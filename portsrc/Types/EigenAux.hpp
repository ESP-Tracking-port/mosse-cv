//
// EigenAux.hpp
//
// Created on: Aug 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_TYPES_EIGENAUX_HPP_)
#define MOSSE_TYPES_EIGENAUX_HPP_

#include "Types/Repr.hpp"
#include "Util/Arithm/Conv.hpp"
#include "Util/Arithm/MemLayout.hpp"
#include <Eigen/Core>

namespace Mosse {
namespace Tp {

template <Repr::Flags F>
struct EigenMapType {
	using ValueType = Tp::Repr::Type<F>;
	using MatrixType = typename Eigen::Matrix<ValueType, Eigen::Dynamic, Eigen::Dynamic>;
	using StrideType = typename Eigen::Stride<Ut::strideInner<F>(), Eigen::Dynamic>;
	using Type = Eigen::Map<MatrixType, Eigen::Unaligned, StrideType>;
};

}  // namespace Tp
}  // namespace Mosse

#endif // MOSSE_TYPES_EIGENAUX_HPP_
