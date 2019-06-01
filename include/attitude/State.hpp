#ifndef ATTITUDE_STATE_HPP_
#define ATTITUDE_STATE_HPP_

#include <iostream>

namespace attitude {

namespace state {

/**
 * @brief Possible cases for a slope according to the X data from the
 * accelerometer.
 *
 * TOP_HORIZONTAL case i
 */
enum CaseX {
	TOP_HORIZONTAL,
	TOP_VERTICAL,
	BOTTOM_VERTICAL,
	BOTTOM_HOTIZONTAL,
	ERROR
};

/**
 * @brief X resolution of the screen.
 *
 */
const uint16_t SCREEN_X = 128;

/**
 * @brief Y resolution of the screen.
 *
 */
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

/**
 * @brief Upper limit for Adc value from X.
 *
 */
const uint16_t ADC_X_UPPER_VALUE = ADC_X_MID_VALUE + ADC_X_MAX_VARIATION;

/**
 * @brief Case1(top horizontal) lower Adc value from X.
 *
 */
const uint16_t ADC_X_CASE1_LOWER_VALUE =
    ADC_X_MID_VALUE + ADC_X_MAX_VARIATION / 2;

/**
 * @brief Case2(top vertical) lower Adc value from X.
 *
 */
const uint16_t ADC_X_CASE2_LOWER_VALUE = ADC_X_MID_VALUE;

/**
 * @brief Case4(botton vertical) lower Adc value from X.
 *
 */
const uint16_t ADC_X_CASE4_LOWER_VALUE =
    ADC_X_MID_VALUE - ADC_X_MAX_VARIATION / 2;

/**
 * @brief Case8(botton horizontal) lower Adc value from X.
 *
 */
const uint16_t ADC_X_CASE8_LOWER_VALUE = ADC_X_MID_VALUE - ADC_X_MAX_VARIATION;

/**
 * @brief Lower limit for Adc value from X.
 *
 */
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
const uint16_t TAN_ADD_VALUES[SCREEN_MAX / 2] = {
    658,  658,  659,  659,  661,  662,  664,  667,  669,  672,  676,
    679,  683,  688,  692,  698,  703,  709,  716,  723,  730,  738,
    746,  755,  764,  774,  785,  796,  807,  820,  833,  847,  861,
    877,  893,  910,  928,  947,  967,  989,  1011, 1035, 1060, 1087,
    1115, 1145, 1177, 1211, 1246, 1284, 1325, 1368, 1414, 1463, 1515,
    1571, 1631, 1695, 1764, 1838, 1918, 2005, 2097, 2198};

struct linePoint {
	int8_t m_u8Value;
	linePoint* m_pNextPoint = NULL;
};
}  // namespace state

class State {
 public:
	/**
	 * @brief Construct a new State object.
	 *
	 * The State is constructed out of the accelerometer Z and Y(roll) data.
	 *
	 * @param i_u16AccelerometerZ The Z data from the accelerometer.
	 * @param i_u16AccelerometerX The X data from the accelerometer.
	 */
	State(uint16_t i_u16AccelerometerZ, uint16_t i_u16AccelerometerX);

	/**
	 * @brief Get the X value for the A point.
	 *
	 * The A point is the starting point, located on the left of the screen
	 *
	 * @return int32_t The X value for the A point.
	 */
	int32_t getPointAX();

	/**
	 * @brief Get the Y value for the A point.
	 *
	 * The A point is the starting point, located on the left of the screen
	 *
	 * @return int32_t The Y value for the A point.
	 */
	int32_t getPointAY();

	/**
	 * @brief Get the X value for the B point.
	 *
	 * The B point is the ending point, located on the left of the screen
	 *
	 * @return int32_t The X value for the B point.
	 */
	int32_t getPointBX();

	/**
	 * @brief Get the Y value for the B point.
	 *
	 * The B point is the ending point, located on the right of the screen.
	 *
	 * @return int32_t The Y value for the B point.
	 */
	int32_t getPointBY();

	/**
	 * @brief Get the value of M.
	 *
	 * @return uint8_t The value of M.
	 */
	uint8_t getM();

	/**
	 * @brief Get the value of B.
	 *
	 * @return uint16_t The value ofB.
	 */
	uint16_t getB();

	/**
	 * @brief Get the the case based on X data from the accelerometer.
	 *
	 * @return attitude::state::CaseX The case based on X data from the
	 * accelerometer.
	 */
	attitude::state::CaseX getCaseX(void);

	/**
	 * @brief Sends detailed info on the State via std::cout.
	 *
	 */
	void printInfo(bool i_bPrintHeader = false);

	/**
	 * @brief Set the Accelerometer Z data.
	 *
	 * @param i_u16AccelerometerZ The Accelerometer Z data.
	 */
	void setAccelerometerZ(uint16_t i_u16AccelerometerZ);

	/**
	 * @brief Set the Accelerometer X data.
	 *
	 * @param i_u16AccelerometerX The Accelerometer X data.
	 */
	void setAccelerometerX(uint16_t i_u16AccelerometerX);

	/**
	 * @brief Obtain the value of M.
	 *
	 * The value of M is calculated depending on the case of the state.
	 *
	 * @return uint8_t M.
	 */
	uint8_t calculateM(void);

	/**
	 * @brief Obtain the value of B.
	 *
	 * @return uint16_t B.
	 */
	uint16_t calculateB(void);

	/**
	 * @brief Set the value of M.
	 *
	 * @param i_u8M The value of M.
	 */
	void setM(uint8_t i_u8M);
	void setM(void);

	/**
	 * @brief Set the value of B.
	 *
	 * @param i_u16B The value of B.
	 */
	void setB(uint16_t i_u16B);
	void setB(void);

	/**
	 * @brief Get the Accelerometer Z data.
	 *
	 * @return uint16_t The Accelerometer Z data.
	 */
	uint16_t getAccelerometerZ();

	/**
	 * @brief Get the Accelerometer X data.
	 *
	 * @return uint16_t The Accelerometer X data.
	 */
	uint16_t getAccelerometerX();

	/**
	 * @brief Set the case for X data from accelerometer.
	 *
	 * @param i_eCaseX The case for the X data from the accelerometer.
	 */
	void setCaseX(attitude::state::CaseX i_eCaseX);
	void setCaseX(void);

	/**
	 * @brief Obtain the case based on X data from the accelerometer.
	 *
	 * @return attitude::state::CaseX The current case.
	 */
	attitude::state::CaseX calculateCaseX();
	// attitude::state::linePoint* getLineH(void);
	void getLineH(int32_t* p_lineH);

 private:
	/**
	 * @breif The slope of the State represented as a vector.
	 *
	 * The value M indicates the slope of the line in the following way.
	 *
	 * M is a line from the center of the screen to any boundary pixel on the
	 * right side of the screen ( [(64-127),0] or [127,(63-127)] or [127,(64-127)]
	 * or [(64,127), 127] ).
	 *
	 * The possible boundary pixel falls in one of four posible cases.

	 * <pre>
	 * BYTE
	 * 0  1  2  3  4  5  6  7
	 * X  X  X  X  X  X  X  X
	 * s0 s1 n0 n1 n2 n3 n4 n5
	 * </pre>
	 *
	 * s0-s1 determine the crossing point of M.
	 *
	 * s0 indicates if it is the upper(0) or lower(1) line.
	 *
	 * s1 indicates if it is an horizontal(0) or verical(1) line.
	 *
	 * <pre>
	 * s0s1 crossing
	 * 00   top horizontal.
	 * 01   top vertical.
	 * 10   bottom vertical.
	 * 11   bottom horizontal.
	 * </pre>
	 *
	 * n0-n5 is P.
	 *
	 * P is the offset of each case of crossing.
	 *
	 * <pre>
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
	 * </pre>
	 *
	 * Assumming the crossing of the following line is (68, 0).
	 *
	 * The M value is
	 * <pre>
	 * 0  1  2  3  4  5  6  7
	 * X  X  X  X  X  X  X  X
	 * s0 s1 n0 n1 n2 n3 n4 n5
	 * 0  0 (       68-64=4  )
	 * 0  0  0  0  0  1  0  0
	 * </pre>
	 *
	 */
	uint8_t m_u8M;

	/**
	 * @brief The B value of the State indicates the height of the line at the
	 * X center of the screen( points ([63,64],[0-127]) ).
	 *
	 * B is obtained directly from the accelerometer Z data.
	 */
	uint16_t m_u16B;

	/**
	 * @brief The stored value of Z data from accelerometer.
	 *
	 */
	uint16_t m_u16AccelerometerZ;

	/**
	 * @brief The stored value of X data from accelerometer.
	 *
	 */
	uint16_t m_u16AccelerometerX;

	/**
	 * @brief The case of the M value depending on the X data from the
	 * accelerometer.
	 *
	 */
	attitude::state::CaseX m_eCaseX;

	/**
	 * @brief Indicates if the tan values have been calculated.
	 *
	 */
	static bool m_bHasCalculatedScaledTanValues;

	/**
	 * @brief The calculated values of tangent steps from 0° to 45°.
	 *
	 * The quantity of steps is half the size of the maximum screen resolution.
	 */
	static uint16_t m_aScaledTanValues[attitude::state::SCREEN_MAX / 2];

	/**
	 * @brief The sum of all the values in m_aScaledTanValues.
	 *
	 * The sum of the elements in the array should equal the representation of
	 * 45° rotation from the accelerometer X data.
	 */
	static uint16_t m_u16ScaledTanValuesSum;

	/**
	 * @brief Calculate the pixels offset from the horizontal or vertical center
	 * of the screen for the state.
	 *
	 * @return uint16_t The pixel offset from the horizontal or verical center of
	 * the screen.
	 */
	uint16_t getPixelOffsetFromAccelerometerX(void);

	/**
	 * @brief Get the scaled tan value for a step.
	 *
	 * @param i_u16Index The index of the step.
	 * @return uint16_t The scaled tan value.
	 */
	uint16_t getScaledTanValue(uint16_t i_u16Index);

	/**
	 * @brief Set the Scaled Tan Value object
	 *
	 * @param i_u16ScaledTanValue
	 * @param i_u16Index
	 */
	void setScaledTanValue(uint16_t i_u16ScaledTanValue, uint16_t i_u16Index);

	/**
	 * @brief Set the scaled tan array values sum.
	 *
	 * @param i_u16ScaledTanValuesSum The scaled tan array values sum.
	 */
	void setScaledTanValuesSum(uint16_t i_u16ScaledTanValuesSum);

	/**
	 * @brief Get the scaled tan array values sum.
	 *
	 * @return uint16_t The scaled tan array values sum.
	 */
	uint16_t getScaledTanValuesSum(void);

	/**
	 * @brief Scale the tan values to the screen metrics.
	 *
	 */
	void scaleTanValues(void);
};

}  // namespace attitude

#endif /* ATTITUDE_STATE_HPP_ */
