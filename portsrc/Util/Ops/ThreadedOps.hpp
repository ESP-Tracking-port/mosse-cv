//
// ThreadedOps.hpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_OPS_THREADEDOPS_HPP_)
#define MOSSE_UTIL_OPS_THREADEDOPS_HPP_

#include "Util/Helper/En.h"
#include "Util/Helper/Apply.hpp"
#include <tuple>

namespace Mosse {
namespace Tp {

class Image;
union NumVariant;

}  // namespace Tp

namespace Ut {

class Ops;

class ThreadedOps {
public:
	union Result {
		Tp::NumVariant numVariant;
	};
private:

	/// \brief Being used as a marker to determine the size of the storage. It only covers argument sets w/ the max.
	/// argument numbers.
	///
	union ArgsSzofMarker {
		std::tuple<void *, void *, void*> argsImageConvFftDomain;
		std::tuple<Tp::Image, void *> argsImageCropInto;
		std::tuple<Tp::Image, void *, Tp::NumVariant, Tp::NumVariant> argsImagePreprocessImpl;
	};

	struct MethodPtrStub {
		void c0(int, void *, char, Tp::Image);
		virtual void c1(Tp::Image) = 0;
	};

	union MethodPtrSzofMarker {
		decltype(&MethodPtrStub::c0) c0;
		decltype(&MethodPtrStub::c1) c1;
	};

	struct Storage {
		std::uint8_t args[sizeof(ArgsSzofMarker)];
		std::uint8_t result[sizeof(Result)];
		std::uint8_t method[sizeof(MethodPtrSzofMarker)];
		// TODO: (XXX) static assert checking that all method pointers have the same `sizeof(...`)
	};

	using ExecutorType = void (ThreadedOps::*)();
public:
	inline const void *result() const
	{
		return reinterpret_cast<const void *>(storage.result);
	}

	ThreadedOps(Ops &);

	/// \tparam C method pointer
	/// \tparam Args arguments for the method
	template <class C, class ...Args>
	void setExec(C c, Args &&...aArgs)
	{
		using ReturnType = decltype((ops.*c)(std::forward<Args>(aArgs)...));
		new (storage.args) std::tuple<Args...>(aArgs...);
		new (storage.method) C{c};
		executorCb = &ThreadedOps::exec<C, ReturnType, Args...>;
	}
private:

	template <class C, class R, class ...Args>
	inline void exec()
	{
		execImpl<C, R, Args...>(nullptr);
	}

	template <class C, class R, class ...Args>
	void execImpl(En<!std::is_same<void, R>::value> = nullptr)
	{
		auto methodPtr = reinterpret_cast<C>(storage.method);
		auto &args = *reinterpret_cast<std::tuple<Args...> *>(storage.args);
		auto invokeCb =
			[methodPtr, this, &args](Args ...aArgs)
			{
				return (ops.*methodPtr)(aArgs...);
			};
		// TODO test apply
		new (storage.result) R{apply(invokeCb, args)};
	}

	template <class C, class R, class ...Args>
	void execImpl(En<std::is_same<void, R>::value> = nullptr)
	{
		auto methodPtr = reinterpret_cast<C>(storage.method);
		auto &args = *reinterpret_cast<std::tuple<Args...> *>(storage.args);
		auto invokeCb =
			[methodPtr, this, &args](Args ...aArgs)
			{
				(ops.*methodPtr)(aArgs...);
			};
		apply(invokeCb, args);
	}
private:
	Storage storage;
	Ops &ops;
	ExecutorType executorCb;
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_OPS_THREADEDOPS_HPP_
