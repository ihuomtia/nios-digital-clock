/*
 * seven_segment_display.h
 *
 *  Created on: Oct 25, 2024
 *      Author: ihuomtia
 */

#ifndef SEVEN_SEGMENT_DISPLAY_H_
#define SEVEN_SEGMENT_DISPLAY_H_

#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "system.h"
#include "io.h"

#define SEV_SEG_BASE SEGMENT_DISPLAY_BASE

#define DIGIT_0 0b1000000
#define DIGIT_1 0b1111001
#define DIGIT_2 0b0100100
#define DIGIT_3 0b0110000
#define DIGIT_4 0b0011001
#define DIGIT_5 0b0010010
#define DIGIT_6 0b0000010
#define DIGIT_7 0b1111000
#define DIGIT_8 0b0000000
#define DIGIT_9 0b0010000

alt_u8 translate_digit_seven_seg(alt_u8 digit) {
	switch(digit) {

		case 1:
			return DIGIT_1;
		case 2:
			return DIGIT_2;
		case 3:
			return DIGIT_3;
		case 4:
			return DIGIT_4;
		case 5:
			return DIGIT_5;
		case 6:
			return DIGIT_6;
		case 7:
			return DIGIT_7;
		case 8:
			return DIGIT_8;
		case 9:
			return DIGIT_9;
		default:
			return DIGIT_0;

	}
}

void set_seg_display(alt_u8 d3, alt_u8 d2, alt_u8 d1, alt_u8 d0) {
	IOWR_32DIRECT(SEV_SEG_BASE, 0,
			(translate_digit_seven_seg(d3) << 21) |
			(translate_digit_seven_seg(d2) << 14) |
			(translate_digit_seven_seg(d1) << 7) |
			(translate_digit_seven_seg(d0) << 0)
	);
}

#endif /* SEVEN_SEGMENT_DISPLAY_H_ */
