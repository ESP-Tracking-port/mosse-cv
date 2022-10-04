//
// Task.hpp
//
// Created on: Sep 19, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(PORTSRC_PORT_TASK_HPP_)
#define PORTSRC_PORT_TASK_HPP_

namespace Mosse {
namespace Port {

struct Task {
	virtual void run() = 0;
};

}  // namespace Port
}  // namespace Mosse

#endif // PORTSRC_PORT_TASK_HPP_
