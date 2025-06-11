/*
 * File:   zad3.c
 * Author: kacpe
 *
 * Created on April 30, 2025, 11:00 AM
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

#include "xc.h"
#include "libpic30.h"
#include "adc.h"
#include "buttons.h"
#include "lcd.h"
#include "string.h"

#define ALARM_THRESHOLD 512
#define LED_PORT LATA
#define SINGLE_LED 0x01
#define ALL_LEDS 0xFFFF
#define BLINK_DELAY 1000000

void display_start_screen(void){
    LCD_ClearScreen();
    LCD_PutString("ALARM SYSTEM", 12);
}

void init(void) {
    BUTTON_Enable(BUTTON_S3);
    BUTTON_Enable(BUTTON_S4);
    BUTTON_Enable(BUTTON_S5);
    BUTTON_Enable(BUTTON_S6);
    LCD_Initialize();
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA = 0x0000;
    TRISD |= (1 << 6);
}

unsigned int read_potentiometer(void) {
    unsigned int val = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
    return (val == 0xFFFF) ? 0 : val;
}

void blink_single_led_5s(void) {
    for (int i = 0; i < 5; i++) {
        LED_PORT = SINGLE_LED;
        __delay32(BLINK_DELAY);
        LED_PORT = 0x00;
        __delay32(BLINK_DELAY);
    }
}

void blink_all_leds_once(void) {
    LED_PORT = ALL_LEDS;
    __delay32(BLINK_DELAY);
    LED_PORT = 0x00;
    __delay32(BLINK_DELAY);
}

int main(void) {
    init();
    display_start_screen();

    unsigned char alarm_active = 0;
    unsigned char full_alarm_mode = 0;
    unsigned char prev = 1;
    unsigned char alarm_blocked = 0;

    while (1) {
        unsigned int pot_value = read_potentiometer();
        unsigned char btn = PORTDbits.RD6;
        
        if (!alarm_active && !alarm_blocked && pot_value > ALARM_THRESHOLD) {
            alarm_active = 1;
            blink_single_led_5s();
            full_alarm_mode = 1;
        }

        if (full_alarm_mode) {
            blink_all_leds_once();
        }

        if (alarm_active && ((btn == 0 && prev == 1) || pot_value < ALARM_THRESHOLD)) {
            LED_PORT = 0x00;
            alarm_active = 0;
            full_alarm_mode = 0;
            alarm_blocked = 1;
            __delay32(40000);
        }
        
        prev = btn;
        
        if (alarm_blocked && pot_value < ALARM_THRESHOLD) {
            alarm_blocked = 0;
        }
    }

    return 0;
}
