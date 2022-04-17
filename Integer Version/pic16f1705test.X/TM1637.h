/* 
 * File:   TM1637.h
 * Author: Stefan
 *
 * Created on April 17, 2022, 1:47 PM
 */

#ifndef TM1637_H
#define	TM1637_H

// inspired by https://github.com/avishorp/TM1637/blob/master/TM1637Display.h

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

#define DEFAULT_BIT_DELAY  100
#define _XTAL_FREQ 32000000

// max brightness, 0b0111 for brightness + 0b1000 for display on
#define m_brightness 0x0f
// always write the whole display
#define pos 0


#ifdef	__cplusplus
extern "C" {
#endif

void initDisplay(void);
void setSegments(const char segments[]);
void bitDelay(void);
void start(void);
void stop(void);
bool writeByte(uint8_t b);
void showDots(uint8_t dots, uint8_t* digits);



#ifdef	__cplusplus
}
#endif

#endif	/* TM1637_H */

