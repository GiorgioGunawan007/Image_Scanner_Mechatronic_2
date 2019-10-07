// header file for the LCD functions
#ifndef _LCD_H_
#define _LCD_H_

void cmd2LCD (char cmd);
void openLCD(void);
void putcLCD(char cx);
void putsLCD(char *ptr); 
void delayby10us(int k);

#endif /* _LCD_H_ */