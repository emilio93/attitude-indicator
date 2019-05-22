#ifndef MAIN_HPP_
#define MAIN_HPP_

// ~1ms with 48MHz clock.
// 48e6ticks/seg * 1e-3 seg = 48e3ticks
// The main Setup function for the application
#define TIMER32_COUNT 48e3

void Setup(void);
void debugSetup(void);

#endif /* MAIN_HPP_ */
