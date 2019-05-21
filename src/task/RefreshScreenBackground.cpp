#include <iostream>

#include "task/RefreshScreenBackground.hpp"

Graphics_Context* task::RefreshScreenBackground::m_pContext;

task::RefreshScreenBackground::RefreshScreenBackground(
    peripheral::LcdScreen* i_pLcdScreen, Graphics_Context* i_pContext) {
	this->setLcdScreen(i_pLcdScreen);
	this->setContext(i_pContext);
}

uint8_t task::RefreshScreenBackground::setup(void) {
	this->getLcdScreen()->Init();
	this->getLcdScreen()->SetOrientation(peripheral::lcdScreen::ORIENTATION_UP);
	// Initialize a object
	Graphics_Context* l_pGraphicsContext = new Graphics_Context;
	// Set the context
	this->setContext(l_pGraphicsContext);
	Graphics_initContext(l_pGraphicsContext,
	                     peripheral::LcdScreen::GetCrystalfontz128x128(),
	                     peripheral::LcdScreen::GetCrystalfontz128x128_funcs());
	Graphics_setBackgroundColor(l_pGraphicsContext, GRAPHICS_COLOR_WHITE);
	GrContextFontSet(l_pGraphicsContext, &g_sFontFixed6x8);
	this->run();
	return NO_ERR;
}

uint8_t task::RefreshScreenBackground::run(void) {
	this->m_u16X = mkii::Accelerometer::GetX();
	this->m_u16Y = mkii::Accelerometer::GetY();
	this->m_u16Z = mkii::Accelerometer::GetZ();
	this->repaintScreen();
	return NO_ERR;
}

void task::RefreshScreenBackground::repaintScreen() {
	uint16_t l_u16Threshold = 8192;

	uint16_t l_u16LimitThreshold = 3258;
	uint16_t l_u16TopLimit = l_u16Threshold + l_u16LimitThreshold;
	uint16_t l_u16BottomLimit = l_u16Threshold - l_u16LimitThreshold;

	uint16_t l_u16MinDelta = 30;
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

	if ((this->m_u16Z > this->m_u16OldZValue &&
	     (this->m_u16Z - this->m_u16OldZValue) > l_u16MinDelta) ||
	    (this->m_u16Z < this->m_u16OldZValue &&
	     (this->m_u16OldZValue - this->m_u16Z) > l_u16MinDelta)) {
		this->m_u16OldZValue = this->m_u16Z;
		if (this->m_u16Z > l_u16TopLimit) {
			l_u16Height = 0;
		} else if (this->m_u16Z < l_u16BottomLimit) {
			l_u16Height = 127;
		}

		Graphics_Context* l_pGraphicsContext = this->getContext();
		l_stRectBlue = {0, 0, 127, (uint8_t)l_u16Height};
		l_stRectBrown = {0, 127, 127, (uint8_t)l_u16Height};
		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_LIGHT_BLUE);
		Graphics_fillRectangle(l_pGraphicsContext, &l_stRectBlue);
		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_PERU);
		Graphics_fillRectangle(l_pGraphicsContext, &l_stRectBrown);

		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_WHITE);
		Graphics_drawLine(l_pGraphicsContext, 0, l_u16Height, 127, l_u16Height);

		int i, maxi;
		uint8_t j, k;
		uint8_t l_u8LongLineHalfLenght = 8;
		uint8_t l_u8ShortLineHalfLenght = 4;
		uint8_t l_u8XCenter = 64;
		uint8_t l_u8YStart = l_u16Height;
		uint8_t l_u8YSeparation = 10;
		bool l_isToggle = true;

		l_u8YStart = 23;
		maxi = 4;

		for (i = 0; i < maxi; i++) {
			j = l_isToggle ? l_u8XCenter - l_u8LongLineHalfLenght
			               : l_u8XCenter - l_u8ShortLineHalfLenght;
			k = l_isToggle ? l_u8XCenter + l_u8LongLineHalfLenght
			               : l_u8XCenter + l_u8ShortLineHalfLenght;

			Graphics_drawLine(l_pGraphicsContext, j, l_u8YStart + l_u8YSeparation * i,
			                  k, l_u8YStart + l_u8YSeparation * i);
			l_isToggle = !l_isToggle;
		}

		l_u8YStart = 74;
		l_isToggle = false;
		maxi = 5;
		for (i = 0; i < maxi; i++) {
			j = l_isToggle ? l_u8XCenter - l_u8LongLineHalfLenght
			               : l_u8XCenter - l_u8ShortLineHalfLenght;
			k = l_isToggle ? l_u8XCenter + l_u8LongLineHalfLenght
			               : l_u8XCenter + l_u8ShortLineHalfLenght;

			Graphics_drawLine(l_pGraphicsContext, j, l_u8YStart + l_u8YSeparation * i,
			                  k, l_u8YStart + l_u8YSeparation * i);
			l_isToggle = !l_isToggle;
		}
	}
}

void task::RefreshScreenBackground::setLcdScreen(
    peripheral::LcdScreen* i_pLcdScreen) {
	this->m_pLcdScreen = i_pLcdScreen;
}

peripheral::LcdScreen* task::RefreshScreenBackground::getLcdScreen(void) {
	return this->m_pLcdScreen;
}

void task::RefreshScreenBackground::setContext(Graphics_Context* i_pContext) {
	this->m_pContext = i_pContext;
}

Graphics_Context* task::RefreshScreenBackground::getContext(void) {
	return task::RefreshScreenBackground::m_pContext;
}
