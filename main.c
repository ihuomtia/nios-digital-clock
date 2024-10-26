/*
 * main.c
 *
 *  Created on: Oct 25, 2024
 *      Author: ihuomtia
 */

#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "custom_hal/inc/pushbuttons.h"
#include "custom_hal/inc/seven_segment_display.h"

#include "system.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"

#include <unistd.h>

#define MAX_SECONDS 86400 // number of seconds in a day cycle

#define NORMAL_MODE 0x0
#define CONFIGURE_MINUTES_MODE 0x1
#define CONFIGURE_HOURS_MODE 0x2

static alt_u32 seconds;
static alt_u8 mode;

static void timer_isr(void * context, alt_u32 id) {
	// clear the interrupt
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0x0);

	// do clock stuff ...
	if (mode == NORMAL_MODE) {
		seconds += 1;
		if (seconds > MAX_SECONDS) seconds = 0;
	}
}

void init_timer_interrupt( void ) {
	// register interrupt
	alt_ic_isr_register(TIMER_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_IRQ, (void *) timer_isr, NULL, 0x0);

	// start timer
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, ALTERA_AVALON_TIMER_CONTROL_ITO_MSK
												| ALTERA_AVALON_TIMER_CONTROL_CONT_MSK
												| ALTERA_AVALON_TIMER_CONTROL_START_MSK);
}

void handle_keys() {
	// toggle config mode
	if (on_key_released(KEY_0)) {
		if (mode == NORMAL_MODE) {
			mode = CONFIGURE_HOURS_MODE;
			IOWR_8DIRECT(LEDS_BASE, 0, 0b11110000);
		} else {
			mode = NORMAL_MODE;
		}
	}

	// choose hours or minutes mode
	if (on_key_released(KEY_1)) {
		if (mode == CONFIGURE_HOURS_MODE) {
			mode = CONFIGURE_MINUTES_MODE;
			IOWR_8DIRECT(LEDS_BASE, 0, 0b00001111);
		} else if (mode == CONFIGURE_MINUTES_MODE) {
			mode = CONFIGURE_HOURS_MODE;
			IOWR_8DIRECT(LEDS_BASE, 0, 0b11110000);
		}
	}

	// down key
	if (on_key_released(KEY_2)) {
		if (mode == CONFIGURE_HOURS_MODE && seconds >= 3600) {
			seconds -= 3600;
		} else if (mode == CONFIGURE_MINUTES_MODE && seconds >= 60) {
			seconds -= 60;
		}
	}

	// up key
	if (on_key_released(KEY_3)) {
		if (mode == CONFIGURE_HOURS_MODE) {
			seconds += 3600;
		} else if (mode == CONFIGURE_MINUTES_MODE) {
			seconds += 60;
		}

		if (seconds >= MAX_SECONDS) seconds = 0;
	}
}

void clock_process() {
	handle_keys();

	alt_u8 hours = (seconds / 3600) % 24;
	alt_u8 minutes = (seconds / 60) % 60;

	// updating the display
	set_seg_display((hours / 10) % 10, hours % 10, (minutes / 10) % 10, minutes % 10);

	// update leds (to indicate the passing of seconds)
	if (mode == NORMAL_MODE)
		IOWR_8DIRECT(LEDS_BASE, 0, (0x1 << (seconds % 0x8)));
}


int main()
{ 
  alt_putstr("Program Started: Digital Clock!\n");

  // initializing the clock
  seconds = 0;
  mode = NORMAL_MODE;

  init_timer_interrupt();

  while (1) {
	  clock_process();
  }

  return 0;
}
