#include <iostream>

#include "task/Accelerometer.hpp"

task::Accelerometer::Accelerometer(mkii::Accelerometer* i_pAccelerometer) {
	this->setAccelerometer(i_pAccelerometer);
}

uint8_t task::Accelerometer::run() {
	this->getAccelerometer()->UpdateResults();
	this->getAccelerometer()->TriggerConversion();

	return (NO_ERR);
}

uint8_t task::Accelerometer::setup() {
	this->getAccelerometer()->TriggerConversion();
	return (NO_ERR);
}

void task::Accelerometer::setAccelerometer(
    mkii::Accelerometer* i_pAccelerometer) {
	this->m_pAccelerometer = i_pAccelerometer;
}

mkii::Accelerometer* task::Accelerometer::getAccelerometer(void) {
	return this->m_pAccelerometer;
}
