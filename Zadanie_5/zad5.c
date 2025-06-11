/*
 * File:   zad5.c
 * Author: kacpe
 *
 * Created on April 30, 2025, 11:01 AM
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
#pragma config JTAGEN = OFF 

#include <xc.h>
#include "libpic30.h"
#include "adc.h"
#include "buttons.h"
#include "lcd.h"
#include <stdio.h>
#include "string.h"

#define GRACZ1_BUTTON PORTBbits.RB3
#define GRACZ2_BUTTON PORTBbits.RB5

unsigned long getGameTimeMs(void) {
    unsigned long adc = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
    if (adc == 0xFFFF) return 0;

    unsigned char poziom = adc / 341;
    switch (poziom) {
        case 0: return 60000;
        case 1: return 180000;
        default: return 300000;
    }
}

void showTime(unsigned long t1, unsigned long t2) {
    char buf[32];
    LCD_ClearScreen();
    sprintf(buf, "P1: %02lu:%02lu", t1 / 60000, (t1 / 1000) % 60);
    LCD_PutString(buf, strlen(buf));
    LCD_PutChar('\n');
    sprintf(buf, "P2: %02lu:%02lu", t2 / 60000, (t2 / 1000) % 60);
    LCD_PutString(buf, strlen(buf));
}

void showEndScreen(const char *loser) {
    LCD_ClearScreen();
    LCD_PutString("KONIEC GRY", 11);
    LCD_PutChar('\n');
    LCD_PutString(loser, strlen(loser));
}

void init() {
    LCD_Initialize();
    BUTTON_Enable(BUTTON_S3);
    BUTTON_Enable(BUTTON_S4);
    BUTTON_Enable(BUTTON_S5);
    BUTTON_Enable(BUTTON_S6);
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);

//    TRISB |= (1 << 3); // RB3 input
//    TRISB |= (1 << 5); // RB5 input
    TRISD |= (1 << 6);      // RD6 = NEXT
    TRISD |= (1 << 13);     // RD13 = PREV
}

int main() {
    init();

    unsigned long startTime = getGameTimeMs();
    unsigned long lastShownTime = 0xFFFFFFFF;
    
    while (PORTDbits.RD6 == 1) {
        startTime = getGameTimeMs();

        if (startTime != lastShownTime) {
            showTime(startTime, startTime);
            lastShownTime = startTime;
        }

        __delay32(40000);
    }
    
    unsigned long t1 = startTime, t2 = startTime;

    int activePlayer = 1;
    unsigned char prevRB3 = 1, prevRB5 = 1;

    showTime(t1, t2);

    while (1) {
        if (activePlayer == 1 && t1 > 0) {
            __delay32(160000);
            t1 -= 100;
        } else if (activePlayer == 2 && t2 > 0) {
            __delay32(160000);
            t2 -= 100;
        }

        showTime(t1, t2);

        if (t1 == 0) {
            showEndScreen("Gracz 1 przegral");
            break;
        } else if (t2 == 0) {
            showEndScreen("Gracz 2 przegral");
            break;
        }

//        unsigned char currRB3 = PORTBbits.RB3;
//        unsigned char currRB5 = PORTBbits.RB5;
        
        unsigned char currRB3 = PORTDbits.RD6;
        unsigned char currRB5 = PORTDbits.RD13;

        if (prevRB3 == 1 && currRB3 == 0) {
            activePlayer = 1;
            __delay32(40000);
        }
        
        if (prevRB5 == 1 && currRB5 == 0) {
            activePlayer = 2;
            __delay32(40000);
        }

        prevRB3 = currRB3;
        prevRB5 = currRB5;
        
        __delay32(40000);
    }
    
    while(1){
        
    }

    return 0;
}
