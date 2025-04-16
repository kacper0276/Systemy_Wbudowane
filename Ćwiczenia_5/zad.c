/*
 * File:   zad.c
 * Author: kacpe
 *
 * Created on April 16, 2025, 10:05 AM
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

void init(void) {
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA = 0x0000; // Port A na wyjscie
    TRISD |= (1 << 6); // rd6 jako wejscie
}

int main(void) {
    init();
    
    // Zmienne
    unsigned long adc_value;
    unsigned char tryb = 0;
    unsigned char poprzedni_stan_przycisku = 1;
    unsigned long opoznienie;
    
    while(1) {
        // Odczyt wartosci z potencjometru
        adc_value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
        
        if (adc_value == 0xFFFF) continue;
        opoznienie = 40000 + (adc_value * 4000);
        
        unsigned char stan_przycisku = PORTDbits.RD6;
        
        if (poprzedni_stan_przycisku == 1 && stan_przycisku == 0) {
            tryb ^= 1;
            __delay32(40000);
        }
        poprzedni_stan_przycisku = stan_przycisku;
        
        if (tryb == 0) {
            LATA = 0x0001; // Zapalenie pierwszej diody
            __delay32(opoznienie);
            LATA = 0x0000; // Zgaszenie diod
            __delay32(opoznienie);
        } else {
            LATA = 0xFFFF; // Zapalenie wszystkich diod
            __delay32(opoznienie);
            LATA = 0x0000; // Zgaszenie diod
            __delay32(opoznienie);
        }
    }
    
    return 0;
}
