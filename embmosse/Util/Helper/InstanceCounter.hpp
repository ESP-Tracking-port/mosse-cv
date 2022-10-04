//
// InstanceCounter.hpp
//
// Created on: Sep 21, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_HELPER_INSTANCECOUNTER_HPP_)
#define MOSSE_UTIL_HELPER_INSTANCECOUNTER_HPP_

namespace Mosse {
namespace Ut {

template <class T>
struct InstanceCounter {
	static decltype(sizeof(int)) counter;
	decltype(sizeof(int)) instanceNum;

	InstanceCounter() : instanceNum{counter++}
	{
	}
};

template <class T>
decltype(sizeof(int)) InstanceCounter<T>::counter = 0;

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_HELPER_INSTANCECOUNTER_HPP_
