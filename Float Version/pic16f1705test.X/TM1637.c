/* 
 * File:   TM1637.h
 * Author: Stefan
 *
 * Contains the implementation for the TM1637 display driver header file
 * reduced to the bare essentials in order to fit on a tiny microcontroller
 * with as low overhead as possible
 */


#include <xc.h>
#include "TM1637.h"


#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

const uint8_t digitToSegment[] = {
    // XGFEDCBA
    0b00111111,    // 0
    0b00000110,    // 1
    0b01011011,    // 2
    0b01001111,    // 3
    0b01100110,    // 4
    0b01101101,    // 5
    0b01111101,    // 6
    0b00000111,    // 7
    0b01111111,    // 8
    0b01101111,    // 9
    0b01110111,    // A
    0b01111100,    // b
    0b00111001,    // C
    0b01011110,    // d
    0b01111001,    // E
    0b01110001     // F
};

void initDisplay()
{
	// Set the pin direction and default value.
	// Both pins are set as inputs, allowing the pull-up resistors to pull them up
    /*
    pinMode(m_pinClk, INPUT);
    pinMode(m_pinDIO,INPUT);
	digitalWrite(m_pinClk, LOW);
	digitalWrite(m_pinDIO, LOW);
     */
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 1; 
    PORTAbits.RA5 = 0;
    PORTAbits.RA4 = 0;
}

void bitDelay()
{
	__delay_us(DEFAULT_BIT_DELAY);
}

void start(){
    // pinMode(m_pinDIO, OUTPUT);
    TRISAbits.TRISA5 = 0;
    bitDelay();
}

void stop(){
    // pinMode(m_pinDIO, OUTPUT);
    TRISAbits.TRISA5 = 0;
    bitDelay();
	// pinMode(m_pinClk, INPUT);
    TRISAbits.TRISA4 = 1;
	bitDelay();
	// pinMode(m_pinDIO, INPUT);
    TRISAbits.TRISA5 = 1;
	bitDelay();
}

bool writeByte(uint8_t b)
{
    uint8_t data = b;

    // 8 Data Bits
    for(uint8_t i = 0; i < 8; i++) {
        // CLK low
        // pinMode(m_pinClk, OUTPUT);
        TRISAbits.TRISA4 = 0;
        bitDelay();

        // Set data bit
        if (data & 0x01){
            // pinMode(m_pinDIO, INPUT);
            TRISAbits.TRISA5 = 1;
        }
        else{
            // pinMode(m_pinDIO, OUTPUT);
            TRISAbits.TRISA5 = 0;
        }

        bitDelay();

        // CLK high
        // pinMode(m_pinClk, INPUT);
        TRISAbits.TRISA4 = 1;
        bitDelay();
        data = data >> 1;
    }

    // Wait for acknowledge
    // CLK to zero
    // pinMode(m_pinClk, OUTPUT);
    TRISAbits.TRISA4 = 0;
    // pinMode(m_pinDIO, INPUT);
    TRISAbits.TRISA5 = 1;
    bitDelay();

    // CLK to high
    // pinMode(m_pinClk, INPUT);
    TRISAbits.TRISA4 = 1;
    bitDelay();
    // uint8_t ack = digitalRead(m_pinDIO);
    uint8_t ack = PORTAbits.RA5;
    if (ack == 0){
        // pinMode(m_pinDIO, OUTPUT);
        TRISAbits.TRISA5 = 0;
    }

    bitDelay();
    // pinMode(m_pinClk, OUTPUT);
    TRISAbits.TRISA4 = 0;
    bitDelay();

    return ack;
}


void setSegments(const char segments[])
{
    // Write COMM1
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	// Write COMM2 + first digit address
	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

	// Write the data bytes
	for (uint8_t k = 0; k < 4; k++){
        writeByte(digitToSegment[segments[k] & 0x0F] | ((k == 1) ? 0b10000000 : 0b00000000));
    }
    
	stop();

	// Write COMM3 + brightness
	start();
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}

