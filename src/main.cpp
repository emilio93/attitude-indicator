#include "msp.h"

#include <ti/grlib/grlib.h>
extern "C" {
#include <Crystalfontz128x128_ST7735.h>
}

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

mkii::Led* g_pRedLed =
    new mkii::Led(peripheral::gpio::Port::PORT1, peripheral::gpio::Pin::PIN0);

Graphics_Context g_sContext;

void T32_INT1_IRQHandler(void);

// #########################
//          MAIN
// #########################
void main(void) {
	// Instantiate two new Led devices
	// mkii::Led* l_pGreenLed =
	//     new mkii::Led(peripheral::gpio::Port::PORT2, peripheral::gpio::Pin::PIN1);
	mkii::Led* l_pBlueLed =
	    new mkii::Led(peripheral::gpio::Port::PORT2, peripheral::gpio::Pin::PIN2);

	// Instantiate two new Led tasks
	// task::LED* l_pGreenLedTask = new task::LED(l_pGreenLed);
	task::LED* l_pBlueLedTask = new task::LED(l_pBlueLed);
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

    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

	/* Initializes Clock System */
	MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    Graphics_clearDisplay(&g_sContext);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_BLUE);
    Graphics_Rectangle l_stRectangleBlue = {0, 0, 127, 63};
    Graphics_fillRectangle(&g_sContext, &l_stRectangleBlue);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawCircle(&g_sContext, 64, 64, 56);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_BLUE);
    Graphics_Rectangle l_stRectangleBlueRefill = {0, 38, 127, 63};
    Graphics_fillRectangle(&g_sContext, &l_stRectangleBlueRefill);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_PERU);
    Graphics_Rectangle l_stRectangleBrown = {0, 64, 127, 127};
    Graphics_fillRectangle(&g_sContext, &l_stRectangleBrown);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    Graphics_drawLine(&g_sContext, 53, 23, 74, 23);
    Graphics_drawLine(&g_sContext, 58, 33, 69, 33);
    Graphics_drawLine(&g_sContext, 53, 43, 74, 43);
    Graphics_drawLine(&g_sContext, 58, 53, 69, 53);
    Graphics_drawLine(&g_sContext, 0, 64, 127, 64);
    Graphics_drawLine(&g_sContext, 58, 75, 69, 75);
    Graphics_drawLine(&g_sContext, 53, 85, 74, 85);
    Graphics_drawLine(&g_sContext, 58, 95, 69, 95);
    Graphics_drawLine(&g_sContext, 53, 105, 74, 105);
    Graphics_drawLine(&g_sContext, 58, 115, 69, 115);

    Graphics_drawLine(&g_sContext, 63, 8, 63, 2);
    Graphics_drawLine(&g_sContext, 64, 8, 64, 2);
    /**
     * Circulo centrado en (h, k) de radio r:
     *
     * (x-h)^2+(y-k)^2=r^2
     *
     * Para x y y se tiene:
     *
     * x = +- sqrt(r^2 - (y-k)^2) + h
     * y = +- sqrt(r^2 - (x-h)^2) + k
     *
     * a(x,y) = tan((y-k) / (x-h))
     *
     * a(y) = tan((y-k) / (+- sqrt(r^2 - (y-k)^2)))
     * a(x) = tan((+- sqrt(r^2 - (x-h)^2)) / (x-h))
     *
     * y(a, x) = (x-h)*atan(a) + k
     * x(a, y) = (y-k)/atan(a) + h
     *
     */

    Graphics_drawLine(&g_sContext, 114, 38, 119, 35);
    Graphics_drawLine(&g_sContext, 114, 37, 119, 34);

    Graphics_drawLine(&g_sContext, 14, 38, 9, 35);
    Graphics_drawLine(&g_sContext, 14, 37, 9, 34);

    Graphics_drawLine(&g_sContext, 35, 16, 32, 11);

    Graphics_drawLine(&g_sContext, 93, 16, 96, 11);

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
	mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0)->EndInterrupt();
	g_pRedLed->Toggle();
	g_SystemTicks++;
	mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0)
	    ->SetCounter(TIMER32_COUNT);
	mkii::Timer::GetTimer(mkii::timer::TimerTypes::TIMER_32_0)
	    ->SetInterrupt(T32_INT1_IRQHandler);
	return;
}
