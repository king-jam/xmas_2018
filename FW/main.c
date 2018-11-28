#include "main.h"
#include "port_map.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/cpufunc.h>
#include <inttypes.h>

#define COMPA_INT 999 // every 1ms with a prescalar of 1 and clock of 1MHz for 1ms
#define LEDS 20
#define PROGRAM_TRANSITION_DELAY_MS 200

// pull in the port maps in port_map.h via the extern
extern const unsigned char led_port_dir[20] PROGMEM;
extern const unsigned char led_port_lvl[20] PROGMEM;

unsigned char led_states[20] = { 0 };
unsigned char led_states_idx = 0;
unsigned char millis = PROGRAM_TRANSITION_DELAY_MS; // set to trigger on first match
unsigned char program_idx = 0;

const unsigned char program[3][3] PROGMEM = {
	{0, 0, (1 << D1) },
	{0, (1 << D11), 0},
	{(1 << D18), 0, 0}
};

void inline config_power_savings() {
	// we will want to disable modules that aren't needed
	// since we are running on battery
	// Disable everything we don't need (USART0 & ADC)
	PRR |= (1 << PRUSART) | (1 << PRADC);
}

void inline setup_timer0() {
	// Timer Compare Setups (Timer0)
	// Setup timer to prescalar FCpu/1 @ 1 mhz (1000 tickets per 1ms)
	TCCR0B |= (1 << CS00);

	// Set timer to CTC mode
	TCCR0A |= (1 << WGM01) ;
	
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
			light_led(i);
			led_states_idx = i + 1;
			break;
		}
	}
}

void inline update_led_states_from_program() {
	// do update
	led_states[0] = (program[program_idx][D1_D8] & (1 << D1)) && (1 << D1);
	led_states[1] = (program[program_idx][D1_D8] & (1 << D2)) && (1 << D2);
	led_states[2] = (program[program_idx][D1_D8] & (1 << D3)) && (1 << D3);
	led_states[3] = (program[program_idx][D1_D8] & (1 << D4)) && (1 << D4);
	led_states[4] = (program[program_idx][D1_D8] & (1 << D5)) && (1 << D5);
	led_states[5] = (program[program_idx][D1_D8] & (1 << D6)) && (1 << D6);
	led_states[6] = (program[program_idx][D1_D8] & (1 << D7)) && (1 << D7);
	led_states[7] = (program[program_idx][D1_D8] & (1 << D8)) && (1 << D8);
	led_states[8] = (program[program_idx][D9_D16] & (1 << D9)) && (1 << D9);
	led_states[9] = (program[program_idx][D9_D16] & (1 << D10)) && (1 << D10);
	led_states[10] = (program[program_idx][D9_D16] & (1 << D11)) && (1 << D11);
	led_states[11] = (program[program_idx][D9_D16] & (1 << D12)) && (1 << D12);
	led_states[12] = (program[program_idx][D9_D16] & (1 << D13)) && (1 << D13);
	led_states[13] = (program[program_idx][D9_D16] & (1 << D14)) && (1 << D14);
	led_states[14] = (program[program_idx][D9_D16] & (1 << D15)) && (1 << D15);
	led_states[15] = (program[program_idx][D9_D16] & (1 << D16)) && (1 << D16);
	led_states[16] = (program[program_idx][D17_D20] & (1 << D17)) && (1 << D17);
	led_states[17] = (program[program_idx][D17_D20] & (1 << D18)) && (1 << D18);
	led_states[18] = (program[program_idx][D17_D20] & (1 << D19)) && (1 << D19);
	led_states[19] = (program[program_idx][D17_D20] & (1 << D20)) && (1 << D20);
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
	leds_off();
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
