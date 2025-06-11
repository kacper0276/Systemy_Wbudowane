/*
 * File:   zad4.c
 * Author: kacpe
 *
 * Created on April 30, 2025, 11:00 AM
 */

#include "xc.h"
#include "libpic30.h"
#include "lcd.h"
#include <string.h>

#define DISPLAY_WIDTH 16
#define BUFFER_SIZE 33
#define SCROLL_DELAY 2000000

const char special_text[] = "\x7E PROMOCJA: KUP TERAZ! \x7F";

void init(void) {
    LCD_Initialize();
}

void display_scrolling_text(const char *text, int *pos, char *buffer) {
    int text_length = strlen(text);

    memset(buffer, ' ', DISPLAY_WIDTH);
    buffer[DISPLAY_WIDTH] = '\0';

    for (int i = 0; i < DISPLAY_WIDTH; i++) {
        buffer[i] = text[(*pos + i) % text_length];
    }

    LCD_ClearScreen();
    LCD_PutString(buffer, DISPLAY_WIDTH);

    *pos = (*pos + 1) % text_length;

    __delay32(SCROLL_DELAY);
}

int main(void) {
    init();

    int scroll_pos = 0;
    char scroll_buffer[BUFFER_SIZE];

    while (1) {
        display_scrolling_text(special_text, &scroll_pos, scroll_buffer);
    }

    return 0;
}
