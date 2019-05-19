#ifndef TASK_ACCELEROMETER_HPP_
#define TASK_ACCELEROMETER_HPP_

#include <cstddef>

#include <ti/grlib/grlib.h>

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

	uint16_t m_u16OldZValue;

	static Graphics_Context m_stStaticContext;

	mkii::Accelerometer* m_pAccelerometer;

	void setAccelerometer(mkii::Accelerometer* i_pAccelerometer);

	mkii::Accelerometer* getAccelerometer(void);
};

}  // namespace task

#endif /* TASK_ACCELEROMETER_HPP_ */
