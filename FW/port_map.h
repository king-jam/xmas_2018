#ifndef PORT_MAP_H_
#define PORT_MAP_H_

#include <avr/pgmspace.h>

// lower
#define D1_D8 2
#define D1 0
#define D2 1
#define D3 2
#define D4 3
#define D5 4
#define D6 5
#define D7 6
#define D8 7
// mid
#define D9_D16 1
#define D9 0
#define D10 1
#define D11 2
#define D12 3
#define D13 4
#define D14 5
#define D15 6
#define D16 7
// upper
#define D17_D20 0
#define D17 0
#define D18 1
#define D19 2
#define D20 3

#define PA0_OFFSET 0 // Pin 2 (PA0) on ATtiny102
#define PA1_OFFSET 1 // Pin 3 (PA1) on ATtiny102
#define PB1_OFFSET 2 // Pin 5 (PB1) on ATtiny102
#define PB2_OFFSET 3 // Pin 6 (PB2) on ATtiny102
#define PB3_OFFSET 4 // Pin 7 (PB3) on ATtiny102

#define PA_MASK 0x03	// mask to just get the PA components
#define PA_RS 0			// right shift value to align with PORTA/DDRA positions
#define PB_MASK 0x1C	// mask to just get the PB components
#define PB_RS 1			// right shift value to align with PORTB/DDRB positions

//direction config for each LED (1 = output)
const unsigned char led_port_dir[20] PROGMEM = {
	( 1 << PA0_OFFSET | 1 << PA1_OFFSET ), // D1
	( 1 << PA0_OFFSET | 1 << PA1_OFFSET ), // D2
	( 1 << PA1_OFFSET | 1 << PB1_OFFSET ), // D3
	( 1 << PA1_OFFSET | 1 << PB1_OFFSET ), // D4
	( 1 << PB1_OFFSET | 1 << PB2_OFFSET ), // D5

	( 1 << PB1_OFFSET | 1 << PB2_OFFSET ), // D6
	( 1 << PB2_OFFSET | 1 << PB3_OFFSET ), // D7
	( 1 << PB2_OFFSET | 1 << PB3_OFFSET ), // D8
	( 1 << PA0_OFFSET | 1 << PB1_OFFSET ), // D9
	( 1 << PA0_OFFSET | 1 << PB1_OFFSET ), // D10

	( 1 << PA1_OFFSET | 1 << PB2_OFFSET ), // D11
	( 1 << PA1_OFFSET | 1 << PB2_OFFSET ), // D12
	( 1 << PB1_OFFSET | 1 << PB3_OFFSET ), // D13
	( 1 << PB1_OFFSET | 1 << PB3_OFFSET ), // D14
	( 1 << PA0_OFFSET | 1 << PB2_OFFSET ), // D15

	( 1 << PA0_OFFSET | 1 << PB2_OFFSET ), // D16
	( 1 << PA1_OFFSET | 1 << PB3_OFFSET ), // D17
	( 1 << PA1_OFFSET | 1 << PB3_OFFSET ), // D18
	( 1 << PA0_OFFSET | 1 << PB3_OFFSET ), // D19
	( 1 << PA0_OFFSET | 1 << PB3_OFFSET ) //  D20
};

//output config for each LED (1 = High, 0 = Low)
const unsigned char led_port_lvl[20] PROGMEM = {
	( 1 << PA0_OFFSET ), // D1
	( 1 << PA1_OFFSET ), // D2
	( 1 << PA1_OFFSET ), // D3
	( 1 << PB1_OFFSET ), // D4
	( 1 << PB1_OFFSET ), // D5

	( 1 << PB2_OFFSET ), // D6
	( 1 << PB2_OFFSET ), // D7
	( 1 << PB3_OFFSET ), // D8
	( 1 << PA0_OFFSET ), // D9
	( 1 << PB1_OFFSET ), // D10

	( 1 << PA1_OFFSET ), // D11
	( 1 << PB2_OFFSET ), // D12
	( 1 << PB1_OFFSET ), // D13
	( 1 << PB3_OFFSET ), // D14
	( 1 << PA0_OFFSET ), // D15

	( 1 << PB2_OFFSET ), // D16
	( 1 << PA1_OFFSET ), // D17
	( 1 << PB3_OFFSET ), // D18
	( 1 << PA0_OFFSET ), // D19
	( 1 << PB3_OFFSET ) // D20
};

#endif /* PORT_MAP_H_ */