#include "task/LED.hpp"

task::LED::LED(mkii::Led* i_pLed) { this->setLed(i_pLed); }

uint8_t task::LED::run() {
	this->getLed()->Toggle();
	return (NO_ERR);
}

uint8_t task::LED::setup() {
	// setup is done when the object was constructed.
	return (NO_ERR);
}

void task::LED::setLed(mkii::Led* i_pLed) { this->m_pLed = i_pLed; }

mkii::Led* task::LED::getLed(void) { return this->m_pLed; }
