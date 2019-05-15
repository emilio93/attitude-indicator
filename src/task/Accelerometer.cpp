#include <iostream>

#include "task/Accelerometer.hpp"

Graphics_Context task::Accelerometer::m_stStaticContext;

task::Accelerometer::Accelerometer(mkii::Accelerometer* i_pAccelerometer) {
	this->setAccelerometer(i_pAccelerometer);
}

uint8_t task::Accelerometer::run() {
	this->m_u16X = ADC14_getResult(ADC_MEM0);
	this->m_u16Y = ADC14_getResult(ADC_MEM1);
	this->m_u16Z = ADC14_getResult(ADC_MEM2);

	this->updateBackground();
	this->printResults();
	this->getAccelerometer()->TriggerConversion();

	return (NO_ERR);
}

uint8_t task::Accelerometer::setup() {
	/* Initializes display */
	Crystalfontz128x128_Init();

	/* Set default screen orientation */
	Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

	/* Initializes graphics context */
	Graphics_initContext(&task::Accelerometer::m_stStaticContext,
	                     &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
	Graphics_setBackgroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_WHITE);

	GrContextFontSet(&task::Accelerometer::m_stStaticContext, &g_sFontFixed6x8);

	this->updateBackground();
	this->printResults();

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

void task::Accelerometer::drawLine(int32_t x1, int32_t y1, int32_t x2,
                                   int32_t y2) {
	Graphics_drawLine(&task::Accelerometer::m_stStaticContext, x1, y1, x2, y2);
}

void task::Accelerometer::updateBackground() {
	uint16_t l_u16Threshold = 8192;
	uint16_t l_u16Divider = 52;
	uint16_t l_u16Dif;
	uint16_t l_u16Height;
	Graphics_Rectangle l_stRectBlue;
	Graphics_Rectangle l_stRectBrown;
	if (this->m_u16Z > l_u16Threshold) {
		l_u16Dif = (this->m_u16Z - l_u16Threshold) / l_u16Divider;
		l_u16Height = 64 - l_u16Dif;
	} else {
		l_u16Dif = (l_u16Threshold - this->m_u16Z) / l_u16Divider;
		l_u16Height = 64 + l_u16Dif;
	}

	l_stRectBlue = {0, 0, 127, l_u16Height};
	l_stRectBrown = {0, 127, 127, l_u16Height};
	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_LIGHT_BLUE);
	Graphics_fillRectangle(&task::Accelerometer::m_stStaticContext,
	                       &l_stRectBlue);
	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_PERU);
	Graphics_fillRectangle(&task::Accelerometer::m_stStaticContext,
	                       &l_stRectBrown);

	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_WHITE);
	this->drawLine(0, l_u16Height, 127, l_u16Height);

	int i, maxi;
	uint8_t j, k;
	uint8_t l_u8LongLineHalfLenght = 8;
	uint8_t l_u8ShortLineHalfLenght = 4;
	uint8_t l_u8XCenter = 64;
	uint8_t l_u8YStart = l_u16Height;
	uint8_t l_u8YSeparation = 10;
	bool l_isToggle = true;

	uint8_t l_u8OffsetBlue;
	if (l_u16Height > 120) {
		l_u8OffsetBlue = 120;
	} else if (l_u16Height > 110) {
		l_u8OffsetBlue = 110;
	} else if (l_u16Height > 100) {
		l_u8OffsetBlue = 100;
	} else if (l_u16Height > 90) {
		l_u8OffsetBlue = 90;
	} else if (l_u16Height > 80) {
		l_u8OffsetBlue = 80;
	} else if (l_u16Height > 70) {
		l_u8OffsetBlue = 70;
	} else if (l_u16Height > 60) {
		l_u8OffsetBlue = 60;
	} else if (l_u16Height > 50) {
		l_u8OffsetBlue = 50;
	} else if (l_u16Height > 40) {
		l_u8OffsetBlue = 40;
	} else if (l_u16Height > 30) {
		l_u8OffsetBlue = 30;
	} else if (l_u16Height > 20) {
		l_u8OffsetBlue = 20;
	} else if (l_u16Height > 10) {
		l_u8OffsetBlue = 10;
	} else {
		l_u8OffsetBlue = 0;
	}

	l_u8YStart = l_u16Height - l_u8OffsetBlue;
	maxi = l_u8OffsetBlue / 10;

	for (i = 0; i < maxi; i++) {
		j = l_isToggle ? l_u8XCenter - l_u8LongLineHalfLenght
		               : l_u8XCenter - l_u8ShortLineHalfLenght;
		k = l_isToggle ? l_u8XCenter + l_u8LongLineHalfLenght
		               : l_u8XCenter + l_u8ShortLineHalfLenght;

		this->drawLine(j, l_u8YStart + l_u8YSeparation * i, k,
		               l_u8YStart + l_u8YSeparation * i);
		l_isToggle = !l_isToggle;
	}

	l_u8YStart = l_u16Height + 10;
	if (l_u16Height < 7) {
		maxi = 12;
	} else if (l_u16Height < 17) {
		maxi = 11;
	} else if (l_u16Height < 27) {
		maxi = 10;
	} else if (l_u16Height < 37) {
		maxi = 9;
	} else if (l_u16Height < 47) {
		maxi = 8;
	} else if (l_u16Height < 57) {
		maxi = 7;
	} else if (l_u16Height < 67) {
		maxi = 6;
	} else if (l_u16Height < 77) {
		maxi = 5;
	} else if (l_u16Height < 87) {
		maxi = 4;
	} else if (l_u16Height < 97) {
		maxi = 3;
	} else if (l_u16Height < 107) {
		maxi = 2;
	} else if (l_u16Height < 117) {
		maxi = 1;
	} else {
		maxi = 0;
	}
	l_isToggle = false;
	for (i = 0; i < maxi; i++) {
		j = l_isToggle ? l_u8XCenter - l_u8LongLineHalfLenght
		               : l_u8XCenter - l_u8ShortLineHalfLenght;
		k = l_isToggle ? l_u8XCenter + l_u8LongLineHalfLenght
		               : l_u8XCenter + l_u8ShortLineHalfLenght;

		this->drawLine(j, l_u8YStart + l_u8YSeparation * i, k,
		               l_u8YStart + l_u8YSeparation * i);
		l_isToggle = !l_isToggle;
	}
}

void task::Accelerometer::printResults() {
	char string[10];
	Graphics_setBackgroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_PERU);
	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_BLACK);
	sprintf(string, "X: %5d", this->m_u16X);
	Graphics_drawString(&task::Accelerometer::m_stStaticContext, (int8_t*)string,
	                    AUTO_STRING_LENGTH, 4, 100, TRANSPARENT_TEXT);
	sprintf(string, "Y: %5d", this->m_u16Y);
	Graphics_drawString(&task::Accelerometer::m_stStaticContext, (int8_t*)string,
	                    AUTO_STRING_LENGTH, 4, 110, TRANSPARENT_TEXT);
	sprintf(string, "Z: %5d", this->m_u16Z);
	Graphics_drawString(&task::Accelerometer::m_stStaticContext, (int8_t*)string,
	                    AUTO_STRING_LENGTH, 4, 120, TRANSPARENT_TEXT);
}
