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

#include "scheduler/Task.hpp"

namespace task {

class LED : public scheduler::Task {
 public:
	LED(uint16_t);
	uint16_t m_u16BITN;
	virtual uint8_t run(void);
	virtual uint8_t setup(void);

 protected:
 private:
};

}  // namespace task

#endif /* LED_HPP_ */
