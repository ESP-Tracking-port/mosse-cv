//
// Thread.hpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_PORTSRC_PORT_THREAD_HPP_)
#define MOSSE_PORTSRC_PORT_THREAD_HPP_

#include <memory>

namespace Mosse {
namespace Port {

class Task;

class Thread {
public:
	virtual std::unique_ptr<Thread> makeFromTask(Task &) = 0;
	virtual ~Thread() = default;

	/// \brief Starts the thread running the task it's been provided with
	///
	/// \pre The underlying representation must not expect that a tear down code (like that in FreeRTOS) will be called
	/// by MOSSE implementation. If any is requried, the task must be wrapped accordingly.
	///
	virtual void start() = 0;
	Thread();
protected:
	Thread(Task *aTask);
	Task *task();

	template <class T>
	static std::unique_ptr<Thread> makeUnique(Task &aTask)
	{
		return std::unique_ptr<Thread>{new T{&aTask}};
	}
private:
	Task *mtask;
};

}  // namespace Port
}  // namespace Mosse

#endif // MOSSE_PORTSRC_PORT_THREAD_HPP_
