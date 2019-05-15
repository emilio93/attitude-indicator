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

	this->drawBackground();
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
		l_u16Height = 64 + l_u16Dif;
	} else {
		l_u16Dif = (l_u16Threshold - this->m_u16Z) / l_u16Divider;
		l_u16Height = 64 - l_u16Dif;
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

	if (l_u16Height > 40) {
		l_u8YStart = l_u16Height - 40;
		maxi = 4;
	} else if (l_u16Height > 30) {
		l_u8YStart = l_u16Height - 30;
		maxi = 3;
	} else if (l_u16Height > 20) {
		l_u8YStart = l_u16Height - 20;
		maxi = 2;
	} else if (l_u16Height > 10) {
		l_u8YStart = l_u16Height - 10;
		maxi = 1;
	} else {
		maxi = 0;
	}

	for (i = 0; i < maxi; i++) {
		j = l_isToggle ? l_u8XCenter - l_u8LongLineHalfLenght
		               : l_u8XCenter - l_u8ShortLineHalfLenght;
		k = l_isToggle ? l_u8XCenter + l_u8LongLineHalfLenght
		               : l_u8XCenter + l_u8ShortLineHalfLenght;

		this->drawLine(j, l_u8YStart + l_u8YSeparation * i, k,
		               l_u8YStart + l_u8YSeparation * i);
		l_isToggle = !l_isToggle;
	}
	if (l_u16Height > 47) {
		l_u8YStart = l_u8YStart - 7;
		for (i = 0; i < 5; i++) {
			this->drawLine(l_u8XCenter - i, l_u8YStart + i, l_u8XCenter + i,
			               l_u8YStart + i);
		}
	}

	l_u8YStart = l_u16Height + 10;
	if (l_u16Height < 77) {
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

void task::Accelerometer::drawBackground() {
	int i;
	uint8_t j, k;
	uint8_t l_u8LongLineHalfLenght = 8;
	uint8_t l_u8ShortLineHalfLenght = 4;
	uint8_t l_u8XCenter = 64;
	uint8_t l_u8YStart = 23;
	uint8_t l_u8YSeparation = 10;
	bool l_isToggle = true;

	Graphics_clearDisplay(&task::Accelerometer::m_stStaticContext);

	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_LIGHT_BLUE);
	Graphics_Rectangle l_stRectangleBlue = {0, 0, 127, 63};
	Graphics_fillRectangle(&task::Accelerometer::m_stStaticContext,
	                       &l_stRectangleBlue);

	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_WHITE);
	Graphics_drawCircle(&task::Accelerometer::m_stStaticContext, 64, 64, 56);

	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_LIGHT_BLUE);
	Graphics_Rectangle l_stRectangleBlueRefill = {0, 38, 127, 63};
	Graphics_fillRectangle(&task::Accelerometer::m_stStaticContext,
	                       &l_stRectangleBlueRefill);

	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_PERU);
	Graphics_Rectangle l_stRectangleBrown = {0, 64, 127, 127};
	Graphics_fillRectangle(&task::Accelerometer::m_stStaticContext,
	                       &l_stRectangleBrown);

	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_WHITE);

	this->drawLine(0, 63, 127, 63);
	this->drawLine(0, 64, 127, 64);

	this->drawLine(63, 8, 63, 2);
	this->drawLine(64, 8, 64, 2);

	l_u8LongLineHalfLenght = 8;
	l_u8ShortLineHalfLenght = 4;
	l_u8XCenter = 64;
	l_u8YStart = 23;
	l_u8YSeparation = 10;
	l_isToggle = true;

	for (i = 0; i < 4; i++) {
		j = l_isToggle ? l_u8XCenter - l_u8LongLineHalfLenght
		               : l_u8XCenter - l_u8ShortLineHalfLenght;
		k = l_isToggle ? l_u8XCenter + l_u8LongLineHalfLenght
		               : l_u8XCenter + l_u8ShortLineHalfLenght;

		this->drawLine(j, l_u8YStart + l_u8YSeparation * i, k,
		               l_u8YStart + l_u8YSeparation * i);
		l_isToggle = !l_isToggle;
	}

	l_u8YStart = 10;
	for (i = 0; i < 5; i++) {
		this->drawLine(l_u8XCenter - i, l_u8YStart + i, l_u8XCenter + i,
		               l_u8YStart + i);
	}

	l_u8YStart = 74;
	l_isToggle = false;
	for (i = 0; i < 5; i++) {
		j = l_isToggle ? l_u8XCenter - l_u8LongLineHalfLenght
		               : l_u8XCenter - l_u8ShortLineHalfLenght;
		k = l_isToggle ? l_u8XCenter + l_u8LongLineHalfLenght
		               : l_u8XCenter + l_u8ShortLineHalfLenght;

		this->drawLine(j, l_u8YStart + l_u8YSeparation * i, k,
		               l_u8YStart + l_u8YSeparation * i);
		l_isToggle = !l_isToggle;
	}
	/**
	 * Circulo centrado en (h, k) de radio r:
	 *
	 * (x-h)^2+(y-k)^2=r^2
	 *
	 * Para x y y se tiene:
	 *
	 * x = +- sqrt(r^2 - (y-k)^2) + h
	 * y = +- sqrt(r^2 - (x-h)^2) + k
	 *
	 * a(x,y) = tan((y-k) / (x-h))
	 *
	 * a(y) = tan((y-k) / (+- sqrt(r^2 - (y-k)^2)))
	 * a(x) = tan((+- sqrt(r^2 - (x-h)^2)) / (x-h))
	 *
	 * y(a, x) = (x-h)*atan(a) + k
	 * x(a, y) = (y-k)/atan(a) + h
	 *
	 */

	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_WHITE);

	this->drawLine(114, 38, 119, 35);
	this->drawLine(114, 37, 119, 34);

	this->drawLine(14, 38, 9, 35);
	this->drawLine(14, 37, 9, 34);

	this->drawLine(35, 16, 32, 11);

	this->drawLine(93, 16, 96, 11);
}

void task::Accelerometer::printResults() {
	char string[10];
	Graphics_setBackgroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_PERU);
	Graphics_setForegroundColor(&task::Accelerometer::m_stStaticContext,
	                            GRAPHICS_COLOR_BLACK);
	sprintf(string, "X: %5d", this->m_u16X);
	Graphics_drawString(&task::Accelerometer::m_stStaticContext, (int8_t*)string,
	                    AUTO_STRING_LENGTH, 4, 100, OPAQUE_TEXT);
	sprintf(string, "Y: %5d", this->m_u16Y);
	Graphics_drawString(&task::Accelerometer::m_stStaticContext, (int8_t*)string,
	                    AUTO_STRING_LENGTH, 4, 110, OPAQUE_TEXT);
	sprintf(string, "Z: %5d", this->m_u16Z);
	Graphics_drawString(&task::Accelerometer::m_stStaticContext, (int8_t*)string,
	                    AUTO_STRING_LENGTH, 4, 120, OPAQUE_TEXT);
}
