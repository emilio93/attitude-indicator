#ifndef SCREEN_BACKGROUNDPAINTER_HPP_
#define SCREEN_BACKGROUNDPAINTER_HPP_

#include <cstddef>
#include <iostream>

//#include <ti/grlib/grlib.h>

//#include "peripheral/LcdScreen.hpp"

#include "attitude/State.hpp"

namespace peripheral {
class LcdScreen;
}

typedef struct Graphics_Context {
	int32_t size;
} Graphics_Context;

namespace screen {

class BackgroundPainter {
 public:
	BackgroundPainter(attitude::State* i_pState,
	                  Graphics_Context* i_pGraphicsContext,
	                  peripheral::LcdScreen* i_pLcdScreen);

	void paint(void);

	void setState(attitude::State* i_pState);

 private:
	attitude::State* getState(void);

	void setGraphicsContext(Graphics_Context* i_pGraphicsContext);
	Graphics_Context* getGraphicsContext(void);

	void setLcdScreen(peripheral::LcdScreen* i_pcdScreen);
	peripheral::LcdScreen* getLcdScreen(void);

	attitude::State* m_pState;

	Graphics_Context* m_pGraphicsContext;

	peripheral::LcdScreen* m_pLcdScreen;
};

}  // namespace screen

#endif /* SCREEN_BACKGROUNDPAINTER_HPP_ */
