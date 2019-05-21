#ifndef ATTITUDE_ATTITUDEINDICATOR_HPP_
#define ATTITUDE_ATTITUDEINDICATOR_HPP_

#include "msp.h"

#include "mkii/Accelerometer.hpp"

namespace attitude {

namespace attitudeIndicator {

/**
 * The state is defined by B and M, the equation for the attitude indicator line
 * is:
 * y(x) = Mx+B
 */
struct State {
	uint8_t m_u8B;
	uint8_t m_u8M;
	bool m_bLeftToRightRise;
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
 * Noting the Y axis is inverted in the screen.
 *
 * y(x) = M * (Xmax/2 - x) - Y
 */
class AttitudeIndicator {
 public:
	AttitudeIndicator(mkii::Accelerometer* i_pAccelerometer);

	/**
	 * Obtain the Y position of the horizon line.
	 *
	 * @return uint8_t
	 */
	uint8_t getM(void);

	/**
	 * Obtain the Y position of the horizon line.
	 *
	 * @return uint8_t
	 */
	uint8_t getY(void);

	/**
	 * @brief Compares the latest state to the current state and returns the area
	 * in pixels that are different.
	 *
	 * @return uint16_t The quantity of pixels that are different between the
	 * latest and current state.
	 */
	uint16_t getRepaintArea(void);

	/**
	 * @brief Sets the latest state from the adc data.
	 */
	void updateLatestState(uint16_t);

	/**
	 * @brief Sets the latest state as the current state and update the latest
	 * state.
	 */
	void updateState(void);

 private:
	/**
	 * Pointer to the accelerometer used to determine the state.
	 *
	 */
	mkii::Accelerometer* m_pAccelerometer;

	void setAccelerometer(mkii::Accelerometer* i_pAccelerometer);

	/**
	 * @brief The latest state is the newer state for the acquired accelerometer
	 * data. The latest state is not necessarily painted to screen.
	 */
	attitude::attitudeIndicator::State m_stLatestState;

	/**
	 * @brief The state currently displayed on the screen.
	 *
	 */
	attitude::attitudeIndicator::State m_stCurrentState;
};

}  // namespace attitude

#endif /* ATTITUDE_ATTITUDEINDICATOR_HPP_ */
