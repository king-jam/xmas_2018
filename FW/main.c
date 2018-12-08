#include "port_map.h"
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU 1000000UL
#define COMPA_INT 99 // every 1us with a prescalar of 1 and clock of 1MHz for 1ms
#define LEDS 20
#define PROGRAM_TRANSITION_DELAY_MS 1000

// pull in the port maps in port_map.h via the extern
extern const unsigned char led_port_dir[20] PROGMEM;
extern const unsigned char led_port_lvl[20] PROGMEM;

//unsigned char active_leds[4] = {0,1,2,3};
unsigned int millis = PROGRAM_TRANSITION_DELAY_MS;
unsigned char pinState = 0;
unsigned int tickCount = 0;
unsigned int pwmValue = 0;
unsigned char pwmMax = 100;
unsigned char fadeIncrement = 1;
unsigned char up = 1;

void inline config_power_savings() {
	// we will want to disable modules that aren't needed
	// since we are running on battery
	// Disable everything we don't need (USART0 & ADC)
	PRR |= ( 1 << PRUSART ) | ( 1 << PRADC );
}

void inline setup_timer0() {
	// Timer Compare Setups (Timer0)
	// Setup timer to prescalar FCpu/1024 @ 1 mhz (1000 tickets per 1ms)
	TCCR0B |= ( 1 << CS00 );

	// Set timer to CTC mode
	TCCR0B |= ( 1 << WGM02 );
	
	// Enable CTC interrupt
	TIMSK0 |= ( 1 << OCIE0A );
	
	// set to the compare interval (1ms)
	OCR0A = COMPA_INT;
}

void leds_off() {
	DDRA = 0;
	PORTA = 0;
	DDRB = 0;
	PORTB = 0;
}

void light_led(unsigned char led) {
	unsigned char dir = 0;
	unsigned char lvl = 0;
	dir = pgm_read_byte(&(led_port_dir[led]));
	lvl = pgm_read_byte(&(led_port_lvl[led]));
	DDRA = (dir & PA_MASK) >> PA_RS;
	PORTA = (lvl & PA_MASK) >> PA_RS;
	DDRB = (dir & PB_MASK) >> PB_RS;
	PORTB = (lvl & PB_MASK) >> PB_RS;
}

// Credit to: https://www.baldengineer.com/software-pwm-with-millis.html
void handle_pwm() {
  tickCount++;
    // determine if we're counting on or off time
  if (pinState == 1) {
    // see if we hit the desired on percentage
    // not as precise as 255 or 1024, but easier to do math
    if (tickCount >= pwmValue) {
      pinState = 0;
    }
  } else {
    // if it isn't on, it is off
    if (tickCount >= pwmMax) {
      pinState = 1;
      tickCount = 0;
    }
  }
  // could probably use some bitwise optimization here, digitalWrite()
  // really slows things down after 10 pins.
  if(pinState == 1) {
  	light_led(6);
  } else {
  	leds_off();
  }
}

// Handle state redraw timer interval
// This handles redrawing all the LEDs
// so often to ensure only one is lit at a time
ISR(TIM0_COMPA_vect) {
	handle_pwm();

	if(millis == PROGRAM_TRANSITION_DELAY_MS) {
		millis = 0;
		if(up) {
			pwmValue += fadeIncrement;
			if(pwmValue > 30) {
				pwmValue = 30;
				up = 0;
			}
		} else {
			pwmValue -= fadeIncrement;
			if(pwmValue <= 0) {
				pwmValue = 0;
				up = 1;
			}
		}
	}
	millis++;
}

// Define the main function
int main(void) {
	config_power_savings();
	setup_timer0();
	sei();
	while(1) {
		set_sleep_mode(SLEEP_MODE_IDLE);
		cli();
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
}
