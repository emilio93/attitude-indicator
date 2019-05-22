#include <iostream>

#include "task/RefreshScreenRollIndicator.hpp"

Graphics_Context* task::RefreshScreenRollIndicator::m_pContextArrow;
Graphics_Context* task::RefreshScreenRollIndicator::m_pContextCircle;

task::RefreshScreenRollIndicator::RefreshScreenRollIndicator(
    peripheral::LcdScreen* i_pLcdScreen, Graphics_Context* i_pContextA,
    Graphics_Context* i_pContextC) {
	this->setLcdScreen(i_pLcdScreen);
	this->setContextArrow(i_pContextA);
	this->setContextCircle(i_pContextC);
}

uint8_t task::RefreshScreenRollIndicator::setup(void) {
	this->getLcdScreen()->Init();
	this->getLcdScreen()->SetOrientation(peripheral::lcdScreen::ORIENTATION_UP);
	// Initialize a object
	Graphics_Context* l_pGraphicsContextCircle = new Graphics_Context;
	Graphics_Context* l_pGraphicsContextArrow = new Graphics_Context;
	// Set the context
	this->setContextCircle(l_pGraphicsContextCircle);
	this->setContextArrow(l_pGraphicsContextArrow);
	Graphics_initContext(l_pGraphicsContextCircle,
	                     peripheral::LcdScreen::GetCrystalfontz128x128(),
	                     peripheral::LcdScreen::GetCrystalfontz128x128_funcs());
	Graphics_initContext(l_pGraphicsContextArrow,
	                     peripheral::LcdScreen::GetCrystalfontz128x128(),
	                     peripheral::LcdScreen::GetCrystalfontz128x128_funcs());
	Graphics_setBackgroundColor(l_pGraphicsContextCircle, GRAPHICS_COLOR_WHITE);
	Graphics_setBackgroundColor(l_pGraphicsContextArrow, GRAPHICS_COLOR_WHITE);

	this->run();
	return NO_ERR;
}

uint8_t task::RefreshScreenRollIndicator::run(void) {
	this->m_u16X = mkii::Accelerometer::GetX();

	this->repaintRollIndicator();
	this->paintArrow_0(63, 8);
	this->paintArrow_60(20, 26);
	this->paintArrow_30(10, 10);
	return NO_ERR;
}

void task::RefreshScreenRollIndicator::repaintRollIndicator() {
	Graphics_Context* l_pGraphicsContext = this->getContextCircle();
	Graphics_Rectangle l_stRectClimbArea;
	l_stRectClimbArea = {0, 0, 127, 36};
	Graphics_setClipRegion(l_pGraphicsContext, &l_stRectClimbArea);

	/*
	 * Build roll indicator on screen
	 */
	Graphics_setBackgroundColor(l_pGraphicsContext, GRAPHICS_COLOR_WHITE);
	Graphics_setForegroundColor(l_pGraphicsContext, GRAPHICS_COLOR_BLACK);
	// Graphics_drawPixel(l_pGraphicsContext, 10,10);
	Graphics_drawCircle(l_pGraphicsContext, 64, 64, 56);

	// Build indicator standar lines
	for (int i = 0; i < 12; i++) {
		Graphics_drawLine(l_pGraphicsContext, DRAWLINERAW[i * 4],
		                  DRAWLINERAW[i * 4 + 1], DRAWLINERAW[i * 4 + 2],
		                  DRAWLINERAW[i * 4 + 3]);
	}
	// Draw 0° from vertical arrow
	for (int i = 6; i > 0; i--) {
		Graphics_drawLine(l_pGraphicsContext, 64 + (i >> 1), 8 - i, 64 - (i >> 1),
		                  8 - i);
	}
	// Draw 45°,vertical arrow
	for (int i = 0; i < 4; i++) {
		Graphics_drawLine(l_pGraphicsContext, 24 - i, 24, 24, 24 - i);
		Graphics_drawLine(l_pGraphicsContext, 104 + i, 24, 104, 24 - i);
	}
	return;
}

void task::RefreshScreenRollIndicator::setLcdScreen(
    peripheral::LcdScreen* i_pLcdScreen) {
	this->m_pLcdScreen = i_pLcdScreen;
}

peripheral::LcdScreen* task::RefreshScreenRollIndicator::getLcdScreen(void) {
	return this->m_pLcdScreen;
}

void task::RefreshScreenRollIndicator::setContextArrow(
    Graphics_Context* i_pContext) {
	this->m_pContextCircle = i_pContext;
}

void task::RefreshScreenRollIndicator::setContextCircle(
    Graphics_Context* i_pContext) {
	this->m_pContextArrow = i_pContext;
}

Graphics_Context* task::RefreshScreenRollIndicator::getContextArrow(void) {
	return task::RefreshScreenRollIndicator::m_pContextArrow;
}

Graphics_Context* task::RefreshScreenRollIndicator::getContextCircle(void) {
	return task::RefreshScreenRollIndicator::m_pContextCircle;
}

void task::RefreshScreenRollIndicator::paintArrow_0(uint16_t x, uint16_t y) {
	// uint16_t l_u16XOffset = 0;
	uint16_t l_u16YOffset = 2;
	Graphics_Context* l_pGraphicsContext = this->getContextArrow();
	Graphics_Rectangle l_stRectClimbArea;
	l_stRectClimbArea = {0, 0, 127, 64};
	Graphics_setClipRegion(l_pGraphicsContext, &l_stRectClimbArea);

	// Draw 0° from vertical arrow
	for (int i = 0; i < 6; i++) {
		Graphics_drawLine(l_pGraphicsContext, x + (i >> 1), y + i + l_u16YOffset,
		                  x - (i >> 1), y + i + l_u16YOffset);
	}
	return;
}

void task::RefreshScreenRollIndicator::paintArrow_30(uint16_t x, uint16_t y) {
	return;
}

void task::RefreshScreenRollIndicator::paintArrow_60(uint16_t x, uint16_t y) {
	uint16_t l_u16XOffset = 2;
	uint16_t l_u16YOffset = 2;
	Graphics_Context* l_pGraphicsContext = this->getContextArrow();
	Graphics_Rectangle l_stRectClimbArea;
	l_stRectClimbArea = {0, 0, 127, 64};
	Graphics_setClipRegion(l_pGraphicsContext, &l_stRectClimbArea);

	// Draw 0° from vertical arrow
	for (int i = 0; i < 5; i++) {
		Graphics_drawLine(l_pGraphicsContext, x + l_u16XOffset + i,
		                  y + l_u16YOffset + (i >> 1), x + l_u16XOffset + 1 + i,
		                  y + l_u16YOffset + 4 - (i >> 1));
	}
	return;
}
