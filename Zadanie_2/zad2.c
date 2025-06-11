/*
 * File:   zad2.c
 * Author: kacpe
 *
 * Created on 16 kwietnia 2025, 21:25
 */

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)


#include <xc.h>
#include "libpic30.h"
#include "adc.h"
#include "buttons.h"
#include "lcd.h"
#include "string.h"

void init(void) {
    LCD_Initialize();
    BUTTON_Enable(BUTTON_S3);
    BUTTON_Enable(BUTTON_S4);
    BUTTON_Enable(BUTTON_S5);
    BUTTON_Enable(BUTTON_S6);
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA = 0x0000;
    TRISD |= (1 << 6);
    TRISD |= (1 << 13);
}

void display_start_screen(void){
    LCD_ClearScreen();
}

unsigned long getDelayLevel(void) {
    unsigned long adc = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
    if (adc == 0xFFFF) return 0;

    unsigned char poziom = adc / 205;
    switch (poziom) {
        case 0: return 320000;
        case 1: return 640000;
        case 2: return 960000;
        case 3: return 1280000;
        case 4:
        default: return 1600000;
    }
}

void binaryCounter(unsigned char *val) {
    unsigned int delay = getDelayLevel();
    LATA = (*val)++;
    __delay32(delay);
}

void snake(unsigned char *kierunek, unsigned char *pozycja) {
    unsigned int delay = getDelayLevel();
    unsigned char width = 3;
    unsigned char leds = ((1 << width) - 1) << *pozycja;
    LATA = leds;

    if (*kierunek == 0) {
        if (*pozycja < 8 - width) {
            (*pozycja)++;
        } else {
            *kierunek = 1;
            (*pozycja)--;
        }
    } else {
        if (*pozycja > 0) {
            (*pozycja)--;
        } else {
            *kierunek = 0;
            (*pozycja)++;
        }
    }
    __delay32(delay);
}

int main(void) {
    init();

    unsigned char program = 0;
    unsigned char counter = 0;
    unsigned char pos = 0;
    unsigned char dir = 0;

    unsigned char prev_next = 1;
    unsigned char prev_prev = 1;

    while (1) {
        unsigned char next = PORTDbits.RD6;
        unsigned char prev = PORTDbits.RD13;

        if (prev_next == 1 && next == 0) {
            program = (program + 1) % 2;
            __delay32(40000);
        }
        prev_next = next;

        if (prev_prev == 1 && prev == 0) {
            program = (program == 0) ? 1 : 0;
            __delay32(40000);
        }
        prev_prev = prev;

        unsigned int delay = getDelayLevel();
        
        if (program == 0) {
            binaryCounter(&counter);
        } else {
            snake(&pos, &dir);
        }

        __delay32(delay);
    }

    return 0;
}
