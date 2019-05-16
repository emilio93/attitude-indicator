#ifndef TASK_LED_HPP_
#define TASK_LED_HPP_

#include <cstddef>

#include "mkii/Led.hpp"

#include "scheduler/Task.hpp"

namespace task {

class LED : public scheduler::Task {
 public:
	LED(mkii::Led* i_pLed);
	virtual uint8_t run(void);
	virtual uint8_t setup(void);

 protected:
 private:
	mkii::Led* m_pLed;

	void setLed(mkii::Led* i_pLed);

	mkii::Led* getLed(void);
};

}  // namespace task

#endif /* TASK_LED_HPP_ */
