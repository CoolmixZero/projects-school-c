#ifndef _LCD_H
#define _LCD_H

/**
 * Initialize LCD
 * 
 * Everything you need to initialize your LCD display, place here.
 * This function should be called in function setup().
 */
void lcd_init();


/**
 * Clears the LCD display
 * 
 * Clears content of the LCD display. After the call is display empty.
 */
void lcd_clear();


/**
 * Set cursor to given posititon
 * 
 * Sets cursor to given position based on y and x coordinates.
 * @param y the row
 * @param x the column
 */
void lcd_set_cursor(int y, int x);


/**
 * Print text on LCD display
 * 
 * Prints the givent text on the display on the current
 * cursor position.
 * @param text the text to print
 */
void lcd_print(char* text);


/**
 * Set cursor to given positition and print text
 * 
 * Sets the cursor to given position based on y and x coorindates
 * and then prints the given text.
 * @param y the row
 * @param x the column
 * @param text the text to print
 */
void lcd_print_at(int y, int x, char* text);

#endif
