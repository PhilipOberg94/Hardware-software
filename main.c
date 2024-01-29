/*
* 4Pincommunication(2).c
*
* Created: 2024-01-29 00:14:47
* Author : Öberg
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>

#define LCD_DPRT PORTD
#define LCD_DDRD DDRD
#define LCD_DPIN PIND
#define LCD_CPRT PORTB
#define LCD_CDDR DDRB
#define LCD_CPIN PINB
#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2
#define BUTTON1 3
#define BUTTON2 4
#define BUTTON3 5

void lcdCOmmand(unsigned char cmnd) {
    LCD_DPRT = cmnd & 0xF0;
    LCD_CPRT &= ~ (1<<LCD_RS);
    LCD_CPRT &= ~ (1<<LCD_RW);
    LCD_CPRT |= (1<<LCD_EN);
    _delay_us(10);
    LCD_CPRT &= ~(1<<LCD_EN);
    _delay_us(3000);
    
    LCD_DPRT = cmnd<<4;
    LCD_CPRT |= (1<<LCD_EN);
    _delay_us(10);
    LCD_CPRT &= ~ (1<<LCD_EN);
    _delay_us(3000);
}

void lcdData(unsigned char data) {
    LCD_DPRT = data & 0xF0;
    LCD_CPRT |= (1<<LCD_RS);
    LCD_CPRT &= ~ (1<<LCD_RW);
    LCD_CPRT |= (1<<LCD_EN);
    _delay_us(10);
    LCD_CPRT &= ~ (1<<LCD_EN);
    LCD_DPRT = data<<4;
    LCD_CPRT |= (1<<LCD_EN);
    _delay_us(10);
    LCD_CPRT &= ~ (1<<LCD_EN);
    _delay_us(3000);
}

bool sw1 (){
    if (PINB & (1<<BUTTON1)) {
        return false;
    }
    else return true;
}
bool sw2 (){
    if (PINB & (1<<BUTTON2)) {
        return false;
    }
    else return true;
}
bool sw3 (){
    if (PINB & (1<<BUTTON3)) {
        return false;
    }
    else return true;
}

void lcd_init() {
    LCD_DDRD = 0xFF;
    LCD_CDDR = 0xFF;
    PORTB = 0b00111000;
    LCD_CPRT &=~(1<<LCD_EN);
    lcdCOmmand(0x33);
    lcdCOmmand(0x32);
    lcdCOmmand(0x28);
    lcdCOmmand(0x0e);
    lcdCOmmand(0x01);
    _delay_us(2000);
    lcdCOmmand(0x06);
}

void lcd_gotoxy(unsigned char x, unsigned char y) {
    unsigned char firstCharAdr[] = {0x80,0xC0,0x94,0xD4};
    lcdCOmmand(firstCharAdr[y-1]+x-1);
    _delay_ms(1);
}

void lcd_print(char * str) {
    unsigned char i = 0;
    
    while(str[i] != 0) {
        lcdData(str[i]);
        i++;
    }
}


int main(void)
{
    lcd_init();

    while (1){
        lcdCOmmand(1);
        lcd_gotoxy(1,1);
        lcd_print("The world is but");
        lcd_gotoxy(1,2);
        lcd_print("one country");
        _delay_ms(3000);
        if (sw1())
        {
            lcdCOmmand(1);
            lcd_gotoxy(1,1);
            lcd_print("and mankind its ");
            lcd_gotoxy(1,2);
            lcd_print("citizens     ");
            _delay_ms(3000);
        }
    }
}