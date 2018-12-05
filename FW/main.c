#include "main.h"
#include "port_map.h"
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU 1000000UL
#define COMPA_INT 999 // every 1ms with a prescalar of 1 and clock of 1MHz for 1ms
#define LEDS 20
#define PROGRAM_TRANSITION_DELAY_MS 200

// pull in the port maps in port_map.h via the extern
extern const unsigned char led_port_dir[20] PROGMEM;
extern const unsigned char led_port_lvl[20] PROGMEM;
extern void *memcpy_P(void *, const void *, size_t);

unsigned char led_states[20] = { 0 };
unsigned char led_states_idx = 0;
unsigned char millis = PROGRAM_TRANSITION_DELAY_MS; // set to trigger on first match
unsigned char program_idx = 0;

const led_prog_record program[3] PROGMEM = {
	{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
	{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
};

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

void inline leds_off() {
	DDRA = 0;
	PORTA = 0;
	DDRB = 0;
	PORTB = 0;
}

void inline refresh_leds() {
	for(unsigned char i = led_states_idx; i < LEDS; i++) {
		if(led_states[i] == 1) {
			leds_off();
			light_led(i);
			led_states_idx = i + 1;
			break;
		}
	}
	if(led_states_idx == LEDS) {
		led_states_idx = 0;
	}
}

void inline update_led_states_from_program() {
	// do update
	led_prog_record record;
	memcpy_P(&record, &(program[program_idx]), sizeof(record));
	for(unsigned char i = 0; i < LEDS; i++) {
		led_states[i] = (record.all & (1 << i));
	}
	// bump idx
	program_idx++;
	// if we are at the number of rows, we are too far and need 
	// to reset
	if(program_idx == sizeof(program)/sizeof(program[0])) {
		program_idx = 0;
	}
}

// Handle state redraw timer interval
// This handles redrawing all the LEDs
// so often to ensure only one is lit at a time
ISR(TIM0_COMPA_vect) {
	if(millis == PROGRAM_TRANSITION_DELAY_MS) {
		millis = 0;
		update_led_states_from_program();
	}
	millis++;
	refresh_leds();
}

// Define the main function
int main(void) {
	config_power_savings();
	setup_timer0();
	sei();
	DDRA = ( 1 << DDRA0 );
	while(1) {
		set_sleep_mode(SLEEP_MODE_IDLE);
		cli();
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
}
