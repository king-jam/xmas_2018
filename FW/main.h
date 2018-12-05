#ifndef MAIN_H_
#define MAIN_H_

typedef union {
	struct {
		unsigned char D2: 1;
		unsigned char D3: 1;
		unsigned char D1: 1;
		unsigned char D4: 1;
		unsigned char D5: 1;
		unsigned char D6: 1;
		unsigned char D7: 1;
		unsigned char D8: 1;
		unsigned char D9: 1;
		unsigned char D10: 1;
		unsigned char D11: 1;
		unsigned char D12: 1;
		unsigned char D13: 1;
		unsigned char D14: 1;
		unsigned char D15: 1;
		unsigned char D16: 1;
		unsigned char D17: 1;
		unsigned char D18: 1;
		unsigned char D19: 1;
		unsigned char D20: 1;
	} leds;
	unsigned long int all;
} led_prog_record;

#define D1 0
#define D2 1
#define D3 2
#define D4 3
#define D5 4
#define D6 5
#define D7 6
#define D8 7
#define D9 8
#define D10 9
#define D11 10
#define D12 11
#define D13 12
#define D14 13
#define D15 14
#define D16 15
#define D17 16
#define D18 17
#define D19 18
#define D20 19

#endif /* MAIN_H_ */