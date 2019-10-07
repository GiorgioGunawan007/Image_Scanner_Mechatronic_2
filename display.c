// Defines inlcudes for the display c file
#include <hidef.h>          // Defines common macros and register definitions
#include "derivative.h"     // Defines common macros and register definitions
#include <math.h>           // Includes the math functions for c programming languages
#include "display.h"        // Header file for the 7-seg display interface fucntion prototypes for displaying the distance
#include "iic.h"            // Header file for iic interface function prototypes
#include "sci1.h"           // Header file for the Serial interface function prototypes


// Defines a look up table for the 7 seg display numbers  and an array that contains the hex numbers to
// enable each of the seven segments displays connected to Port P in order
int sevenSegOut[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int sevenSegEnable[4] = {0xFE,0xFD,0xFB,0xF7};

// This function takes a distance in in mm and displays it to the seven segment display
void displayDist(unsigned int num)
{
    
    // Defines arrays to store digits individually
    int digits[4];

    // Splits the 4 digit distace into 4 separate digits and stores one in each index of the array
    splitNum(digits,num);

    // Takes the created digits array that was passed from the splitNum functiona nd displays each digit
    // to the screen in order
    displayDigits(digits);

    // returns from the function
    return;
}

// This function takes in an array and a number and splits the number into separate digits storing
// each digit in a separate index in each array
void splitNum(int *array,int number){
  
  // Defines an integer variable to store the position in the number of the digit
  int pos;
  
  // Defines a variable to store the remainder that is used in the calculation
  int remainder;
  // Conversion to a base 10 number
  int base = 10;
  
  // Loops thorugh the digits array
  for(pos = 3; pos >= 0;pos--){
    // Obtain the remainder through the modulus
    remainder = number%base;
    
    // Stores the reaminder (digit) in it's own index in the function input array
    array[pos] = remainder;
    
    // Progressively moves down to the next digit and repeats
    number /= base;
  }
}

// Function to display the digits on the seven seg
void displayDigits(int *arr){
  
  // Counter variable definition
  int a;
  int b;
  int digit; // int variable to store digits  
  
  // Initialises the registers required for seven seg use
  DDRB = 0xFF;
  DDRP = 0xFF;
  DDRJ = 0xFF;
  PTJ = 0;
  

  // Loops thorugh all the digits that have been obtaned now that the number has been split  
  for(b = 0; b < 4 ; b++){
      
    // Variable to hold the digit to be displayed
    digit = arr[b];
    
    // Writes the digit to port B
    PORTB = sevenSegOut[digit];
    
      
    // Enables the correct seven seg
    PTP = sevenSegEnable[b];
    
    // Creates a delay
    sevSegDelay(1);
      
  }

  return;
}


// Delay function that is used in the seven segment display
void sevSegDelay(long int a){
  int ix;
  TSCR1 = 0x90; // enable TCNT and fast timer flag clear */
  TSCR2 = 0x03; // disable timer interrupt, set prescaler to 8*/
  TIOS |=0x10; // enable TC5
  TC5 =TCNT + 100; // less than 10 micro secodn delay (Check this calculation in the lab book on the weekend
  for(ix =0; ix < a; ix++){
    while(!(TFLG1 & TFLG1_C5F_MASK));
    TC0 += 100;
  }
}
