#include <iostream>

#include "task/Accelerometer.hpp"

task::Accelerometer::Accelerometer(mkii::Accelerometer* i_pAccelerometer) {
	this->setAccelerometer(i_pAccelerometer);
}

uint8_t task::Accelerometer::run() {
	this->m_u16X = ADC14_getResult(ADC_MEM0);
	this->m_u16Y = ADC14_getResult(ADC_MEM1);
	this->m_u16Z = ADC14_getResult(ADC_MEM2);

	this->printResults();
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
