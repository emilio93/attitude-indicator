#include <iostream>

#include "task/RefreshScreenBackground.hpp"

task::RefreshScreenBackground::RefreshScreenBackground(
    peripheral::LcdScreen* i_pLcdScreen, Graphics_Context i_stContext) {
	this->setLcdScreen(i_pLcdScreen);
	this->setContext(i_stContext);
}

uint8_t task::RefreshScreenBackground::setup(void) {
	this->getLcdScreen()->Init();
	this->getLcdScreen()->SetOrientation(peripheral::lcdScreen::ORIENTATION_UP);
	Graphics_initContext(&this->getContext(),
	                     &peripheral::LcdScreen::GetCrystalfontz128x128(),
	                     &peripheral::LcdScreen::GetCrystalfontz128x128_funcs());
	Graphics_setBackgroundColor(&this->getContext(), GRAPHICS_COLOR_WHITE);
	GrContextFontSet(&this->getContext(), &g_sFontFixed6x8);
}

uint8_t task::RefreshScreenBackground::run(void) {
	uint16_t l_u16Threshold = 512;

	uint16_t l_u16LimitThreshold = 208;
	uint16_t l_u16TopLimit = l_u16Threshold + l_u16LimitThreshold;
	uint16_t l_u16BottomLimit = l_u16Threshold - l_u16LimitThreshold;

	uint16_t l_u16MinDelta = 5;
	uint16_t l_u16Divider = 4;

	uint16_t l_u16Dif;
	uint16_t l_u16Height;

	this->m_u16X = mkii::Accelerometer::GetX();
	this->m_u16Y = mkii::Accelerometer::GetY();
	this->m_u16Z = mkii::Accelerometer::GetZ();

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

		l_stRectBlue = {0, 0, 127, l_u16Height};
		l_stRectBrown = {0, 127, 127, l_u16Height};
		Graphics_setForegroundColor(&this->getContext(), GRAPHICS_COLOR_LIGHT_BLUE);
		Graphics_fillRectangle(&this->getContext(), &l_stRectBlue);
		Graphics_setForegroundColor(&this->getContext(), GRAPHICS_COLOR_PERU);
		Graphics_fillRectangle(&this->getContext(), &l_stRectBrown);

		Graphics_setForegroundColor(&this->getContext(), GRAPHICS_COLOR_WHITE);
		Graphics_drawLine(&this->getContext(), 0, l_u16Height, 127, l_u16Height);

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

			Graphics_drawLine(&this->getContext(), j,
			                  l_u8YStart + l_u8YSeparation * i, k,
			                  l_u8YStart + l_u8YSeparation * i);
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

			Graphics_drawLine(&this->getContext(), j,
			                  l_u8YStart + l_u8YSeparation * i, k,
			                  l_u8YStart + l_u8YSeparation * i);
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

void task::RefreshScreenBackground::setContext(Graphics_Context i_stContext) {
	this->m_stContext = i_stContext;
}

Graphics_Context task::RefreshScreenBackground::getContext(void) {
	return this->m_stContext;
}
