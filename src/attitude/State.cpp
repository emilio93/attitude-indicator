#include "attitude/State.hpp"
uint16_t attitude::State::m_aScaledTanValues[attitude::state::SCREEN_MAX / 2] =
    {0};
bool attitude::State::m_bHasCalculatedScaledTanValues = false;
uint16_t attitude::State::m_u16ScaledTanValuesSum = 0;

attitude::State::State(uint16_t i_u16AccelerometerZ,
                       uint16_t i_u16AccelerometerX) {
	// fill tan values table
	this->scaleTanValues();
	this->setAccelerometerZ(i_u16AccelerometerZ);
	this->setAccelerometerX(i_u16AccelerometerX);

	this->setCaseX(this->calculateCaseX());

	// Set the B value from Accelerometer Z data
	this->setB(this->calculateB());
	this->setM(this->calculateM());
}

int32_t attitude::State::getPointAX() {
	int32_t l_i32PointAX = (attitude::state::SCREEN_Y / 2) - 1;
	if (this->getCaseX() == attitude::state::CaseX::TOP_HORIZONTAL ||
	    this->getCaseX() == attitude::state::CaseX::BOTTOM_HOTIZONTAL) {
		l_i32PointAX = l_i32PointAX - 2 * this->getPixelOffsetFromAccelerometerX();

	} else if (this->getCaseX() == attitude::state::CaseX::TOP_VERTICAL ||
	           this->getCaseX() == attitude::state::CaseX::BOTTOM_VERTICAL) {
		l_i32PointAX = l_i32PointAX - 2 * (attitude::state::SCREEN_Y / 2);
	}
	return l_i32PointAX;
}

int32_t attitude::State::getPointAY() {
	int32_t l_i32PointAY = this->getB();
	if (this->getCaseX() == attitude::state::CaseX::TOP_HORIZONTAL) {
		l_i32PointAY = l_i32PointAY + 2 * (attitude::state::SCREEN_Y / 2);
	} else if (this->getCaseX() == attitude::state::CaseX::TOP_VERTICAL) {
		l_i32PointAY =
		    l_i32PointAY + 2 * (this->getPixelOffsetFromAccelerometerX());
	} else if (this->getCaseX() == attitude::state::CaseX::BOTTOM_VERTICAL) {
		l_i32PointAY =
		    l_i32PointAY - 2 * (this->getPixelOffsetFromAccelerometerX());
	} else if (this->getCaseX() == attitude::state::CaseX::BOTTOM_HOTIZONTAL) {
		l_i32PointAY = l_i32PointAY - 2 * (attitude::state::SCREEN_Y / 2);
	}
	return l_i32PointAY;
}

int32_t attitude::State::getPointBX() {
	int32_t l_i32PointBX = attitude::state::SCREEN_X / 2;
	if (this->getCaseX() == attitude::state::CaseX::TOP_HORIZONTAL ||
	    this->getCaseX() == attitude::state::CaseX::BOTTOM_HOTIZONTAL) {
		l_i32PointBX = l_i32PointBX + 2 * this->getPixelOffsetFromAccelerometerX();

	} else if (this->getCaseX() == attitude::state::CaseX::TOP_VERTICAL ||
	           this->getCaseX() == attitude::state::CaseX::BOTTOM_VERTICAL) {
		l_i32PointBX = l_i32PointBX + 2 * (attitude::state::SCREEN_X / 2);
	}
	return l_i32PointBX;
}

int32_t attitude::State::getPointBY() {
	int32_t l_i32PointBY = this->getB();
	if (this->getCaseX() == attitude::state::CaseX::TOP_HORIZONTAL) {
		l_i32PointBY = l_i32PointBY - 2 * (attitude::state::SCREEN_Y / 2);
	} else if (this->getCaseX() == attitude::state::CaseX::TOP_VERTICAL) {
		l_i32PointBY =
		    l_i32PointBY - 2 * (this->getPixelOffsetFromAccelerometerX());
	} else if (this->getCaseX() == attitude::state::CaseX::BOTTOM_VERTICAL) {
		l_i32PointBY =
		    l_i32PointBY + 2 * (this->getPixelOffsetFromAccelerometerX());
	} else if (this->getCaseX() == attitude::state::CaseX::BOTTOM_HOTIZONTAL) {
		l_i32PointBY = l_i32PointBY + 2 * (attitude::state::SCREEN_Y / 2);
	}
	return l_i32PointBY;
}

void attitude::State::setM(uint8_t i_u8M) { this->m_u8M = i_u8M; }
uint8_t attitude::State::getM(void) { return this->m_u8M; }
void attitude::State::setB(uint16_t i_u16B) { this->m_u16B = i_u16B; }
uint16_t attitude::State::getB(void) { return this->m_u16B; }
attitude::state::CaseX attitude::State::getCaseX() { return this->m_eCaseX; }

uint8_t attitude::State::calculateM(void) {
	uint8_t l_u8PixelOffset = (uint8_t)this->getPixelOffsetFromAccelerometerX();
	uint8_t l_u8M = 0x00;
	if (this->getCaseX() == attitude::state::CaseX::TOP_HORIZONTAL) {
		l_u8M = 0x00 | (0x3F & l_u8PixelOffset);
	} else if (this->getCaseX() == attitude::state::CaseX::TOP_VERTICAL) {
		l_u8M = 0x40 | (0x3F & l_u8PixelOffset);
	} else if (this->getCaseX() == attitude::state::CaseX::BOTTOM_VERTICAL) {
		l_u8M = 0x80 | (0x3F & l_u8PixelOffset);
	} else if (this->getCaseX() == attitude::state::CaseX::BOTTOM_HOTIZONTAL) {
		l_u8M = 0xc0 | (0x3F & l_u8PixelOffset);
	}
	return l_u8M;
}

uint16_t attitude::State::calculateB(void) {
	uint16_t l_u16B;
	if (this->getAccelerometerZ() > attitude::state::ADC_Z_MID_VALUE) {
		l_u16B = this->getAccelerometerZ() - attitude::state::ADC_Z_MID_VALUE;
		l_u16B = l_u16B / attitude::state::ADC_Z_SCREEN_FACTOR;
		l_u16B = (attitude::state::SCREEN_Y / 2) - l_u16B;
	} else {
		l_u16B = attitude::state::ADC_Z_MID_VALUE - this->getAccelerometerZ();
		l_u16B = l_u16B / attitude::state::ADC_Z_SCREEN_FACTOR;
		l_u16B = (attitude::state::SCREEN_Y / 2) + l_u16B;
		l_u16B = l_u16B - 1;
	}
	return l_u16B;
}

void attitude::State::setCaseX(attitude::state::CaseX i_eCaseX) {
	this->m_eCaseX = i_eCaseX;
}

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
	attitude::State::m_aScaledTanValues[i_u16Index] = i_u16ScaledTanValue;
}

uint16_t attitude::State::getScaledTanValue(uint16_t i_u16Index) {
	return attitude::State::m_aScaledTanValues[i_u16Index];
}

void attitude::State::setScaledTanValuesSum(uint16_t i_u16ScaledTanValuesSum) {
	attitude::State::m_u16ScaledTanValuesSum = i_u16ScaledTanValuesSum;
}

uint16_t attitude::State::getScaledTanValuesSum(void) {
	return attitude::State::m_u16ScaledTanValuesSum;
}

void attitude::State::scaleTanValues(void) {
	// run function only once.
	if (attitude::State::m_bHasCalculatedScaledTanValues) return;
	attitude::State::m_bHasCalculatedScaledTanValues = true;

	uint16_t l_u16i = 0;
	uint16_t u_16LimitI = 100;
	uint16_t l_u16Sum = 0;
	uint16_t l_u16InitialVariation = attitude::state::ADC_X_MAX_VARIATION / 2;
	do {
		l_u16Sum = 0;
		for (uint16_t i = 0; i < attitude::state::SCREEN_MAX / 2; i++) {
			// Example TAN_ADD_VALUES[0] = 658
			//         ADC_X_MAX_VARIATION = 3308
			//         TAN_ADD_VALUES_SUM = 65536
			//         l_u32ScaledValue = 658 * 3308/2 / 65536 = 16.606
			uint32_t l_u32ScaledValue = attitude::state::TAN_ADD_VALUES[i] *
			                            (l_u16InitialVariation) /
			                            attitude::state::TAN_ADD_VALUES_SUM;
			this->setScaledTanValue((uint16_t)l_u32ScaledValue, i);
			l_u16Sum = l_u16Sum + (uint16_t)l_u32ScaledValue;
		}
		this->setScaledTanValuesSum(l_u16Sum);

		if (this->getScaledTanValuesSum() >
		    (attitude::state::ADC_X_MAX_VARIATION / 2)) {
			l_u16InitialVariation--;
		} else if (this->getScaledTanValuesSum() <
		           (attitude::state::ADC_X_MAX_VARIATION / 2)) {
			l_u16InitialVariation = l_u16InitialVariation + 7;
		}
		l_u16i++;
		if (l_u16i > u_16LimitI) return;
	} while (this->getScaledTanValuesSum() !=
	         attitude::state::ADC_X_MAX_VARIATION / 2);
}

attitude::state::CaseX attitude::State::calculateCaseX(void) {
	if (this->getAccelerometerX() > (attitude::state::ADC_X_CASE1_LOWER_VALUE)) {
		// line crosses horizontal top border
		return attitude::state::CaseX::TOP_HORIZONTAL;
	} else if (this->getAccelerometerX() >
	           attitude::state::ADC_X_CASE2_LOWER_VALUE) {
		// line crosses vertical top border
		return attitude::state::CaseX::TOP_VERTICAL;
	} else if (this->getAccelerometerX() >
	           attitude::state::ADC_X_CASE4_LOWER_VALUE) {
		// line crosses vertical bottom border
		return attitude::state::CaseX::BOTTOM_VERTICAL;
	} else if (this->getAccelerometerX() >=
	           attitude::state::ADC_X_CASE8_LOWER_VALUE) {
		// line crosses horizontal bottom border
		return attitude::state::CaseX::BOTTOM_HOTIZONTAL;
	} else {
		// shouldn get here
		return attitude::state::CaseX::ERROR;
	}
}

uint16_t attitude::State::getPixelOffsetFromAccelerometerX(void) {
	// the direction indicate the direction iteration over the steps.
	// true indicates from 0 to end.
	// false indicates from end to 0.
	bool l_bAscendingDirection = false;
	int16_t l_i16Sum = 0;

	if (this->getCaseX() == attitude::state::CaseX::TOP_HORIZONTAL) {
		// line crosses horizontal top border
		l_bAscendingDirection = false;
		l_i16Sum =
		    this->getAccelerometerX() - attitude::state::ADC_X_CASE1_LOWER_VALUE;

	} else if (this->getCaseX() == attitude::state::CaseX::TOP_VERTICAL) {
		// line crosses vertical top border
		l_bAscendingDirection = true;
		l_i16Sum =
		    this->getAccelerometerX() - attitude::state::ADC_X_CASE2_LOWER_VALUE;

	} else if (this->getCaseX() == attitude::state::CaseX::BOTTOM_VERTICAL) {
		// line crosses vertical bottom border
		l_bAscendingDirection = false;
		l_i16Sum =
		    this->getAccelerometerX() - attitude::state::ADC_X_CASE4_LOWER_VALUE;

	} else if (this->getCaseX() == attitude::state::CaseX::BOTTOM_HOTIZONTAL) {
		// line crosses horizontal bottom border
		l_bAscendingDirection = true;
		l_i16Sum =
		    this->getAccelerometerX() - attitude::state::ADC_X_CASE8_LOWER_VALUE;
	}

	// clip the value to avoid unexpected results
	if (l_i16Sum > this->getScaledTanValuesSum()) {
		l_i16Sum = this->getScaledTanValuesSum();
	}

	uint16_t l_u16i;
	if (l_bAscendingDirection) {
		for (l_u16i = 0; l_u16i < attitude::state::SCREEN_MAX / 2; l_u16i++) {
			l_i16Sum = l_i16Sum - this->getScaledTanValue(l_u16i);
			if (l_i16Sum <= 0) {
				break;
			}
		}
	} else {
		for (l_u16i = attitude::state::SCREEN_MAX / 2; l_u16i > 0; l_u16i--) {
			l_i16Sum = l_i16Sum - this->getScaledTanValue(l_u16i - 1);
			if (l_i16Sum <= 0) {
				break;
			}
		}
	}
	if (l_u16i > 63) l_u16i = 63;

	return l_u16i;
}

void attitude::State::printInfo(bool i_bPrintHeader) {
	if (i_bPrintHeader) {
		std::cout << "Z value\tX value\tX Case\tB value\tState\tS0-S1\tN0-N5\tPxl "
		             "off\tPoint A\t\tPoint B\n";
	}
	std::cout << this->getAccelerometerZ() << "\t";
	std::cout << this->getAccelerometerX() << "\t";
	std::cout << this->getCaseX() << "\t";
	std::cout << this->getB() << "\t";
	std::cout << std::hex << (uint16_t)this->getM() << std::dec;
	std::cout << "\t";
	std::cout << std::hex << (uint16_t)((0xC0 & this->getM()) >> 6) << std::dec;
	std::cout << "\t";
	std::cout << (uint16_t)(0x3f & this->getM()) << std::dec;
	std::cout << "\t";
	std::cout << this->getPixelOffsetFromAccelerometerX() << "\t";
	std::cout << "(" << this->getPointAX() << ", " << this->getPointAY() << ")\t";
	std::cout << "(" << this->getPointBX() << ", " << this->getPointBY() << ")\n";
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
	uint16_t l_u16Step = 200;
	uint16_t l_u16ZValue = 8192;
	bool l_bPrint = true;

	if (l_bPrint)
		std::cout << "Z value\tX value\tX Case\tB value\tState\tS0-S1\tN0-N5\tPxl "
		             "off\tPoint A\t\tPoint B\n";
	while (i <= l_u16End) {
		attitude::State* l_pState = new attitude::State(l_u16ZValue, i);
		if (l_bPrint) l_pState->printInfo();
		i = i + l_u16Step;
		j++;
		delete l_pState;
	}

	return 0;
}
#endif
