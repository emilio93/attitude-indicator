#ifndef TASK_REFRESHSCREENBACKGROUND_HPP_
#define TASK_REFRESHSCREENBACKGROUND_HPP_

#include <cstddef>

#include <ti/grlib/grlib.h>

#include "mkii/Accelerometer.hpp"
#include "peripheral/LcdScreen.hpp"

#include "attitude/State.hpp"
#include "scheduler/Task.hpp"

namespace task {

class RefreshScreenBackground : public scheduler::Task {
 public:
	RefreshScreenBackground(attitude::State* o_pState,
	                        peripheral::LcdScreen* i_pLcdScreen,
	                        Graphics_Context* i_pContext);

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
	void setContext(Graphics_Context* i_stContext);

	/**
	 * Get the Context data.
	 *
	 * @return Graphics_Context The Context data.
	 */
	Graphics_Context* getContext(void);

	attitude::State* getState(void);
	void setState(attitude::State* i_pState);

	void repaintScreen(void);

 protected:
 private:
	/**
	 * Data from accelerometer.
	 */
	uint16_t m_u16X;
	uint16_t m_u16Y;
	uint16_t m_u16Z;

	/**
	 * The current state to draw.
	 *
	 */
	attitude::State* m_pState;

	/**
	 * Previous data from accelerometer.
	 */
	uint16_t m_u16OldZValue;

	/**
	 * The graphics context in which is being draw.
	 */
	static Graphics_Context* m_pContext;

	/**
	 * The LcdScreen Object.
	 */
	peripheral::LcdScreen* m_pLcdScreen;
};

}  // namespace task

#endif /* TASK_REFRESHSCREENBACKGROUND_HPP_ */
