//
// OsApi.hpp
//
// Created on: Oct 04, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_PORT_OSAPI_HPP_)
#define MOSSE_PORT_OSAPI_HPP_

namespace Mosse {
namespace Port {

class OsApi {
private:
	struct TaskYield {
		bool enable;
	};
public:
	/// \brief More fine-tuned (conditional) approach to whether the control should be yielded. Enables to retain the
	/// control for the duration of frame processing
	///
	void taskYieldMinDelayWrap();

	/// \brief Enables / Disables yield
	///
	inline void setTaskYieldMinDelayFlag(bool aShouldYield)
	{
		taskYield.enable = aShouldYield;
	}

	/// \brief Yield the control to another task for the shortest possible period of time, when a current task has nothing
	/// useful to do apart from waiting for certain conditions to be met.
	///
	/// \details Use case: to prevent watchdog triggering in systems like FreeRTOS
	///
	virtual void taskYieldMinDelay() = 0;

	static OsApi *instance();
protected:
	OsApi() = default;
	OsApi(const OsApi &) = delete;
	OsApi(OsApi &&) = delete;
	OsApi &operator=(const OsApi &) = delete;
	OsApi &operator=(OsApi &&) = delete;
	static void setInstance(OsApi &aInstance);
private:
	static OsApi *inst;
	TaskYield taskYield = {true};
};

class StubOsApi : public OsApi {
public:
	StubOsApi();
	void taskYieldMinDelay() override;
};

}  // namespace Port
}  // namespace Mosse

#endif // MOSSE_PORT_OSAPI_HPP_
