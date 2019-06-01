#ifndef TASK_REFRESHSCREENROLLINDICATOR_HPP_
#define TASK_REFRESHSCREENROLLINDICATOR_HPP_

#include <cstddef>

#include <ti/grlib/grlib.h>

#include "mkii/Accelerometer.hpp"
#include "peripheral/LcdScreen.hpp"

#include "scheduler/Task.hpp"

namespace task {

/**
 * @brief The adc measurement when the X measure is in the middle.
 *
 */
const uint16_t ADC_X_MID_VALUE = 8192;

/**
 * @brief The measurement difference from the middle to extremes.
 *
 */
const uint16_t ADC_X_MAX_VARIATION = 3308;
const uint16_t ADC_X_UPPER_VALUE = ADC_X_MID_VALUE + ADC_X_MAX_VARIATION;
const uint16_t ADC_X_LOWER_VALUE = ADC_X_MID_VALUE - ADC_X_MAX_VARIATION;

const uint16_t DRAWLINERAW[] = {
    16, 36, 7,  31, 112, 36, 121, 31, 24, 24, 17, 17, 104, 24, 111, 17,
    36, 16, 31, 7,  92,  16, 97,  7,  35, 16, 31, 8,  93,  16, 97,  8,
    54, 9,  54, 4,  74,  9,  74,  4,  45, 11, 43, 5,  83,  11, 85,  5};

class RefreshScreenRollIndicator : public scheduler::Task {
 public:
	RefreshScreenRollIndicator(peripheral::LcdScreen* i_pLcdScreen,
	                           Graphics_Context* i_pContextA,
	                           Graphics_Context* i_pContextC);

	/**
	 * Refresh the area that has changed.
	 *
	 * @return uint8_t Return code.
	 */
	virtual uint8_t run(void);

	/**
	 * Set up the LcdScreen.
	 *
	 * @return uint8_t Return code.
	 */
	virtual uint8_t setup(void);

	/**
	 * Set the LcdScreen object.
	 *
	 * @param i_pLcdScreen The LcdScreen object.
	 */
	void setLcdScreen(peripheral::LcdScreen* i_pLcdScreen);

	/**
	 * Get the LcdScreen object.
	 *
	 * @return peripheral::LcdScreen* The LcdScreen object.
	 */
	peripheral::LcdScreen* getLcdScreen(void);

	/**
	 * Set the Context data.
	 *
	 * @param i_stContext The Context data.
	 */
	void setContextCircle(Graphics_Context* i_stContext);

	/**
	 * Set the Context data.
	 *
	 * @param i_stContext The Context data.
	 */
	void setContextArrow(Graphics_Context* i_stContext);

	/**
	 * Get the Context data.
	 *
	 * @return Graphics_Context The Context data.
	 */
	Graphics_Context* getContextCircle(void);

	/**
	 * Get the Context data.
	 *
	 * @return Graphics_Context The Context data.
	 */
	Graphics_Context* getContextArrow(void);

	void repaintRollIndicator(void);
	void paintArrow_0(uint16_t, uint16_t);
	void paintArrow_30(uint16_t, uint16_t);
	void paintArrow_60(uint16_t, uint16_t);

 protected:
 private:
	/**
	 * Data from accelerometer.
	 */
	uint16_t m_u16X;
	//	uint16_t m_u16Y;
	// uint16_t m_u16Z;

	/**
	 * Previous data from accelerometer.
	 */
	uint16_t m_u16OldZValue;

	/**
	 * The graphics context in which is being draw.
	 */
	static Graphics_Context* m_pContextCircle;
	static Graphics_Context* m_pContextArrow;

	/**
	 * The LcdScreen Object.
	 */
	peripheral::LcdScreen* m_pLcdScreen;
};

}  // namespace task

#endif /* TASK_REFRESHSCREENBACKGROUND_HPP_ */
