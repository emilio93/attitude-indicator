#include "msp.h"

#include "mkii/Led.hpp"
#include "mkii/Timer.hpp"

#include "main.hpp"
#include "scheduler/Scheduler.hpp"
#include "scheduler/Task.hpp"
#include "task/LED.hpp"

// ##########################
// Global/Static declarations
// ##########################
uint8_t scheduler::Task::m_u8NextTaskID = 0;  // - Init task ID
volatile static uint64_t g_SystemTicks = 0;   // - The system counter.
scheduler::Scheduler g_MainScheduler;         // - Instantiate a Scheduler

void T32_INT1_IRQHandler(void);

// #########################
//          MAIN
// #########################
void main(void) {
	// Instantiate two new Led devices
	mkii::Led* l_pGreenLed =
	    new mkii::Led(peripheral::gpio::Port::PORT2, peripheral::gpio::Pin::PIN1);
	mkii::Led* l_pBlueLed =
	    new mkii::Led(peripheral::gpio::Port::PORT2, peripheral::gpio::Pin::PIN2);

	// Instantiate two new Led tasks
	task::LED* l_pBlueLedTask = new task::LED(l_pBlueLed);
	task::LED* l_pGreenLedTask = new task::LED(l_pGreenLed);
	// - Run the overall setup function for the system
	Setup();
	// - Attach the Tasks to the Scheduler;
	g_MainScheduler.attach(l_pBlueLedTask, 500);
	// g_MainScheduler.attach(l_pGreenLedTask, 300);

	// - Run the Setup for the scheduler and all tasks
	g_MainScheduler.setup();
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
	// ****************************
	//         DEVICE CONFIG
	// ****************************
	// - Disable WDT
	MAP_WDT_A_holdTimer();

	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	// - This is the heart beat indicator.
	mkii::Led* l_pRedLed =
	    new mkii::Led(peripheral::gpio::Port::PORT1, peripheral::gpio::Pin::PIN0);
	delete l_pRedLed;
	l_pRedLed = NULL;

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer32
	MAP_Interrupt_enableMaster();
	mkii::Timer* l_pTimer =
	    mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0);
	l_pTimer->SetCounter(TIMER32_COUNT);
	l_pTimer->SetInterrupt(T32_INT1_IRQHandler);

	return;
}

// - Handle the Timer32 Interrupt
void T32_INT1_IRQHandler(void) {
	mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0)->EndInterrupt();
	mkii::Led* l_pRedLed = new mkii::Led(peripheral::gpio::Port::PORT1,
	                                     peripheral::gpio::Pin::PIN0, false);
	l_pRedLed->Toggle();
	g_SystemTicks++;
	delete l_pRedLed;
	l_pRedLed = NULL;
	mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0)
	    ->SetCounter(TIMER32_COUNT);
	mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0)
	    ->SetInterrupt(T32_INT1_IRQHandler);
	return;
}
