#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <Arduino.h>

#include "mastermind.h"
#include "lcd_wrapper.h"
#include <time.h>

//FUNCTION TO MAKE NUMBERS WHICH ARENT EQUAL TO EACH OTHER
char* not_equal_number(char* used, int size) {
    size_t i = 0;
    size_t j = 0;
    while (i < size) {
        j = i + 1;
        while (j < size) {
            if (used[j] == used[i]) {
                used[j] = (rand() % 10) + 48;
                used[i] = (rand() % 10) + 48;
                return not_equal_number(used, size);
            }
            ++j;
        }
        ++i;
    }
    return used;
}

void turn_off_leds() {
    for (int led = 13; led > 5; --led) {
        digitalWrite(led, LOW);
    }
}

void get_score(const char* secret, const char* guess, int* peg_a, int* peg_b) {
    if (strlen(secret) != strlen(guess)) {
        return;
    }

    *peg_a = 0;
    *peg_b = 0;

    int idx[4] = { 0, 0, 0, 0 };

    int pos = 0;
    while (pos < 4) {
        if (guess[pos] == secret[pos]) {
            *peg_a += 1;
            idx[pos] = 1;
        }
        ++pos;
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (guess[j] == secret[i] && idx[i] != 1 && idx[j] != 1 && i != j) {
                *peg_b += 1;
                break;
            }
        }
    }
}

char* generate_code(bool repeat, int length) {
    if (length < 1) {
        return NULL;
    }

    if (repeat == false && length > 10) {
        return NULL;
    }

    char* new_code = (char*)malloc(sizeof(char) * length + 1);

    int min = 1;
    int max = 10;

    int i = 1;
    while (i < length) {
        min = min * 10;
        max = max * 10;
        ++i;
    }

    //FOR MUCH MORE RANDOM VALUES
    randomSeed(analogRead(A3));

    int num = random(min, max);

    i = length - 1;
    while (i >= 0) {
        new_code[i] = (num % 10) + 48;
        num /= 10;
        --i;
    }

    new_code[length] = '\0';

    if (repeat == false) {
        return not_equal_number(new_code, length);
    }
    else {
        return new_code;
    }
}

void render_leds(const int peg_a, const int peg_b)
{
    turn_off_leds();
    int led = 7;

    for (int j = 0; j < peg_a; ++j, led += 2)
        digitalWrite(led, HIGH);

    led--;
    for (int j = 0; j < peg_b; ++j, led += 2)
        digitalWrite(led, HIGH);
}

void render_history(char* secret, char** history, const int entry_nr)
{
    //CLEARS THE TOP
    lcd_print_at(0, 0, "                ");

    //TWO PEGS
    int a;
    int b;

    //ATTEMPS
    char attempts[3] = "00";
    attempts[1] = entry_nr + '0';
    attempts[2] = '\0';
    lcd_print_at(0, 0, attempts);

    //SCORE COUNT
    char count[5] = "0A0B";
    get_score(secret, history[entry_nr], &a, &b);
    count[0] = a + '0';
    count[2] = b + '0';
    count[4] = '\0';
    lcd_print_at(0, 2, ":");
    lcd_print_at(0, 3, history[entry_nr]);
    lcd_print_at(0, 9, count);

    //RENDERING LEDS TO HISTORY SCORE
    render_leds(a, b);
}

void play_game(char* secret)
{
    // CHEATS
    lcd_print("Secret is:");
    lcd_print(" ");
    lcd_print(secret);
    delay(3000);
    lcd_clear();

    //INTRO
    turn_off_leds();
    lcd_print_at(0, 3, "Mastermind");
    delay(1000);
    lcd_print_at(1, 0, "It`s");
    delay(50);
    lcd_print_at(1, 6, "in");
    delay(30);
    lcd_print_at(1, 9, "a");
    delay(40);
    lcd_print_at(1, 12, "game");
    delay(3000);
    lcd_clear();

    //MAKE MEMORY FOR HISTORY
    char** history = (char**)malloc(10 * sizeof(char*));
    for (int prev_step = 0; prev_step < 10; ++prev_step) {
        history[prev_step] = (char*)malloc(5 * sizeof(char));
    }

    //PRINTING MAIN SCREEN
    lcd_print_at(0, 0, "Find  my  secret");
    for (int count = 0; count < 10; ++count)
    {
        int num_1 = 0, num_2 = 0, num_3 = 0, num_4 = 0;
        int a = 0;
        int b = 0;
        int hist_idx = count;

        char secret_string[5] = "0000";
        lcd_print_at(1, 0, "Your guess: ");
        lcd_print_at(1, 12, secret_string);

        while (digitalRead(BTN_ENTER_PIN) == LOW)
        {
            //FIRST NUMBER & TURNS ON HISTORY MOD
            if (digitalRead(BTN_1_PIN) == HIGH)
            {
                delay(200);

                //GOES TO PREV GUESS
                if (digitalRead(BTN_2_PIN) == HIGH)
                {
                    if (hist_idx >= 1) {
                        --hist_idx;
                    }
                    else {
                        continue;
                    }

                    render_history(secret, history, hist_idx);
                    continue;
                }
                //GOES TO NEXT GUESS
                if (digitalRead(BTN_3_PIN) == HIGH)
                {
                    if (hist_idx < 10 && hist_idx < count - 1) {
                        ++hist_idx;
                    }
                    else {
                        continue;
                    }

                    render_history(secret, history, hist_idx);
                    continue;
                }

                //FIRST NUMBER
                ++num_1;
                if (num_1 > 9) {
                    num_1 = 0;
                }

                secret_string[0] = num_1 + '0';
                lcd_print_at(1, 12, secret_string);
            }

            //SECOND NUMBER
            if (digitalRead(BTN_2_PIN) == HIGH)
            {
                ++num_2;
                if (num_2 > 9) {
                    num_2 = 0;
                }

                secret_string[1] = num_2 + '0';
                lcd_print_at(1, 12, secret_string);
                delay(200);
            }

            //THIRD NUMBER
            if (digitalRead(BTN_3_PIN) == HIGH)
            {
                ++num_3;
                if (num_3 > 9) {
                    num_3 = 0;
                }

                secret_string[2] = num_3 + '0';
                lcd_print_at(1, 12, secret_string);
                delay(200);
            }

            //FOURTH NUMBER
            if (digitalRead(BTN_4_PIN) == HIGH)
            {
                ++num_4;
                if (num_4 > 9) {
                    num_4 = 0;
                }

                secret_string[3] = num_4 + '0';
                lcd_print_at(1, 12, secret_string);
                delay(200);
            }
        }

        secret_string[4] = '\0';
        lcd_print_at(1, 12, secret_string);
        strcpy(history[count], secret_string);
        get_score(secret, secret_string, &a, &b);
        render_history(secret, history, count);

        //GAME COMPLETED
        if (a == 4)
        {
            lcd_clear();
            lcd_print_at(0, 0, "Oh.. My secret..");
            delay(1000);
            lcd_print_at(1, 2, "You found it..");
            delay(3000);
            lcd_clear();
            lcd_print_at(0, 0, "To continue");
            lcd_print_at(1, 5, "press Enter");
            turn_off_leds();
            while (digitalRead(BTN_ENTER_PIN) == LOW)
            {
                //Press Enter
            }
            lcd_clear();
            break;
        }

        //GAME FAILED
        else if (a != 4 && count == 9)
        {
            lcd_clear();
            lcd_print_at(0, 1, "Ha-ha, you out");
            delay(1000);
            lcd_print_at(1, 0, "Secret was");
            lcd_print_at(1, 12, secret);
            delay(2000);
            lcd_clear();
            lcd_print_at(0, 0, "To continue");
            lcd_print_at(1, 5, "press Enter");
            turn_off_leds();
            while (digitalRead(BTN_ENTER_PIN) == LOW)
            {
                //Press Enter
            }
            lcd_clear();
            break;
        }
        delay(200);
    }

    for (int prev_step = 0; prev_step < 10; ++prev_step) {
        free(history[prev_step]);
    }
    free(history);
}
