#include <iostream>

#include "debug/printf.hpp"
#include "task/RefreshScreenBackground.hpp"

Graphics_Context* task::RefreshScreenBackground::m_pContext;

task::RefreshScreenBackground::RefreshScreenBackground(
    attitude::State* i_pState, peripheral::LcdScreen* i_pLcdScreen,
    Graphics_Context* i_pContext) {
	this->setState(i_pState);
	this->setLcdScreen(i_pLcdScreen);
	this->setContext(i_pContext);
	this->m_pOldState =
	    new attitude::State(this->getState()->getAccelerometerZ(),
	                        this->getState()->getAccelerometerX());
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
	// this->repaintScreen();
	this->testLines();
	return NO_ERR;
}

void task::RefreshScreenBackground::testLines() {
	Graphics_Context* l_pGraphicsContext = this->getContext();
	// Graphics_Rectangle l_stRectBlue = {0, 0, 127, 127};
	// Graphics_setBackgroundColor(l_pGraphicsContext, GRAPHICS_COLOR_WHITE);

	uint8_t nPixel = 127;  // [ 0 - 127 ]
	int16_t div_temp;      //=127/(128-nPixel);

	this->getState()->setAccelerometerX(this->m_u16X);
	this->getState()->setAccelerometerZ(this->m_u16Z);

	// this->getState()->setCaseX();
	// this->getState()->setM();
	// this->getState()->setB(64);
	if (this->getState()->getPointAY() == this->getState()->getPointBY()) {
return;
}
	int32_t lineH[128] = {0};
	int32_t oldLineH[128] = {0};
	this->getState()->getLineH(lineH);
	this->getOldState()->getLineH(oldLineH);

	if (this->getState()->getPointAY() < this->getState()->getPointBY()) {
		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_PERU);
		for (int y = 0; y < 128; y++) {
			Graphics_drawLineH(l_pGraphicsContext, 0, lineH[y], y);
		}
		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_LIGHT_BLUE);
		for (int y = 0; y < 128; y++) {
			Graphics_drawLineH(l_pGraphicsContext, lineH[y], 128, y);
		}
	} else {

		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_LIGHT_BLUE);
		for (int y = 0; y < 128; y++) {
			Graphics_drawLineH(l_pGraphicsContext, 0, lineH[y], y);
		}
		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_PERU);
		for (int y = 0; y < 128; y++) {
			Graphics_drawLineH(l_pGraphicsContext, lineH[y], 128, y);
		}

	}


		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_BLACK);
		Graphics_drawLine(l_pGraphicsContext, this->getState()->getPointAX(),
		                  this->getState()->getPointAY(),
		                  this->getState()->getPointBX(),
		                  this->getState()->getPointBY());
	// delete[] lineH;

	// Graphics_drawLine(l_pGraphicsContext, this->getState()->getPointAX(),
	// 		  this->getState()->getPointAY(),
	// 		  this->getState()->getPointBX(),
	// 		  this->getState()->getPointBY());

	// State::line_point temp_point;
	// Graphics_drawLine(l_pGraphicsContext, p1_x, p1_y, p2_x, p2_y);

	// 	while (true) {
	// 		for (uint16_t i = 0; i < 0xFFFF; i++) {
	// 			for (uint16_t j = 0; j < 0x000F; j++) {
	// 			}
	// 		}
	// 		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_WHITE);
	// 		Graphics_fillRectangle(l_pGraphicsContext, &l_stRectBlue);
	// 		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_BLACK);

	// 		nPixel--;
	// 		div_temp = (127 / (128 - nPixel));
	// 		for (int i = 0; i <= 128; i++) {
	// 			Graphics_drawLineH(l_pGraphicsContext, p1_x, i - (i / div_temp),
	// 			                   p1_y - i);
	// 		}
	// 	}
}

void task::RefreshScreenBackground::repaintScreen() {
	uint16_t l_u16Threshold = attitude::state::ADC_Z_MID_VALUE;

	uint16_t l_u16LimitThreshold = attitude::state::ADC_Z_MAX_VARIATION;
	uint16_t l_u16TopLimit = l_u16Threshold + l_u16LimitThreshold;
	uint16_t l_u16BottomLimit = l_u16Threshold - l_u16LimitThreshold;

	uint16_t l_u16MinDelta = 30;
	uint16_t l_u16Divider = attitude::state::ADC_Z_SCREEN_FACTOR;

	uint16_t l_u16Dif;
	uint16_t l_u16Height;

	Graphics_Rectangle l_stRectBlue;
	Graphics_Rectangle l_stRectBrown;
	if (this->m_u16Z > l_u16Threshold) {
		l_u16Dif = (this->m_u16Z - l_u16Threshold) / l_u16Divider;
		l_u16Height = (attitude::state::SCREEN_Y / 2) - l_u16Dif;
	} else {
		l_u16Dif = (l_u16Threshold - this->m_u16Z) / l_u16Divider;
		l_u16Height = (attitude::state::SCREEN_Y / 2) + l_u16Dif;
	}

	if ((this->m_u16Z > this->m_u16OldZValue &&
	     (this->m_u16Z - this->m_u16OldZValue) > l_u16MinDelta) ||
	    (this->m_u16Z < this->m_u16OldZValue &&
	     (this->m_u16OldZValue - this->m_u16Z) > l_u16MinDelta)) {
		this->m_u16OldZValue = this->m_u16Z;
		if (this->m_u16Z > l_u16TopLimit) {
			l_u16Height = 0;
		} else if (this->m_u16Z < l_u16BottomLimit) {
			l_u16Height = attitude::state::SCREEN_Y;
		}

		this->getState()->setAccelerometerX(this->m_u16X);
		this->getState()->setAccelerometerZ(this->m_u16Z);

		this->getState()->setB(this->getState()->calculateB());
		this->getState()->setM(this->getState()->calculateM());

		Graphics_Context* l_pGraphicsContext = this->getContext();
		l_stRectBlue = {0, 0, attitude::state::SCREEN_X, (uint8_t)l_u16Height};
		l_stRectBrown = {0, attitude::state::SCREEN_Y, attitude::state::SCREEN_X,
		                 (uint8_t)l_u16Height};
		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_LIGHT_BLUE);
		Graphics_fillRectangle(l_pGraphicsContext, &l_stRectBlue);
		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_PERU);
		Graphics_fillRectangle(l_pGraphicsContext, &l_stRectBrown);

		Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_BLACK);
		Graphics_drawLine(l_pGraphicsContext, this->getState()->getPointAX(),
		                  this->getState()->getPointAY(),
		                  this->getState()->getPointBX(),
		                  this->getState()->getPointBY());
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

attitude::State* task::RefreshScreenBackground::getState(void) {
	return this->m_pState;
}

attitude::State* task::RefreshScreenBackground::getOldState(void) {
	return this->m_pOldState;
}

void task::RefreshScreenBackground::setState(attitude::State* i_pState) {
	this->m_pState = i_pState;
}
