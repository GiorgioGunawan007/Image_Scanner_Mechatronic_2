// Defines the includes for the ptu.c file
#include <hidef.h>            // Defines common macros and registers
#include "derivative.h"      
#include "ptu.h"
#include "LIDAR_V2Func.h"
#include "iic.h"
#include "imu.h"
#include "LCD.h"

// Define messages for LCD display
char tilt_message[7] = "tilt: \0";
char heading_message[9] = "heading: ";

// Define an array to split up numbers and the array length
int array[3];
int length = 3;
  
// PTU function to scan the desired area row by row
void ptu(unsigned short res, signed int min_az, signed int max_az, signed int min_ele,signed int max_ele,unsigned short n_sample, unsigned short freq){
 
  // Define variables for max/min azimuth/elevation for PWM duty register
  unsigned short maxcol;
  unsigned short mincol;
  unsigned short maxrow;
  unsigned short minrow;
  
  // Define index variables for the nested for loop
  unsigned short i;
  unsigned short j;
  
  // Define strings to store tilt and heading orientation of PTU
  char current_heading[4];
  char current_tilt[4];
  
  // Set up registers
  PWMCTL = 0xCC;       // Concatinate channels 6/7 and channels 4/5 of PWM
  PWMPRCLK = 0x55;     // Set prescaler of 32 for the clocks
  PWMCLK = 0;          // Select clock A/B as the clock source
  PWMPOL = 0xA0;       // Set output of PWM signal to high at the start
  PWMCAE = 0;          // Select the left-aligned output mode
  
  PWMPER5 = 0x98;   // Period register value for PWM channel 5
  PWMPER4 = 0x3A;
  
  PWMPER7 = 0x98;   // Period register value for PWM channel 7
  PWMPER6 = 0x3A;
  
  // Set max/min servo orientation in heading direction for PWMDTY
  maxcol = 1205+(max_az*8);
  mincol = 1205+(min_az*8);
  
  // Set max/min servo orientation in tilt direction for PWMDTY
  maxrow = 1295+(max_ele*8);
  minrow = 1295+(min_ele*8);
  
  // Set initial tilt orientation of servo
  i = maxrow;
  PWMDTY7 = i & 0xff;    // lower order byte goes to duty register 7 
  PWMDTY6 = (i >> 8);    // higher order byte goes to duty register 6
  
  // Set initial heading orientation of servo
  j = maxcol;
  PWMDTY5 = j & 0xff;
  PWMDTY4 = (j >> 8);
  
  // Enable PWM channels 7 and 5
  PWME = 0xA0;
  
  // Calculate current orientation of servo in degrees for the LCD display and convert to string
  calculate_tilt(array,current_tilt,i);
  calculate_heading(array,current_heading,j);
  
  // Takes an initial distance measurement
  pulseWidthMeasure(n_sample,freq);
  SCI1_OutChar(0x2C);
  
  // Displaying on LCD
  openLCD();                  // Initialise LCD
  putsLCD(tilt_message);      // Write 'tilt: ' on LCD
  putsLCD(current_tilt);      // Put the string containing current value of tilt on LCD
  cmd2LCD(0xC0);              // Go to next line on LCD
  putsLCD(heading_message);   // Write 'heading: ' on LCD
  putsLCD(current_heading);   // Put the string containing current value of heading on LCD
  
  //SCI1_OutChar(0x0A);
  //SCI1_OutChar(0x0D);
  SCI1_OutUDec((unsigned short)i);
  SCI1_OutChar(0x2C);
         
  //SCI1_OutChar(0x0A);
  //SCI1_OutChar(0x0D);
  SCI1_OutUDec((unsigned short)j);
  SCI1_OutChar(0x0A);
  SCI1_OutChar(0x0D);
  
  // Loop for going through rows (determines tilt orientation of PTU)
  for (i = (maxrow) ; i >= minrow; i-=res) {
    
    // If we are in any row except the top row, go 1 row down 
    if ( i < maxrow) {
          
          getSample();
          //delay(10);
          pulseWidthMeasure(n_sample,freq);
          //getIMUData();         
          //delay(100);
        
          // Go 1 row down by updating the PWM duty register value
          PWMDTY7 = i & 0xff;
          PWMDTY6 = (i >> 8);
        
          // Calculate new tilt orientation in degrees as a string for LCD display
          calculate_tilt(array,current_tilt,i);
        
          // Displaying on LCD
          openLCD();                  // Initialise LCD
          putsLCD(tilt_message);      // Write 'tilt: ' on LCD
          putsLCD(current_tilt);      // Put the string containing current value of tilt on LCD
          cmd2LCD(0xC0);              // Go to next line on LCD
          putsLCD(heading_message);   // Write 'heading: ' on LCD
          putsLCD(current_heading);   // Put the string containing current value of heading on LCD
          
          //SCI1_OutChar(0x0A);
          //SCI1_OutChar(0x0D);
          SCI1_OutUDec((unsigned short)i);
          SCI1_OutChar(0x2C);
          
          
          //SCI1_OutChar(0x0A);
          //SCI1_OutChar(0x0D);
          SCI1_OutUDec((unsigned short)j);
          SCI1_OutChar(0x0A);
          SCI1_OutChar(0x0D);          
    }    
 
      // Loop for going through columns
      for (j = (maxcol); j >= mincol; j -=res) {
        
        // Update the heading orientation of servo   
        getSample();
        pulseWidthMeasure(n_sample,freq);
        //getIMUData();
        PWMDTY5 = j & 0xff; // Chane curr
        PWMDTY4 = (j >> 8);  
        
        
        // Calculate new heading orientation in degrees as a string for LCD display
        calculate_heading(array,current_heading,j);
        
        // Displaying on LCD
        openLCD();                  // Initialise LCD
        putsLCD(tilt_message);      // Write 'tilt: ' on LCD
        putsLCD(current_tilt);      // Put the string containing current value of tilt on LCD
        cmd2LCD(0xC0);              // Go to next line on LCD
        putsLCD(heading_message);   // Write 'heading: ' on LCD
        putsLCD(current_heading);   // Put the string containing current value of heading on LCD
          
        
        //SCI1_OutChar(0x0A);
        //SCI1_OutChar(0x0D);
        SCI1_OutUDec((unsigned short)i);
        SCI1_OutChar(0x2C); 
          
        //SCI1_OutChar(0x0A);
        //SCI1_OutChar(0x0D);
        SCI1_OutUDec((unsigned short)j);
        SCI1_OutChar(0x0A);
        SCI1_OutChar(0x0D);
        
      }
      
  }
  
  getSample();
  //delay(10);
  pulseWidthMeasure(n_sample,freq);
  SCI1_OutChar(0x2C);
  //getIMUData();
        
  //SCI1_OutChar(0x0A);
  //SCI1_OutChar(0x0D);
  SCI1_OutUDec((unsigned short)i);
  SCI1_OutChar(0x2C); 
         
  //SCI1_OutChar(0x0A);
  //SCI1_OutChar(0x0D);
  SCI1_OutUDec((unsigned short)j);
  SCI1_OutChar(0x0A);
  SCI1_OutChar(0x0D);
   
  // Disable PWM channels 7 and 5
  PWME = 0x00;
  
  // Sends the Finished command via serial to MATLAB to indicate that scan has been completed
  SCI1_OutString("DONE,DONE,DONE\0");
  
  return;
}


// Function to calculate current tilt orientation
void calculate_tilt( int*array,char *current_tilt, unsigned int i){
  float tilt;
  int t;
  
  // Use formula to calculate tilt in degrees in range 0-180
  tilt = (i - 1295 + 720)/8;
  
  // Add 0.5 for correct rounding to nearest integer
  t = tilt+0.5;
  
  // Call split function to split value of tilt into an array of integers
  split(array, t, length);
  
  // Call itos function to convert the array of int to a string
  itos(current_tilt,array, length);
}

// Function to calculate current heading orientation and convert to string
void calculate_heading(int*array, char* current_heading,unsigned int j){
  float heading;
  int h;
  
  // Use formula to calculate heading in degrees in range 0-180
  heading = (j - 1205 + 720)/8;
  
  // Add 0.5 for correct rounding to nearest integer
  h = heading + 0.5;
    
  // Call split function to split value of heading into an array of integers
  split(array,h,length);

  // Call itos function to convert the array of int to a string
  itos(current_heading,array, length);
}

// Function to split digits into an array of int
void split(int *array, int number,int l) {
    
    int i;                              // Define a counter variable for a loop
    int remainder;                      // Variable to store the remainder for modulo
    
    if (number < 100){                  // If the number has 2 digits
      for (i=l-1; i>0; i--) {           // Loop through the array backwards
        remainder = number%10;          // The remainder of the modulo operation is the desired value at index i of the array
        array[i] = remainder;           // Store the digit in the array at index i
        number /= 10;                   // Integer divide the number by 10
      }
      array[0] = 0;                     // Add zero to the start of the 2 digit number so that it becomes 3 digits for LCD display
    } else {                            // If the number has 3 digits
      
      for (i=l-1; i>=0; i--) {          // Loop through the array backwards
          remainder = number%10;        // The remainder of the modulo operation is the desired value at index i of the array
          array[i] = remainder;         // Store the digit in the array at index i
          number /= 10;                 // Integer divide the number by 10
      }
    }
}

// Function to convert int array to string of char
void itos(char* string, int* array, int l) {
    
    int k;
    for (k = 0; k <l; k++){             // Go through the elements of the array
      
        string[k] = array[k] + 0x30;    // Convert each integer to a character by adding 0x30
    }
    
    string[l] = '\0';                   // Add null character to end of string
}


