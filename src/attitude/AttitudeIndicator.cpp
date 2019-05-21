#include "attitude/AttitudeIndicator.hpp"

attitude::AttitudeIndicator::AttitudeIndicator(
    mkii::Accelerometer* i_pAccelerometer) {
	this->setAccelerometer(i_pAccelerometer);
}

void attitude::AttitudeIndicator::setAccelerometer(
    mkii::Accelerometer* i_pAccelerometer) {
	this->m_pAccelerometer = i_pAccelerometer;
}
