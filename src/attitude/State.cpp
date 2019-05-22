#include "attitude/State.hpp"

attitude::State::State(uint16_t i_u16AccelerometerZ,
                       uint16_t i_u16AccelerometerX) {
	// fill tan values table
	this->scaleTanValues();
	this->setAccelerometerZ(i_u16AccelerometerZ);
	this->setAccelerometerX(i_u16AccelerometerX);

	// Set the B value from Acceleroometer Z data
	uint16_t l_u16B;
	if (this->getAccelerometerX() > attitude::state::ADC_Z_MID_VALUE) {
		l_u16B = i_u16AccelerometerX - attitude::state::ADC_Z_MID_VALUE;
		l_u16B = l_u16B / attitude::state::ADC_Z_SCREEN_FACTOR;
		l_u16B = (attitude::state::SCREEN_Y / 2) - l_u16B;
	} else {
		l_u16B = attitude::state::ADC_Z_MID_VALUE - this->getAccelerometerX();
		l_u16B = l_u16B / attitude::state::ADC_Z_SCREEN_FACTOR;
		l_u16B = (attitude::state::SCREEN_Y / 2) + l_u16B;
	}
	this->setB(l_u16B);

	uint16_t l_u16M;
	uint16_t l_u16CaseX = this->getAccelerometerXCase();
	l_u16M = this->getPixelOffsetFromAccelerometerX();


}

void attitude::State::setM(uint16_t i_u16M) { this->m_u16M = i_u16M; }
uint16_t attitude::State::getM(void) { return this->m_u16M; }
void attitude::State::setB(uint16_t i_u16B) { this->m_u16B = i_u16B; }
uint16_t attitude::State::getB(void) { return this->m_u16B; }

void attitude::State::setAccelerometerZ(uint16_t i_u16AccelerometerZ) {
	this->m_u16AccelerometerZ = i_u16AccelerometerZ;
	if (i_u16AccelerometerZ > (attitude::state::ADC_Z_MID_VALUE +
	                           attitude::state::ADC_Z_MAX_VARIATION)) {
		this->m_u16AccelerometerZ =
		    attitude::state::ADC_Z_MID_VALUE + attitude::state::ADC_Z_MAX_VARIATION;
	}
	if (i_u16AccelerometerZ < (attitude::state::ADC_Z_MID_VALUE -
	                           attitude::state::ADC_Z_MAX_VARIATION)) {
		this->m_u16AccelerometerZ =
		    attitude::state::ADC_Z_MID_VALUE - attitude::state::ADC_Z_MAX_VARIATION;
	}
}
void attitude::State::setAccelerometerX(uint16_t i_u16AccelerometerX) {
	this->m_u16AccelerometerX = i_u16AccelerometerX;
	if (i_u16AccelerometerX > attitude::state::ADC_X_UPPER_VALUE) {
		this->m_u16AccelerometerX = attitude::state::ADC_X_UPPER_VALUE;
	}
	if (i_u16AccelerometerX < attitude::state::ADC_X_LOWER_VALUE) {
		this->m_u16AccelerometerX = attitude::state::ADC_X_LOWER_VALUE;
	}
}

uint16_t attitude::State::getAccelerometerZ() {
	return this->m_u16AccelerometerZ;
}

uint16_t attitude::State::getAccelerometerX() {
	return this->m_u16AccelerometerX;
}

void attitude::State::setScaledTanValue(uint16_t i_u16ScaledTanValue,
                                        uint16_t i_u16Index) {
	this->m_aScaledTanValues[i_u16Index] = i_u16ScaledTanValue;
}

uint16_t attitude::State::getScaledTanValue(uint16_t i_u16Index) {
	return this->m_aScaledTanValues[i_u16Index];
}

void attitude::State::setScaledTanValuesSum(uint16_t i_u16ScaledTanValuesSum) {
	this->m_u16ScaledTanValuesSum = i_u16ScaledTanValuesSum;
}

uint16_t attitude::State::getScaledTanValuesSum(void) {
	return this->m_u16ScaledTanValuesSum;
}

void attitude::State::scaleTanValues(void) {
	uint16_t l_u16Sum = 0;
	for (uint8_t i = 0; i < attitude::state::SCREEN_MAX / 2; i++) {
		// Example TAN_ADD_VALUES[0] = 658
		//         ADC_X_MAX_VARIATION = 3308
		//         TAN_ADD_VALUES_SUM = 65536
		//         l_u32ScaledValue = 658 * 3308/2 / 65536 = 16.606
		uint32_t l_u32ScaledValue = attitude::state::TAN_ADD_VALUES[i] *
		                            (attitude::state::ADC_X_MAX_VARIATION / 2) /
		                            attitude::state::TAN_ADD_VALUES_SUM;
		this->setScaledTanValue((uint16_t)l_u32ScaledValue, i);
		l_u16Sum = l_u16Sum + (uint16_t)l_u32ScaledValue;
	}
	this->setScaledTanValuesSum(l_u16Sum);
}

uint16_t attitude::State::getAccelerometerXCase(void) {
	if (this->getAccelerometerX() > (attitude::state::ADC_X_CASE1_LOWER_VALUE)) {
		// line crosses horizontal top border
		return 1;
	} else if (this->getAccelerometerX() >
	           attitude::state::ADC_X_CASE2_LOWER_VALUE) {
		// line crosses vertical top border
		return 2;
	} else if (this->getAccelerometerX() >
	           attitude::state::ADC_X_CASE4_LOWER_VALUE) {
		// line crosses vertical bottom border
		return 4;
	} else {
		// line crosses horizontal bottom border
		return 8;
	}
}

uint16_t attitude::State::getPixelOffsetFromAccelerometerX(void) {
	// the direction indicate the direction iteration over the steps.
	// true indicates from 0 to end.
	// false indicates from end to 0.
	bool l_bAscendingDirection = false;
	int16_t l_u16Sum;
	uint8_t l_u8Case = this->getAccelerometerXCase();

	if (l_u8Case == 1) {
		// line crosses horizontal top border
		l_bAscendingDirection = false;
		l_u16Sum =
		    this->getAccelerometerX() - attitude::state::ADC_X_CASE1_LOWER_VALUE;

	} else if (l_u8Case == 2) {
		// line crosses vertical top border
		l_bAscendingDirection = true;
		l_u16Sum =
		    this->getAccelerometerX() - attitude::state::ADC_X_CASE2_LOWER_VALUE;

	} else if (l_u8Case == 4) {
		// line crosses vertical bottom border
		l_bAscendingDirection = false;
		l_u16Sum =
		    this->getAccelerometerX() - attitude::state::ADC_X_CASE4_LOWER_VALUE;

	} else {
		// line crosses horizontal bottom border
		l_bAscendingDirection = true;
		l_u16Sum =
		    this->getAccelerometerX() - attitude::state::ADC_X_CASE8_LOWER_VALUE;
	}

	// clip the value to avoid unexpected results
	if (l_u16Sum > this->getScaledTanValuesSum()) {
		l_u16Sum = this->getScaledTanValuesSum();
	}

	uint16_t i;
	if (l_bAscendingDirection) {
		for (i = 0; i < attitude::state::SCREEN_MAX / 2; i++) {
			l_u16Sum = l_u16Sum - this->getScaledTanValue(i);
			if (l_u16Sum <= 0) {
				return i;
			}
		}
	} else {
		for (i = attitude::state::SCREEN_MAX / 2; i > 0; i--) {
			l_u16Sum = l_u16Sum - this->getScaledTanValue(i - 1);
			if (l_u16Sum <= 0) {
				return i;
			}
		}
	}
}

#ifdef TEST_ATTITUDE_STATE
/**
 * To compile, execute from base dir
 * g++ -O0 -g -Iinclude -DTEST_ATTITUDE_STATE src/attitude/State.cpp
 *
 * Test with valgrind for leaks
 * valgrind --leak-check=yes ./a.out
 *
 * To execute
 * ./a.out
 *
 *
 * The code test the correct intialization of the attitude::State object
 *
 */
int main() {
	uint16_t i = 4800;
	uint16_t j = 0;
	uint16_t l_u16Start = 4850;
	uint16_t l_u16End = 11550;
	uint16_t l_u16Step = 5;
	uint16_t l_u16ZValue = 8192;
	bool l_bPrint = false;

	if (l_bPrint) std::cout << "X value\tX Case\tB value\tPixel offset\n";
	while (i <= l_u16End) {
		attitude::State* l_pState = new attitude::State(l_u16ZValue, i);

		if (l_bPrint) std::cout << l_pState->getAccelerometerX() << "\t";
		if (l_bPrint) std::cout << l_pState->getAccelerometerXCase() << "\t";
		if (l_bPrint) std::cout << l_pState->getB() << "\t";
		if (l_bPrint) std::cout << l_pState->getPixelOffsetFromAccelerometerX() << "\n";
		i = i + l_u16Step;
		j++;
		delete l_pState;
	}

	return 0;
}
#endif