//
// ThreadedOps.hpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_UTIL_OPS_THREADEDOPS_HPP_)
#define MOSSE_UTIL_OPS_THREADEDOPS_HPP_

#include "Port/Task.hpp"
#include "Util/Helper/En.h"
#include "Util/Helper/Apply.hpp"
#include <tuple>
#include <cassert>

namespace Mosse {
namespace Tp {

class Image;
union NumVariant;

}  // namespace Tp

namespace Ut {

class DecomposedOps;

/// \brief Wraps an instance of Ops, provides an easy-to-use API to swap worker `Ops` methods. Enables multi-threaded
/// execution of `Ops` instances.
///
/// \warning Considering the number and the structure of inter-thread interactions, lock-free synchronization approach
/// suffices. But it must be revised, if this presupposition is no longer correct.
///
class ThreadedOps : public Port::Task {
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
		void c2();
		virtual void c3(int, char, bool, Tp::Image);
		static_assert(sizeof(&MethodPtrStub::c3) == sizeof(&MethodPtrStub::c1)
			&& sizeof(&MethodPtrStub::c0) == sizeof(&MethodPtrStub::c2), "");
	};

	union MethodPtrSzofMarker {
		decltype(&MethodPtrStub::c0) c0;
		decltype(&MethodPtrStub::c1) c1;
	};

	/// \brief Direct "pointer-to-member-pointer" cast is prohibited
	///
	template <class M>
	struct MethodWrapper {
		M method;
	};

	struct Storage {
		std::uint8_t args[sizeof(ArgsSzofMarker)];
		std::uint8_t method[sizeof(MethodPtrSzofMarker)];
		Result result;
	};

	using ExecutorType = void (ThreadedOps::*)();
public:
	inline const Result &result() const
	{
		return storage.result;
	}

	/// \brief Spinlock. Checks whether the current op is finished.
	///
	inline bool isDone() const
	{
		return nullptr == executorCb;
	}

	inline void requestStop()
	{
		shouldRun = false;
	}

	void run() override;
	ThreadedOps(DecomposedOps &);

	/// \brief Shortcut for creating compile time wrappers over the methods of `Ops`. It saves method pointer,
	/// arguments, and invokes those in a parallel thread using the instance of `Ops` provided during construction.
	/// See `exec`, `execImpl`, and `run` for more info
	///
	/// \tparam C method pointer
	/// \tparam Args arguments for the method
	template <class C, class ...Args>
	void setExec(C c, Args &&...aArgs)
	{
		using ReturnType = decltype((ops.*c)(std::forward<Args>(aArgs)...));
		new (storage.args) std::tuple<Args...>(aArgs...);
		new (storage.method) MethodWrapper<C>{c};
		executorCb = &ThreadedOps::exec<C, ReturnType, Args...>;
	}
private:

	template <class C, class R, class ...Args>
	inline void exec()
	{
		assert(isDone());
		execImpl<C, R, Args...>(nullptr);
		executorCb = nullptr;
	}

	template <class C, class R, class ...Args>
	void execImpl(En<!std::is_same<void, R>::value> = nullptr)
	{
		auto *methodWrapper = reinterpret_cast<MethodWrapper<C> *>(storage.method);
		auto &args = *reinterpret_cast<std::tuple<Args...> *>(storage.args);
		auto invokeCb =
			[methodWrapper, this, &args](Args ...aArgs)
			{
				return (ops.*(methodWrapper->method))(aArgs...);
			};
		storage.result = {R{apply(invokeCb, args)}};
	}

	template <class C, class R, class ...Args>
	void execImpl(En<std::is_same<void, R>::value> = nullptr)
	{
		auto *methodWrapper = reinterpret_cast<MethodWrapper<C> *>(storage.method);
		auto &args = *reinterpret_cast<std::tuple<Args...> *>(storage.args);
		auto invokeCb =
			[methodWrapper, this, &args](Args ...aArgs)
			{
				return (ops.*(methodWrapper->method))(aArgs...);
			};
		apply(invokeCb, args);
	}
private:
	Storage storage;
	DecomposedOps &ops;
	ExecutorType executorCb;  ///< Pointer to executor method. Also serves as a spinlock.
	bool shouldRun;  ///< Hook for ensuring thread termination
};

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_OPS_THREADEDOPS_HPP_
