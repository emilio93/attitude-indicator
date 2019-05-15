#ifndef TASK_ACCELEROMETER_HPP_
#define TASK_ACCELEROMETER_HPP_
#define __NOP __nop

#include "msp.h"

#include <ti/grlib/grlib.h>
extern "C" {
#include <Crystalfontz128x128_ST7735.h>
}

#include "mkii/Accelerometer.hpp"

#include "scheduler/Task.hpp"

namespace task {

class Accelerometer : public scheduler::Task {
 public:
	Accelerometer(mkii::Accelerometer* i_pAccelerometer);
	virtual uint8_t run(void);
	virtual uint8_t setup(void);

 protected:
 private:
	uint16_t m_u16X;
	uint16_t m_u16Y;
	uint16_t m_u16Z;

	static Graphics_Context m_stStaticContext;

	mkii::Accelerometer* m_pAccelerometer;

	void setAccelerometer(mkii::Accelerometer* i_pAccelerometer);

	mkii::Accelerometer* getAccelerometer(void);

	void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

	void drawBackground();

	void updateBackground();

	void printResults();
};

}  // namespace task

#endif /* LED_HPP_ */
