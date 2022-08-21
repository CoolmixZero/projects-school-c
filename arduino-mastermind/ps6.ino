#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "lcd_wrapper.h"
#include "mastermind.h"


void setup() {
    lcd_init();
    pinMode(LED_BLUE_1, OUTPUT);
    pinMode(LED_RED_1, OUTPUT);
    pinMode(LED_BLUE_2, OUTPUT);
    pinMode(LED_RED_2, OUTPUT);
    pinMode(LED_BLUE_3, OUTPUT);
    pinMode(LED_RED_3, OUTPUT);
    pinMode(LED_BLUE_4, OUTPUT);
    pinMode(LED_RED_4, OUTPUT);
}

void loop() {
    char* game = generate_code(false, 4);
    play_game(game);
    free(game);
}
