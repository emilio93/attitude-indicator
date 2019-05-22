#ifndef ATTITUDE_STATE_HPP_
#define ATTITUDE_STATE_HPP_

#include <iostream>

namespace attitude {

namespace state {

enum CaseX {
	TOP_HORIZONTAL,
	TOP_VERTICAL,
	BOTTOM_VERTICAL,
	BOTTOM_HOTIZONTAL,
	ERROR
};

const uint16_t SCREEN_X = 128;
const uint16_t SCREEN_Y = 128;

/**
 * @brief Max value out of SCREEN_X and SCREEN_Y.
 *
 */
const uint16_t SCREEN_MAX = 128;

/**
 * @brief The adc measurement when the Z measure is in the middle.
 *
 */
const uint16_t ADC_Z_MID_VALUE = 8192;

/**
 * @brief The measurement diference from the middle to one of the extremes.
 *
 */
const uint16_t ADC_Z_MAX_VARIATION = 3308;

/**
 * @brief The factor by which the measurement needs to be divided in order to
 * map to the lcd screen.
 *
 * Formula is ADX_Z_SCREEN_FACTOR = 2*ADX_Z_MAX_VARIATTION/ScreenResolution
 *
 * For a 128 resolution screen
 * ADX_Z_SCREEN_FACTOR = 2/128*ADX_Z_MAX_VARIATTION
 * ADX_Z_SCREEN_FACTOR = 1/64*ADX_Z_MAX_VARIATTION
 * ADX_Z_SCREEN_FACTOR = ADX_Z_MAX_VARIATTION>>6;
 */
const uint16_t ADC_Z_SCREEN_FACTOR = ADC_Z_MAX_VARIATION >> 6;

/**
 * @brief The adc measurement when the X measure is in the middle.
 *
 */
const uint16_t ADC_X_MID_VALUE = 8192;

/**
 * @brief The measurement difference from the middle to one of the extremes.
 *
 */
const uint16_t ADC_X_MAX_VARIATION = 3308;

const uint16_t ADC_X_UPPER_VALUE = ADC_X_MID_VALUE + ADC_X_MAX_VARIATION;

const uint16_t ADC_X_CASE1_LOWER_VALUE =
    ADC_X_MID_VALUE + ADC_X_MAX_VARIATION / 2;

const uint16_t ADC_X_CASE2_LOWER_VALUE = ADC_X_MID_VALUE;

const uint16_t ADC_X_CASE4_LOWER_VALUE =
    ADC_X_MID_VALUE - ADC_X_MAX_VARIATION / 2;

const uint16_t ADC_X_CASE8_LOWER_VALUE = ADC_X_MID_VALUE - ADC_X_MAX_VARIATION;

const uint16_t ADC_X_LOWER_VALUE = ADC_X_MID_VALUE - ADC_X_MAX_VARIATION;

/**
 * @brief The sum of TAN_ADD_VALUES.
 *
 */
const uint16_t TAN_ADD_VALUES_SUM = 0xffff;
/**
 * @brief Tangent steps for 64 steps adding up to TAN_ADD_VALUES_SUM.
 *
 */
const uint16_t TAN_ADD_VALUES[] = {
    658,  658,  659,  659,  661,  662,  664,  667,  669,  672,  676,
    679,  683,  688,  692,  698,  703,  709,  716,  723,  730,  738,
    746,  755,  764,  774,  785,  796,  807,  820,  833,  847,  861,
    877,  893,  910,  928,  947,  967,  989,  1011, 1035, 1060, 1087,
    1115, 1145, 1177, 1211, 1246, 1284, 1325, 1368, 1414, 1463, 1515,
    1571, 1631, 1695, 1764, 1838, 1918, 2005, 2097, 2198};
}  // namespace state

class State {
 public:
	State(uint16_t i_u16AccelerometerZ, uint16_t i_u16AccelerometerX);

	int32_t getPointAX();
	int32_t getPointAY();

	int32_t getPointBX();
	int32_t getPointBY();

	void printInfo();

 private:
	/**
	 *
	 * The value M indicates the slope of the line in the following way.
	 * M is a line from the center of the screen to any boundary pixel of the
	 * right side of the screen ( [(64-127),0] or [127,(63-127)] or [127,(64-127)]
	 * or [(64,127), 127] ).
	 *
	 * The possible boundary pixel falls in one of four posible cases.

	 *
	 * BYTE
	 * 0  1  2  3  4  5  6  7
	 * X  X  X  X  X  X  X  X
	 * s0 s1 n0 n1 n2 n3 n4 n5
	 *
	 * s0-s1 determine the crossing point of M.
	 *
	 * s0 indicates if it is the upper(0) or lower(1) line.
	 * s1 indicates if it is an horizontal(0) or verical(1) line.
	 *
	 * n0-n5 is P
	 *
	 * P is the offset of each case of crossing
	 *
	 * upper horizontal
	 *  0    ->       64
	 *  +------------+
	 *      /        |
	 *     /         |
	 *    /          |
	 *   /           | ^
	 *  +------------+ 0
	 *               | \/
	 *               |    lower
	 *               |    vertical
	 *               |
	 *  +------------+
	 *  0    ->       64
	 *
	 * Assumming the crossing of the following line is (68, 0).
	 *
	 * The M value is
	 * 0  1  2  3  4  5  6  7
	 * X  X  X  X  X  X  X  X
	 * s0 s1 n0 n1 n2 n3 n4 n5
	 * 0  0 (       68-64=4  )
	 *
	 *
	 */
	uint16_t m_u16M;

	uint16_t m_u16B;

	uint16_t m_u16AccelerometerZ;
	uint16_t m_u16AccelerometerX;

	static bool m_bHasCalculatedScaledTanValues;
	static uint16_t m_aScaledTanValues[attitude::state::SCREEN_MAX / 2];
	uint16_t m_u16ScaledTanValuesSum;

	uint16_t calculateB(void);

	attitude::state::CaseX getAccelerometerXCase();

	void setM(uint16_t i_u16M);
	void setB(uint16_t i_u16B);

	uint16_t getM();
	uint16_t getB();

	uint16_t getAccelerometerZ();
	uint16_t getAccelerometerX();
	void setAccelerometerZ(uint16_t i_u16AccelerometerZ);
	void setAccelerometerX(uint16_t i_u16AccelerometerX);

	uint16_t getPixelOffsetFromAccelerometerX(void);

	uint16_t getScaledTanValue(uint16_t i_u16Index);
	void setScaledTanValue(uint16_t i_u16ScaledTanValue, uint16_t i_u16Index);

	void setScaledTanValuesSum(uint16_t i_u16ScaledTanValuesSum);
	uint16_t getScaledTanValuesSum(void);

	void scaleTanValues(void);
};

}  // namespace attitude

#endif /* ATTITUDE_STATE_HPP_ */
