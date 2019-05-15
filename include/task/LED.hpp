/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef LED_HPP_
#define LED_HPP_
#define __NOP __nop

#include "msp.h"

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

#endif /* LED_HPP_ */
