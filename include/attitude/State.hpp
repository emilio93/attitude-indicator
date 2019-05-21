#ifndef ATTITUDE_STATE_HPP_
#define ATTITUDE_STATE_HPP_

#include <iostream>

namespace attitude {

namespace state {

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

	uint8_t getM();
	uint8_t getB();

	uint16_t getAccelerometerZ();
	uint16_t getAccelerometerX();

	uint8_t getPixelOffsetFromAccelerometerX(void);
	uint16_t getScaledTanValue(uint8_t i_u8Index);
	uint8_t getAccelerometerXCase();

 private:
	/**
	 * @brief M is the crossing site along the border of the screen starting
	 * on the center top all through the center bottom clockwise.
	 * The value on the center top is 0 and the value on the center bottom is
	 * 255.
	 */
	uint8_t m_u8M;
	uint8_t m_u8B;

	uint16_t m_u16AccelerometerZ;
	uint16_t m_u16AccelerometerX;

	uint16_t m_aScaledTanValues[attitude::state::SCREEN_MAX / 2];
	uint16_t m_u16ScaledTanValuesSum;

	void setM(uint8_t i_u8M);
	void setB(uint8_t i_u8B);

	void setAccelerometerZ(uint16_t i_u16AccelerometerZ);
	void setAccelerometerX(uint16_t i_u16AccelerometerX);

	void setScaledTanValue(uint16_t i_u16ScaledTanValue, uint8_t i_u8Index);

	void setScaledTanValuesSum(uint16_t i_u16ScaledTanValuesSum);
	uint16_t getScaledTanValuesSum(void);

	void scaleTanValues(void);
};

}  // namespace attitude

#endif /* ATTITUDE_STATE_HPP_ */
