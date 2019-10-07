// Header file for the keypad input functions
#ifndef KEYPAD_H
#define KEYPAD_H
// Macro Definitions
#define BASE 10

// Function Declarations
signed int askMinAz();
signed int askMaxAz();
signed int askMinEl();
signed int askMaxEl();
unsigned int askResolution();
unsigned int askSampleNum();
unsigned int askFrequency();
char getKeyValue(void);
void itoa(int num, char* str, int base);
void reverse(char *str, int len);
#endif