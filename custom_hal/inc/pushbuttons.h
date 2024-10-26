/*
 * pushbuttons.h
 *
 *  Created on: Oct 25, 2024
 *      Author: ihuomtia
 */

#ifndef PUSHBUTTONS_H_
#define PUSHBUTTONS_H_

#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "system.h"
#include "io.h"

#define PUSHBUTTONS_BASE KEYS_BASE
#define KEY_0 0
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3

alt_u8 on_key_released(alt_u8 key) {
	if ((IORD_8DIRECT(PUSHBUTTONS_BASE, 0) & (1 << key)) == 0x0) {

		while ((IORD_8DIRECT(PUSHBUTTONS_BASE, 0) & (1 << key)) == 0x0) {};
		return 0x1;
	}

	return 0x0;
}

#endif /* PUSHBUTTONS_H_ */
