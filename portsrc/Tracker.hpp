//
// Tracker.hpp
//
// Created on: Aug 25, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#if !defined(MOSSE_TRACKER_HPP_)
#define MOSSE_TRACKER_HPP_

#include <array>
#include <Eigen/Core>

namespace Mosse {
namespace Tp {

class Image;
class Roi;
class PointRowCol;

}  // namespace Tp

namespace Ut {

class Port;

}  // namespace Ut

class Tracker {
private:
	struct Tracking {
		Tp::Roi roi;
		float psr;
	};
public:
	Tracker(Ut::Port port);
	virtual ~Tracker() = default;
	void init(Tp::Image, Tp::Roi);
	void update(Tp::Image, bool updatePsr);
	const Tp::Roi &roi() const;
	float lastPsr() const;
protected:
	Tracking tracking;
	Ut::Port port;
};

}  // namespace Mosse

#endif // MOSSE_TRACKER_FWD_
