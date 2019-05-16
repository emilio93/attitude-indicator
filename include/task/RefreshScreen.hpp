#ifndef TASK_REFRESHSCREEN_HPP_
#define TASK_REFRESHSCREEN_HPP_

#include <cstddef>

#include <ti/grlib/grlib.h>
extern "C" {
#include <Crystalfontz128x128_ST7735.h>
}

#include "scheduler/Task.hpp"

namespace task {

class RefreshScreen : public scheduler::Task {
 public:

	/**
	 * Checks every position of the screen for changes. If changes exist, apply
	 * them.
	 *
	 * A drawable object provides a method to check if the color of a given pixel
	 * has changed and applies the change if present.
	 *
	 * @return uint8_t
	 */
	virtual uint8_t run(void);
	virtual uint8_t setup(void);

 protected:
 private:
};

}  // namespace task

#endif /* TASK_REFRESHSCREEN_HPP_ */
