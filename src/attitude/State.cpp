#include "attitude/State.hpp"

attitude::State::State(uint16_t i_u16AccelerometerZ,
                       uint16_t i_u16AccelerometerX) {
	// fill tan values table
	this->scaleTanValues();
	this->setAccelerometerZ(i_u16AccelerometerZ);
	this->setAccelerometerX(i_u16AccelerometerX);

	// Set the B value
	uint16_t l_u16B;
	if (i_u16AccelerometerZ > attitude::state::ADC_Z_MID_VALUE) {
		l_u16B = i_u16AccelerometerX - attitude::state::ADC_Z_MID_VALUE;
		l_u16B = l_u16B / attitude::state::ADC_Z_SCREEN_FACTOR;
		l_u16B = (attitude::state::SCREEN_Y / 2) - l_u16B;
	} else {
		l_u16B = attitude::state::ADC_Z_MID_VALUE - i_u16AccelerometerX;
		l_u16B = l_u16B / attitude::state::ADC_Z_SCREEN_FACTOR;
		l_u16B = (attitude::state::SCREEN_Y / 2) + l_u16B;
	}
	this->setB(l_u16B);
}

void attitude::State::setM(uint8_t i_u8M) { this->m_u8M = i_u8M; }
void attitude::State::setB(uint8_t i_u8B) { this->m_u8B = i_u8B; }

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
                                        uint8_t i_u8Index) {
	this->m_aScaledTanValues[i_u8Index] = i_u16ScaledTanValue;
}

uint16_t attitude::State::getScaledTanValue(uint8_t i_u8Index) {
	return this->m_aScaledTanValues[i_u8Index];
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

uint8_t attitude::State::getAccelerometerXCase() {
	if (this->getAccelerometerX() > (attitude::state::ADC_X_CASE1_LOWER_VALUE)) {
		return 1;
	} else if (this->getAccelerometerX() >
	           attitude::state::ADC_X_CASE2_LOWER_VALUE) {
		return 2;
	} else if (this->getAccelerometerX() >
	           attitude::state::ADC_X_CASE4_LOWER_VALUE) {
		return 4;
	} else {
		return 8;
	}
}

uint8_t attitude::State::getPixelOffsetFromAccelerometerX(void) {
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
	uint16_t i;

	if (l_u16Sum > this->getScaledTanValuesSum()) {
		l_u16Sum = this->getScaledTanValuesSum();
	}

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
			if (l_u16Sum < 0) {
				return i;
			}
		}
	}
}

/**
 * To compile, execute from base dir
 * g++ -Iinclude src/attitude/State.cpp
 *
 * To execute
 * ./a.out
 */
int main() {
	// clang-format off
	attitude::State* l_aStates[] = {
		// horizontal top, case 1
		new attitude::State(8192, 13000), // x value will be set to max(11500)
		new attitude::State(8192, 11400),
		new attitude::State(8192, 11300),
		new attitude::State(8192, 11200),
		new attitude::State(8192, 11100),
		new attitude::State(8192, 11000),
		new attitude::State(8192, 10900),
		new attitude::State(8192, 10800),
		new attitude::State(8192, 10700),
		new attitude::State(8192, 10600),
		new attitude::State(8192, 10500),
		new attitude::State(8192, 10400),
		new attitude::State(8192, 10300),
		new attitude::State(8192, 10200),
		new attitude::State(8192, 10100),
		new attitude::State(8192, 10000),
		new attitude::State(8192, 9847),

		// vertical top, case 2
		new attitude::State(8192, 9846),
		new attitude::State(8192, 9800),
		new attitude::State(8192, 9700),
		new attitude::State(8192, 9600),
		new attitude::State(8192, 9500),
		new attitude::State(8192, 9400),
		new attitude::State(8192, 9300),
		new attitude::State(8192, 9200),
		new attitude::State(8192, 9100),
		new attitude::State(8192, 9000),
		new attitude::State(8192, 8900),
		new attitude::State(8192, 8800),
		new attitude::State(8192, 8700),
		new attitude::State(8192, 8600),
		new attitude::State(8192, 8500),
		new attitude::State(8192, 8400),
		new attitude::State(8192, 8300),
		new attitude::State(8192, 8200),
		new attitude::State(8192, 8193),

		// vertical bottom, case 4
		new attitude::State(8192, 8192),
		new attitude::State(8192, 8100),
		new attitude::State(8192, 8000),
		new attitude::State(8192, 7900),
		new attitude::State(8192, 7800),
		new attitude::State(8192, 7700),
		new attitude::State(8192, 7600),
		new attitude::State(8192, 7500),
		new attitude::State(8192, 7400),
		new attitude::State(8192, 7300),
		new attitude::State(8192, 7200),
		new attitude::State(8192, 7100),
		new attitude::State(8192, 7000),
		new attitude::State(8192, 6900),
		new attitude::State(8192, 6800),
		new attitude::State(8192, 6700),
		new attitude::State(8192, 6600),
		new attitude::State(8192, 6539),

		// horizontal bottom, case 8
		new attitude::State(8192, 6538),
		new attitude::State(8192, 6500),
		new attitude::State(8192, 6400),
		new attitude::State(8192, 6300),
		new attitude::State(8192, 6200),
		new attitude::State(8192, 6100),
		new attitude::State(8192, 6000),
		new attitude::State(8192, 5900),
		new attitude::State(8192, 5800),
		new attitude::State(8192, 5700),
		new attitude::State(8192, 5600),
		new attitude::State(8192, 5500),
		new attitude::State(8192, 5400),
		new attitude::State(8192, 5300),
		new attitude::State(8192, 5200),
		new attitude::State(8192, 5100),
		new attitude::State(8192, 5000),
		new attitude::State(8192, 4900),
		new attitude::State(8192, 4884),
		new attitude::State(8192, 2000), // x value will be set to min(4884)
		NULL
	};
	//clang-format on

	uint16_t i = 0;
	while (l_aStates[i] != NULL) {
		std::cout << "************\n";
		std::cout << " " << i << "\n";
		std::cout << "************\n";

		std::cout << "l_aStates[i]->getAccelerometerXCase(): ";
		std::cout << (uint16_t)l_aStates[i]->getAccelerometerXCase() << "\n";

		std::cout << "l_aStates[i]->getAccelerometerX(): ";
		std::cout << l_aStates[i]->getAccelerometerX() << "\n";

		std::cout << "l_pPixelOffset: ";
		std::cout << (uint16_t)l_aStates[i]->getPixelOffsetFromAccelerometerX() << "\n\n";
		i++;
	}

	return 0;
}