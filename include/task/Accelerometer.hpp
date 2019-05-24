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

	mkii::Accelerometer* getAccelerometer(void);

 private:
	mkii::Accelerometer* m_pAccelerometer;

	void setAccelerometer(mkii::Accelerometer* i_pAccelerometer);
};

}  // namespace task

#endif /* TASK_ACCELEROMETER_HPP_ */
