/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F1705
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "TM1637.h"

/*
                         Main application
 */

#define _XTAL_FREQ 32000000

#define TEMP_CONST 800
#define SLOPE 39

#define AVG_SAMPLES 10
uint16_t avgtemp = 0;
uint16_t avgremainder = 0;

void uint16_to_chararr(unsigned char* sptr, uint16_t Arg)
{
    *sptr = 0;
    while (Arg >= 10000)
    {    Arg -= 10000;
       *sptr += 1;
    }
    sptr++;
    *sptr = 0;
    while (Arg >= 1000)
    {    Arg -= 1000;
       *sptr += 1;
    }
    sptr++;
    *sptr = 0;
    while (Arg >= 100)
    {    Arg -= 100;
       *sptr += 1;
    }
    sptr++;
    *sptr = 0;
    while (Arg >= 10)
    {    Arg -= 10;
       *sptr += 1;
    }
    sptr++;
    *sptr = (unsigned char)Arg;
    // https://www.microchip.com/forums/m853241.aspx#853972
}  



void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    char uintbuf[5] = "00000"; // temporary buffer to store int to BCD results
    char printbuf[6] = "00.0C\0"; // basically a null terminated string for temperature
    initDisplay();
    char segs[4] = {1, 2, '3', '4'};
    while (1)
    {
        for(uint8_t i = 0; i < AVG_SAMPLES; i++){
            uint16_t number = ADC_GetConversion(channel_AN0); // get raw ADC reading from MCP9701
            uint16_t temperature_whole = ((number << 2) - TEMP_CONST) / SLOPE; // convert reading to celsius (whole part)
            uint16_to_chararr(uintbuf, temperature_whole);
            avgtemp += temperature_whole;
            
            
            printbuf[0] = uintbuf[3] + 0x30; // only copy first two numbers, result is guaranteed to be two digit anyway
            printbuf[1] = uintbuf[4] + 0x30;

            uint16_t remainder = ((10 * (((number << 2) - TEMP_CONST))) - (temperature_whole * (SLOPE * 10))) / 39; // one decimal digit precision
            avgremainder += remainder;
            uint16_to_chararr(uintbuf, remainder);
            printbuf[3] = uintbuf[4] + 0x30; // only care about one digit, result is guaranteed to be one digit anyway

            puts(printbuf); // print the temperature to STDIO (UART)
            printf("\n");


            __delay_ms(1000);
        }
        
        uint16_to_chararr(uintbuf, avgtemp / AVG_SAMPLES); // turn uint16 into BCD
        avgtemp = 0;
        printbuf[0] = uintbuf[3] + 0x30;
        printbuf[1] = uintbuf[4] + 0x30;
        
        uint16_to_chararr(uintbuf, avgremainder / AVG_SAMPLES);
        avgremainder = 0;
        printbuf[3] = uintbuf[4] + 0x30; // only care about one digit, result is guaranteed to be one digit anyway
        puts(printbuf);
        
        segs[0] = printbuf[0]; // first two digits
        segs[1] = printbuf[1];
        segs[2] = printbuf[3]; // decimal digit
        segs[3] = 0xC; // C for celsius
        
        setSegments(segs);
    }
}
/**
 End of File
*/