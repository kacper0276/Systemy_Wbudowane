/*
 * File:   zad1.c
 * Author: kacpe
 *
 * Created on 16 kwietnia 2025, 19:25
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
    TRISA = 0x0000;         // PORTA jako wyj?cie (LEDy)
    TRISD |= (1 << 6);      // RD6 = NEXT
    TRISD |= (1 << 13);     // RD13 = PREV
}

void display_start_screen(void){
    LCD_ClearScreen();
    LCD_PutString("DZIEN DOBRY", 11);
    LCD_PutChar('\n');
    LCD_PutString("EKRAN LCD", 9);
}

// ===== KOD GRAYA =====
unsigned char binaryToGray(unsigned char num) {
    return num ^ (num >> 1);
}

// ===== KOD BCD =====
unsigned char toBCD(unsigned char num) {
    return ((num / 10) << 4) | (num % 10);
}

// ===== PSEUDOLOSOWY GENERATOR 6-bit =====
unsigned char lfsr6(unsigned char *reg) {
    unsigned char bit = (
                          ((*reg >> 0) & 1) ^
                          ((*reg >> 1) & 1) ^
                          ((*reg >> 4) & 1) ^
                          ((*reg >> 5) & 1) ^
                          ((*reg >> 6) & 1)
                        );
    *reg = (*reg << 1) | bit;
    return *reg & 0x3F;
}
// ===== PROGRAMY =====
void program1(unsigned char *licznik) {
    LATA = (*licznik)++;
    __delay32(400000);
}

void program2(unsigned char *licznik) {
    LATA = (*licznik)--;
    __delay32(400000);
}

void program3(unsigned char *licznik) {
    LATA = binaryToGray((*licznik)++);
    __delay32(400000);
}

void program4(unsigned char *licznik) {
    LATA = binaryToGray((*licznik)--);
    __delay32(400000);
}

void program5(unsigned char *licznik) {
    if (*licznik > 99) *licznik = 0;
    LATA = toBCD((*licznik)++);
    __delay32(400000);
}

void program6(unsigned char *licznik) {
    if (*licznik == 0xFF || *licznik > 99) *licznik = 99;
    LATA = toBCD((*licznik)--);
    __delay32(400000);
}

void program7(unsigned char *kierunek, unsigned char *pozycja) {
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
    __delay32(400000);
}

void program8(unsigned char *kolejka) {
    static unsigned char leds = 0;
    unsigned char new_bit = 1;

    for (int i = 0; i < 8 - *kolejka; i++) {
        unsigned char frame = leds | new_bit;
        LATA = frame;
        __delay32(320000);
        if (i != 7 - *kolejka)
            new_bit <<= 1;
    }

    leds |= new_bit;
    (*kolejka)++;
    if (*kolejka > 7) {
        *kolejka = 0;
        leds = 0;
    }
}

void program8(unsigned char *dummy_param) {
    static unsigned char active_bit_position = 0;
    static unsigned char permanently_lit_mask = 0;

    if (active_bit_position == 7 ||
        (active_bit_position < 7 && ((permanently_lit_mask >> (active_bit_position + 1)) & 1))) {
        permanently_lit_mask |= (1 << active_bit_position);
        active_bit_position = 0;
    } else {
        active_bit_position++;
    }

    unsigned char current_active_bit = (1 << active_bit_position);
    LATA = permanently_lit_mask | current_active_bit;

    if (permanently_lit_mask == 0xFF) {
        active_bit_position = 0;
        permanently_lit_mask = 0;
        LATA = 0;
    }
}

void program9(unsigned char *seed) {
    LATA = lfsr6(seed);
}

int main(void) {
    init();
    display_start_screen();

    unsigned char program = 1;
    unsigned char licznik = 0;
    unsigned char kierunek = 0;
    unsigned char pozycja = 0;
    unsigned char kolejka = 0;
    unsigned char seed = 0b1110011;

    unsigned char prev_next = 1, prev_prev = 1;

    while(1) {
        unsigned long adc_val = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
        
        if (adc_val == 0xFFFF) continue;
        unsigned long opoznienie = 40000 + (adc_val * 4000);

        unsigned char btn_next = PORTDbits.RD6;
        unsigned char btn_prev = PORTDbits.RD13;

        if (prev_next == 1 && btn_next == 0) {
            program++;
            if (program > 9) program = 1;
            __delay32(40000);
        }
        prev_next = btn_next;

        if (prev_prev == 1 && btn_prev == 0) {
            program--;
            if (program < 1) program = 9;
            __delay32(40000);
        }
        prev_prev = btn_prev;

        switch(program) {
            case 1: program1(&licznik);LCD_PutString("P:1", 3); break;
            case 2: program2(&licznik);LCD_PutString("P:2", 3); break;
            case 3: program3(&licznik);LCD_PutString("P:3", 3); break;
            case 4: program4(&licznik);LCD_PutString("P:4", 3); break;
            case 5: program5(&licznik);LCD_PutString("P:5", 3); break;
            case 6: program6(&licznik);LCD_PutString("P:6", 3); break;
            case 7: program7(&kierunek, &pozycja);LCD_PutString("P:7", 3); break;
            case 8: program8(&kolejka);LCD_PutString("P:8", 3); break;
            case 9: program9(&seed);LCD_PutString("P:9", 3); break;
        }

        __delay32(opoznienie);
    }
    
    while(1){
        
    }

    return 0;
}
