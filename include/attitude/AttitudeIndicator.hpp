#ifndef ATTITUDE_ATTITUDEINDICATOR_HPP_
#define ATTITUDE_ATTITUDEINDICATOR_HPP_

#include "msp.h"

#include "mkii/Accelerometer.hpp"

namespace attitude {

namespace attitudeIndicator {

/**
 * The quantity of previous states to track.
 */
const uint8_t PREV_STATE_COUNT = 5;

/**
 * The state is defined by B and M, the equation for the attitude indicator line
 * is:
 * y(x) = Mx+B
 *
 * x in [0, 127]
 *
 * Example, Y is 60 and M is 0
 * y(x) = 60
 * Example, Y is 50 and M is -6
 * y(x) = -6x + 50
 * y(x=0) =
 *
 * However, this needs to be discretized.
 */
struct State {
	uint8_t m_u8B, uint8_t m_u8M, bool m_bLeftToRightRise
};

}  // namespace attitudeIndicator

/**
 * The AttitudeIndicator is the full representation of the attitude indicator.
 *
 * The state of the indicator consists of a point along the center of the screen
 * and a degree of inclination.
 *
 * The state is determined by a Y position that indicates a point along a
 * vertical line in the center of the screen, an M inclination and a inclination
 * direction(sign of M).
 *
 * For a screen of  (Xmax, Ymax) pixels.
 *
 * y(x) = M * x + b(Y)
 *
 * So we want to obtain b from Y:
 *
 * y(Xmax/2) = Y = M * Xmax/2 + b
 * b = Y - M * Xmax/2
 *
 * And finally redifine for y(x)
 *
 * y(x) = M * x + Y - M * Xmax/2
 * y(x) = M * (x - Xmax/2) + Y
 *
 * This is the equation for the horizon line.
 *
 * y(x) = M * (x - Xmax/2) + Y
 *
 *  _________________
 * |                 |
 * |                 |
 * |________.________|
 * |                 |
 * |_|_|..|_|_|..|_|_|
 * |_|_|..|_|_|..|_|_|
 * 0      63 64     127
 *
 *  _________________
 * |________.________|
 * |                 |
 * |                 |
 * |                 |
 * |_|_|..|_|_|..|_|_|
 * |_|_|..|_|_|..|_|_|
 * 0      63 64     127
 *
 *  _________________
 * |           /     |
 * |          /      |
 * |        ./       |
 * |       /         |
 * |_|_|..|_|_|..|_|_|
 * |_|_|..|_|_|..|_|_|
 * 0      63 64     127
 *
 *
 * How to track changes
 * Having current state and previous state makes it easy.
 *
 *
 *
 *
 */
class AttitudeIndicator {
 public:
	AttitudeIndicator(mkii::Accelerometer* i_pAccelerometer);

	/**
	 * Obtain the Y position of the horizon line.
	 *
	 * @return uint8_t
	 */
	uint8_t getY(void);

	/**
	 * Set the Y position of the horizon line
	 *
	 * @return uint8_t
	 */
	uint8_t setY(void);

 private:
	/**
	 * Pointer to the accelerometer used to determine the state.
	 *
	 */
	mkii::Accelerometer* m_pAccelerometer;

	/**
	 * The Y position of the horizon line.
	 *
	 */
	uint8_t m_u8Y;
};

}  // namespace attitude

#endif /* ATTITUDE_ATTITUDEINDICATOR_HPP_ */
