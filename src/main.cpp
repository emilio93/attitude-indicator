#include "msp.h"

#include "mkii/Led.hpp"
#include "main.hpp"
#include "task/LED.hpp"
#include "scheduler/Scheduler.hpp"
#include "scheduler/Task.hpp"

// ##########################
// Global/Static declarations
// ##########################
uint8_t scheduler::Task::m_u8NextTaskID = 0;            // - Init task ID
volatile static uint64_t g_SystemTicks = 0;  // - The system counter.
scheduler::Scheduler g_MainScheduler;                   // - Instantiate a Scheduler

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
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

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
	// - Configure Timer32_1  with MCLK (3Mhz), Division by 1, Enable the
	// interrupt, Periodic Mode
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts
	TIMER32_1->LOAD = TIMER32_COUNT;  //~1ms ---> a 3Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 |
	                     TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE |
	                     TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn, 1);
	NVIC_EnableIRQ(T32_INT1_IRQn);
	__enable_irq();

	return;
}

extern "C" {
// - Handle the Timer32 Interrupt
void T32_INT1_IRQHandler(void) {
	TIMER32_1->INTCLR = 0U;
	mkii::Led* l_pRedLed = new mkii::Led(peripheral::gpio::Port::PORT1,
	                                     peripheral::gpio::Pin::PIN0, false);
	l_pRedLed->Toggle();
	g_SystemTicks++;
	delete l_pRedLed;
	l_pRedLed = NULL;
	return;
}
}
