#include "msp.h"

#include <ti/grlib/grlib.h>
#include "mkii/Accelerometer.hpp"
#include "mkii/Led.hpp"
#include "mkii/Timer.hpp"

#include "main.hpp"
#include "scheduler/Scheduler.hpp"
#include "scheduler/Task.hpp"
#include "task/Accelerometer.hpp"
#include "task/LED.hpp"
#include "task/RefreshScreenBackground.hpp"

// ##########################
// Global/Static declarations
// ##########################
uint8_t scheduler::Task::m_u8NextTaskID = 0;  // - Init task ID
volatile static uint64_t g_SystemTicks = 0;   // - The system counter.
scheduler::Scheduler g_MainScheduler;         // - Instantiate a Scheduler

mkii::Led* g_pRedLed =
    new mkii::Led(peripheral::gpio::Port::PORT1, peripheral::gpio::Pin::PIN0);

Graphics_Context* g_pContext;

void T32_INT1_IRQHandler(void);

// #########################
//          MAIN
// #########################
void main(void) {
	mkii::Accelerometer* l_pAccelerometer = new mkii::Accelerometer();
	task::Accelerometer* l_pAccelerometerTask =
	    new task::Accelerometer(l_pAccelerometer);

	peripheral::LcdScreen* l_pLcdScreen =
	    new peripheral::LcdScreen(new peripheral::lcdScreen::Spi());
	task::RefreshScreenBackground* l_pRefreshScreenBackground =
	    new task::RefreshScreenBackground(l_pLcdScreen, g_pContext);

	g_MainScheduler.attach(l_pAccelerometerTask, 10);
	g_MainScheduler.attach(l_pRefreshScreenBackground, 30);

	g_MainScheduler.setup();

	Setup();

	// - Main Loop
	while (1) {
		__wfe();  // Wait for Event
		if (g_SystemTicks != g_MainScheduler.m_u64ticks) {
			//- Only execute the tasks if one tick has passed.
			g_MainScheduler.m_u64ticks = g_SystemTicks;
			g_MainScheduler.run();
		}
	}
}

// **********************************
// Setup function for the application
// @input - none
// @output - none
// **********************************
void Setup(void) {
	WDT_A_holdTimer();
	PCM_setCoreVoltageLevel(PCM_VCORE1);
	FlashCtl_setWaitState(FLASH_BANK0, 2);
	FlashCtl_setWaitState(FLASH_BANK1, 2);

	/* Initializes Clock System */
	CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
	CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

	/* Enable Interrupts */
	MAP_Interrupt_enableMaster();

	g_pRedLed->SetState(false);

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer32
	mkii::Timer* l_pTimer =
	    mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0);
	l_pTimer->SetCounter(TIMER32_COUNT);
	l_pTimer->SetInterrupt(T32_INT1_IRQHandler);

	return;
}

// - Handle the Timer32 Interrupt
void T32_INT1_IRQHandler(void) {
	mkii::Timer* l_pTimer =
	    mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0);
	l_pTimer->EndInterrupt();
	g_SystemTicks++;
	l_pTimer->SetCounter(TIMER32_COUNT);
	l_pTimer->SetInterrupt(T32_INT1_IRQHandler);
	return;
}
