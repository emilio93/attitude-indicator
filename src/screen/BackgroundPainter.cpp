#include "screen/BackgroundPainter.hpp"

static int32_t getClipCode(int32_t x, int32_t y) {
	int32_t code;
	code = 0;
	uint8_t l_u8YMax = attitude::state::SCREEN_Y - 1;
	uint8_t l_u8XMax = attitude::state::SCREEN_X - 1;
	if (y < 0) {
		code |= 1;
	}
	if (y > l_u8YMax) {
		code |= 2;
	}
	if (x < 0) {
		code |= 4;
	}
	if (x > l_u8XMax) {
		code |= 8;
	}
	return (code);
}

static int32_t clipLine(int32_t* x1, int32_t* y1, int32_t* x2, int32_t* y2) {
	int32_t code, code1, code2, x, y;
	code1 = getClipCode(*x1, *y1);
	code2 = getClipCode(*x2, *y2);
	while (1) {
		if ((code1 == 0) && (code2 == 0)) {
			return (1);
		}
		if ((code1 & code2) != 0) {
			return (0);
		}
		if (code1) {
			code = code1;
		} else {
			code = code2;
		}
		if (code & 1) {
			x = (*x1 + (((*x2 - *x1) * (0 - *y1)) / (*y2 - *y1)));
			y = 0;
		} else if (code & 2) {
			x = (*x1 + (((*x2 - *x1) * ((attitude::state::SCREEN_Y - 1) - *y1)) /
			            (*y2 - *y1)));
			y = (attitude::state::SCREEN_Y - 1);
		} else if (code & 4) {
			x = 0;
			y = (*y1 + (((*y2 - *y1) * (0 - *x1)) / (*x2 - *x1)));
		} else {
			x = (attitude::state::SCREEN_X - 1);
			y = (*y1 + (((*y2 - *y1) * ((attitude::state::SCREEN_X - 1) - *x1)) /
			            (*x2 - *x1)));
		}
		if (code1) {
			*x1 = x;
			*y1 = y;
			code1 = getClipCode(x, y);
		} else {
			*x2 = x;
			*y2 = y;
			code2 = getClipCode(x, y);
		}
	}
}

screen::BackgroundPainter::BackgroundPainter(
    attitude::State* i_pState, Graphics_Context* i_pGraphicsContext,
    peripheral::LcdScreen* i_pLcdScreen) {
	this->setState(i_pState);
	this->setGraphicsContext(i_pGraphicsContext);
	this->setLcdScreen(i_pLcdScreen);
}

void screen::BackgroundPainter::paint() {
	Graphics_Context* l_pGraphicsContext = this->getGraphicsContext();

	int32_t l_u32AX = this->getState()->getPointAX();
	int32_t l_u32AY = this->getState()->getPointAY();
	int32_t l_u32BX = this->getState()->getPointBX();
	int32_t l_u32BY = this->getState()->getPointBY();
	clipLine(&l_u32AX, &l_u32AY, &l_u32BX, &l_u32BY);
	this->getState()->printInfo(true);
	std::cout << "\t\t\t\t\t\t\t\t(" << l_u32AX << "," << l_u32AY << ")   \t("
	          << l_u32BX << "," << l_u32BY
	          << ")\tb = " << (uint16_t)this->getState()->getB() << "\n\n";
}

void screen::BackgroundPainter::setState(attitude::State* i_pState) {
	this->m_pState = i_pState;
}
attitude::State* screen::BackgroundPainter::getState(void) {
	return this->m_pState;
}

void screen::BackgroundPainter::setGraphicsContext(
    Graphics_Context* i_pGraphicsContext) {
	this->m_pGraphicsContext = i_pGraphicsContext;
}

Graphics_Context* screen::BackgroundPainter::getGraphicsContext(void) {
	return this->m_pGraphicsContext;
}

void screen::BackgroundPainter::setLcdScreen(
    peripheral::LcdScreen* i_pLcdScreen) {
	this->m_pLcdScreen = i_pLcdScreen;
}

peripheral::LcdScreen* screen::BackgroundPainter::getLcdScreen(void) {
	return this->m_pLcdScreen;
}

#ifdef TEST_SCREEN_BACKGROUNDPAINTER
/**
 * To compile, execute from base dir
 * g++ -O0 -g -Iinclude -DTEST_SCREEN_BACKGROUNDPAINTER src/attitude/State.cpp
 * src/screen/BackgroundPainter.cpp
 *
 * Test with valgrind for leaks
 * valgrind --leak-check=yes ./a.out
 *
 * To execute
 * ./a.out
 *
 */
int main() {
	int i = -25;
	int i_u16Step = 100;
	screen::BackgroundPainter* l_pBackgroundPainter =
	    new screen::BackgroundPainter(NULL, NULL, NULL);
	while (i++ < 25) {
		attitude::State* l_pState =
		    new attitude::State(8192 + i_u16Step * i, 8192 - i_u16Step * i);
		l_pBackgroundPainter->setState(l_pState);
		l_pBackgroundPainter->paint();
		delete l_pState;
	}
	delete l_pBackgroundPainter;
	return 0;
}
#endif
