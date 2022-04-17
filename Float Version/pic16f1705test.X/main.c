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
float avgtemp = 0.0f;

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
    initDisplay();
    char segs[4] = {1, 2, '3', '4'};
    while (1)
    {
        for(uint8_t i = 0; i < AVG_SAMPLES; i++){
            uint16_t number = ADC_GetConversion(channel_AN0); // get raw ADC reading from MCP9701
            uint16_t temperature_whole = (float)((number << 2) - TEMP_CONST) / (float)SLOPE; // convert reading to celsius
           
            avgtemp += temperature_whole;
            printf("Temperature: %fC", temperature_whole); // just this printf takes up 3431 bytes haha
            printf("\n");


            __delay_ms(1000);
        }
        
        avgtemp /= (float)AVG_SAMPLES;
        
        segs[0] = (uint8_t)(avgtemp / 10.0f); // first two digits
        segs[1] = (uint8_t)avgtemp % 10;
        segs[2] = (uint8_t)((avgtemp - (float)((uint8_t)avgtemp)) * 10.0f);
        segs[3] = 0xC; // C for celsius
        
        setSegments(segs);
        
        avgtemp = 0.0f;
    }
}
/**
 End of File
*/