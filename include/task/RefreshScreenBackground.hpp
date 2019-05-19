#ifndef TASK_REFRESHSCREENBACKGROUND_HPP_
#define TASK_REFRESHSCREENBACKGROUND_HPP_

#include <cstddef>

#include <ti/grlib/grlib.h>
extern "C" {
#include <Crystalfontz128x128_ST7735.h>
}

#include "mkii/LcdScreen.hpp"

#include "scheduler/Task.hpp"

namespace task {

class RefreshScreenBackground : public scheduler::Task {
 public:
	RefreshScreenBackground(mkii::LcdScreen* i_pLcdScreen,
	                        Graphics_Context i_stContext);

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
	void setLcdScreen(mkii::LcdScreen* i_pLcdScreen);

	/**
	 * Get the LcdScreen object.
	 *
	 * @return mkii::LcdScreen* The LcdScreen object.
	 */
	mkii::LcdScreen* getLcdScreen(void);

	/**
	 * Set the Context data.
	 *
	 * @param i_stContext The Context data.
	 */
	void setContext(Graphics_Context i_stContext);

	/**
	 * Get the Context data.
	 *
	 * @return Graphics_Context The Context data.
	 */
	Graphics_Context getContext(void);

 protected:
 private:
	/**
	 * Data from accelerometer.
	 */
	uint16_t m_u16X;
	uint16_t m_u16Y;
	uint16_t m_u16Z;

	/**
	 * Previous data from accelerometer.
	 */
	uint16_t m_u16OldZValue;

	/**
	 * The graphics context in which is being draw.
	 */
	Graphics_Context m_stContext;

	/**
	 * The LcdScreen Object.
	 */
	mkii::LcdScreen* m_pLcdScreen;
};

}  // namespace task

#endif /* TASK_REFRESHSCREENBACKGROUND_HPP_ */
