/* --------------------------------------------------------------------------                                 
    Template project main
    File name: template.c
    Author: Etienne Le Sueur
    Description: The template main file. It provides a simple example of using
                 some standard scandal functions such as the UART library, the
                 CAN library, timers, LEDs, GPIOs.
                 It is designed to compile and work for the 3 micros we use on
                 the car currently, the MSP430F149 and MCP2515 combination and
                 the LPC11C14 and LPC1768 which have built in CAN controllers.

                 UART_printf is provided by the Scandal stdio library and 
                 should work well on all 3 micros.

                 If you are using this as the base for a new project, you
                 should first change the name of this file to the name of
                 your project, and then in the top level makefile, you should
                 change the CHIP and MAIN_NAME variables to correspond to
                 your hardware.

                 Don't be scared of Scandal. Dig into the code and try to
                 understand what's going on. If you think of an improvement
                 to any of the functions, then go ahead and implement it.
                 However, before committing the changes to the Scandal repo,
                 you should discuss with someone else to ensure that what 
                 you've done is a good thing ;-)

                 Keep in mind that this code is live to the public on
                 Google Code. No profanity in comments please!

    Copyright (C) Etienne Le Sueur, 2011

    Created: 07/09/2011
   -------------------------------------------------------------------------- */

/* 
 * This file is part of the Sunswift Template project
 * 
 * This tempalte is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with the project.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <scandal/engine.h>
#include <scandal/message.h>
#include <scandal/leds.h>
#include <scandal/utils.h>
#include <scandal/uart.h>
#include <scandal/stdio.h>

#include <project/driver_config.h>
#include <project/target_config.h>

#include <arch/can.h>
#include <arch/uart.h>
#include <arch/timer.h>
#include <arch/gpio.h>
#include <arch/types.h>
#include <arch/adc.h>
#include <arch/i2c.h>

/* Do some general setup for clocks, LEDs and interrupts
 * and UART stuff on the MSP430 */
void setup(void) {
#if defined(lpc11c14) || defined(lpc1768)
	GPIO_Init();
	GPIO_SetDir(2,8,1); //Green LED, Out
	GPIO_SetDir(2,7,1); //Yel LED, Out
    ADC_Init(2);
    ADC_EnableChannel(0);
    ADC_Read(0);

#endif // lpc1768 || lpc11c14
} // setup

/* This is an in-channel handler. It gets called when a message comes in on the
 * registered node/channel combination as set up using the USB-CAN. */
void in_channel_0_handler(int32_t value, uint32_t src_time) {
	UART_printf("in_channel_0_handler got called with value %d time at source %u\n\r", (int)value, (unsigned int)src_time);
}

/* This is your main function! You should have an infinite loop in here that
 * does all the important stuff your node was designed for */
int main(void) {
	int i = 0;
	uint32_t value = 0xaa;

	setup();

	scandal_init();

	UART_Init(115200);

	sc_time_t one_sec_timer = sc_get_timer(); /* Initialise the timer variable */
	sc_time_t test_in_timer = sc_get_timer(); /* Initialise the timer variable */

	/* Set LEDs to known states */
	red_led(1);
	yellow_led(0);

	scandal_delay(100); /* wait for the UART clocks to settle */

	/* Display welcome header over UART */
	UART_printf("Welcome to the template project! This is coming out over UART1\n\r");
	UART_printf("The 2 debug LEDs should blink at a rate of 1HZ\n\r");
	UART_printf("If you configure the in channel 0, I should print a message upon receipt of such a channel message\n\r");

	scandal_register_in_channel_handler(0, &in_channel_0_handler);

	/* This is the main loop, go for ever! */
	while (1) {
		/* This checks whether there are pending requests from CAN, and sends a heartbeat message.
		 * The heartbeat message encodes some data in the first 4 bytes of the CAN message, such as
		 * the number of errors and the version of scandal */
		handle_scandal();

        UART_printf("BLARGH\n\r");

	}
}
