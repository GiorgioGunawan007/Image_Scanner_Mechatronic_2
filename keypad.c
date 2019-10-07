#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sci1.h"            /* Serial Functions */
#include "pll.h"                /* macro _SYSCLOCK */
#include "LCD.h"              // LCD Header file
#include "keypad.h"         // Keypad Header

// LCDmessagesShortcut
signed int askMinAz(){

    signed int minAzimuth;   // Create signed integer for storing the minimum azimuth (-50 to -20)
    char choice;                     // This variable stores the user's input
    choice = ' ';                      // Sets user input default to an arbitrary space char  
    
    // Display to ask for minimum azimuth 
    putsLCD("Min Az:1.-50 2.");
    cmd2LCD(0xC0);            // Newline
    
    // 2nd line - Ask for minimum azimuth 
    putsLCD("-40 3.-30 4.-20");
   
    // Ask for input
    do{
	    choice = getKeyValue();    // Function to get keypad value
    }while(choice ==' ');               // Will wait until user presses something
   
    // If user presses:
   // 1 --> min Azimuth = -50
   // 2 --> min Azimuth = -40
   // 3 --> min Azimuth = -30
   // 4 --> min Azimuth = -20
   // Anything else --> min Azimuth = -40 (by default as part of error handling)
    if(choice == '1' ){
          minAzimuth = -50;           
          putsLCD("-50");                // Subsequently displayed
    } else if (choice == '2' ){
          minAzimuth = -40;          
          putsLCD("-40");
    } else if (choice == '3' ){
          minAzimuth = -30;           
          putsLCD("-30");
    } else if (choice == '4' ){
          minAzimuth = -20;          
          putsLCD("-20");
    } else{
          minAzimuth = -40;          
          putsLCD("-40");
    }
    
    return minAzimuth;                // Function returns minimum azimuth for main program to use    
}

signed int askMaxAz(){

    signed int maxAzimuth;         // Create signed integer for storing the maximum azimuth (20 to 50)
    char choice;                            // This variable stores the user's input
    choice = ' ';                             // Sets user input default to an arbitrary space char  
    
    // Display option to ask for maximum azimuth 
    putsLCD("Max Az:1.50 2.40");
    cmd2LCD(0xC0);
    putsLCD("3.30 4.20");
    
    // Ask for input, this part of code will wait until user enters something
    do{
	    choice = getKeyValue();
    }while(choice ==' ');
    
   
   // If user presses:
   // 1 --> max Azimuth = 50
   // 2 --> max Azimuth = 40
   // 3 --> max Azimuth = 30
   // 4 --> max Azimuth = 20
   // Anything else --> max Azimuth = 40
    if(choice == '1' ){
          maxAzimuth = 50;
          putsLCD("50");
    } else if (choice == '2' ){
          maxAzimuth = 40;
          putsLCD("40");
    } else if (choice == '3' ){
          maxAzimuth = 30;
          putsLCD("30");
    } else if (choice == '4' ){
          maxAzimuth = 20;
          putsLCD("20");
    } else{
          maxAzimuth = 40;
          putsLCD("40");
    }
    
    return maxAzimuth;  // Function returns max Azimuth values to be used in the main code     
}

signed int askMinEl(){

    signed int minElevation;                 // Create signed integer for storing the minimum elevation (-25 to -15)
    char choice;                                     // Storing user variable
    choice = ' ';                                      // Default to spac character (arbitrary)
  
    // Display option to ask for minimum elevation
    putsLCD("Min Elev:1.-25");              
    cmd2LCD(0xC0);
    putsLCD("2.-20 3.-15");
    
    
    // Ask for input, this part of code will wait until user enters something
    do{
	    choice = getKeyValue();
    }while(choice ==' ');
    
   
   // If user presses:
   // 1 --> min Elevation = -25 
   // 2 --> min Elevation = -20
   // 3 --> min Elevation = -15
   // Anything else (default value for error handling) --> min Elevation = -20
    if(choice == '1' ){
          minElevation = -25;
          putsLCD("-25");           // numbers are subsequently displayed
    } else if (choice == '2' ){
          minElevation = -20;
          putsLCD("-20");
    } else if (choice == '3' ){
          minElevation = -15;
          putsLCD("-15");
    } else{
          minElevation = -20;
          putsLCD("-20");
    }
    
    return minElevation;        // Function returns minimum elevation to be used in the main code    
}

signed int askMaxEl(){

    signed int maxElevation;   // value for storing the maximum elevation
    char choice;                        // value for storing user input from keypad
    choice = ' ';
  
    // Display option to ask for maximum elevation
    putsLCD("Max Elev:");
    cmd2LCD(0xC0);                    // newline
    putsLCD("1.25 2.20 3.15");
    
    // Ask for input
    do{
	    choice = getKeyValue();
    }while(choice ==' ');
    
   
   // If user presses:
   // 1 --> min Elevation = 25 
   // 2 --> min Elevation = 20
   // 3 --> min Elevation = 15
   // Anything else (default value for error handling) --> min Elevation = 20
    if(choice == '1' ){
          maxElevation = 25;
          putsLCD("25");
    } else if (choice == '2' ){
          maxElevation = 20;
          putsLCD("20");
    } else if (choice == '3' ){
          maxElevation = 15;
          putsLCD("15");
    } else{
          maxElevation = 20;
          putsLCD("-20");
    } 
    
    return maxElevation;    // Return maximum elevation for main code to use 
}

// for this resolution function, the user will be prompted to use the keypad
// to input a number. The number will translate to another integer which is 
// the assigned resolution multiplied by 8. Hence in the main program when
// this function is called, unsigned integer returned from this function has
// to be divided by 8
unsigned int askResolution(){

    unsigned int resolution;      // Create unsigned integer (as resolution never goes to negatuve) for storing the resolution  (0.125 to 2)
    char choice;                         // for storing user input 
    choice = ' ';                          // abritrarily set to space character
    
    // Display option to ask for resolution 
    putsLCD("Res: 1.0.125");     
    cmd2LCD(0xC0);             // newline
    putsLCD("2.0.5 3.1 4.2");
   
   // If user presses:
   // 1 --> resolution will be 0.125
   // 2 --> resolution will be 0.5
   // 3 --> resolution will be 1
   // 4 --> resolution will be 2
   // Anything else (default value for error handling) --> resolution will be 1
    // Ask for input
    do{
	    choice = getKeyValue();
    }while(choice ==' ');
   
    if(choice == '1' ){
          resolution = 1;          // it is not 0.125 but 1 because in the main file the integer here is divide by 8
          putsLCD("0.125");     // subvsequently show the corresponding resolution
    } else if (choice == '2' ){
          resolution = 4;
          putsLCD("0.5");
    } else if (choice == '3' ){
          resolution = 8;
          putsLCD("1");
    } else if (choice == '4' ){
          resolution = 16;
          putsLCD("2");
    } else{
          resolution = 1;
          putsLCD("1");
    }
    
    return resolution;        // Function returns resolution for use later in the code
          
}

// This function asks for number of samples of distances that is to be used. If the user picks 4,
// there will be 4 distances for every point and it will be averaged to get the distance for 
// that particular point
unsigned int askSampleNum(){

    unsigned short sampleNum;  // Create unsigned short for number of sample going from 1 to 8
    char choice;                            //  Create user input variable
    choice = ' ';                            // abritrarily set to space character as default 
    
    // Display option to ask for number of samples 
    putsLCD("Sample Num: ");    
    cmd2LCD(0xC0);
    putsLCD("1.1 2.2 3.4 4.8");
   
    // Ask for input
    do{
	    choice = getKeyValue();    // wait for user to input anything on the keypad
    }while(choice ==' ');
    
   // If user presses:
   // 1 --> sample number will be 1
   // 2 --> sample number will be 2
   // 3 --> sample number will be 4
   // 4 --> sample number will be 8
   // Anything else (default value for error handling) --> sample number will be 2   
    if(choice == '1' ){
          sampleNum = 1;
          putsLCD("1");
    } else if (choice == '2' ){
          sampleNum = 2;
          putsLCD("2");
    } else if (choice == '3' ){
          sampleNum = 4;
          putsLCD("4");
    } else if (choice == '4' ){
          sampleNum = 8;
          putsLCD("8");
    } else{
          sampleNum = 2;
          putsLCD("2");
    } 
    
    return sampleNum;     // returns sample number to be used in the main code      
}

// This function asks for the frequency from the user. That is the speed of the process
// On the keypad, it has 3 options from slow to fast
unsigned int askFrequency(){

    unsigned short freq;       // Create unsigned short for  frequency going from 1 to 3
    char choice;                    // to store user input from keypad
    choice = ' ';                     // Arbitrarily set to space character as default value
    
    // Display option to ask for frequency
    putsLCD("Freq: 1.Slow");
    cmd2LCD(0xC0);
    putsLCD("2.Medium 3.Fast");
   
    // Ask for input
    do{
	    choice = getKeyValue();    // wait for user to input anything on the keypad
    }while(choice ==' ');
    
   // If user presses:
   // 1 --> frequency will be slow
   // 2 --> frequency will be medium
   // 3 --> frequency will be fast 
   // Anything else (default value for error handling) --> frequency willbe medium     
    if(choice == '1' ){
          freq = 1;
          putsLCD("Slow");
    } else if (choice == '2' ){
          freq = 2;
          putsLCD("Med");
    } else if (choice == '3' ){
          freq = 3;
          putsLCD("Fast");
    } else{
          freq = 2;
          putsLCD("Med");
    } 
    
    return freq;     // return frequnecy to be used later in the main code    
}

// Get value from keypad ------------------------------------------------------------------------------------------


// Getting value for keypad
char getKeyValue(void){

  // User input
  char key;
  
  // Create an 4 by 4 array to model the keypad
  const unsigned char keypad[4][4] =
  {
  '1','2','3','A',
  '4','5','6','B',
  '7','8','9','C',
  '*','0','#','D'
  };
  
  // This character will store the number of column that the position is at
  unsigned char column;
  
  // This character will store the number of row that the position is atr
  unsigned char row;
  
  // Set Data Register of PORT A:
  // configure lower 4 pins as input
  DDRA = 0x0F;
  
  // Infinite loop that keeps on checking if there is an input
  // this loop will also check for debouncing (15 ms wait)
  while(1){
 
      // variable row takes 4 MSB and if it has an input (row not 0x00) will get out of the loop
      do{
          PORTA = PORTA|0x0F;
          row = PORTA & 0xF0;   
      } while(row == 0x00);
    
      do{
      
          // similar to above, variable row takes in PORTA's 4 MSB to check for the rows
          // if row = 0x00 still, keep onlooking as keypad is not pressed
          do{
            delayby10us(100); //1 ms delay
            row = PORTA & 0xF0;
          }while(row == 0x00);
      
          delayby10us(1500); // 15ms debouncing wait  to ensure a human actually pressed it and not noise
          row = PORTA & 0xF0;  // Get the row variable again    
      }while(row == 0x00); 
      
      // Check the column
      while(1){
          PORTA &= 0xF0;       
          PORTA |= 0x01;        // key switch detected is pressed at first column
          row = PORTA & 0xF0;  // get row value
          
          // If row has an input (it has to have one for the function to work)
          if(row != 0x00){
              column = 0;
              break;
          }
          PORTA &= 0xF0;
          PORTA |= 0x02;            // key switch detected is pressed at second column
          row = PORTA & 0xF0;  // get row value
          
          // If row has an input (it has to have one for the function to work)
          if(row != 0x00){
              column = 1;              // Becomes 2nd column  (1 because index starts from 0)
              break;
          }
          PORTA &= 0xF0;
          PORTA |= 0x04;             // key switch detected is pressed at third column 
          row = PORTA & 0xF0;   // get row value
          if(row != 0x00){
              column = 2;                // Becomes 3rd column  (2 because index starts from 0)
              break;
          }
          PORTA &= 0xF0;
          PORTA |= 0x08;              // key switch detected is pressed at third column 
          row = PORTA & 0xF0;    // get row value
          if(row != 0x00){
              column = 3;                // Becomes 4th column  (3 because index starts from 0)
              break;
          }
          row = 0;                           // set row back to 0 for next loop (can be 0x00 but 0 works too)
      break;                                  // go out of the while loop
      }
      
      // Get the row and then get the number.
      // Since we already have column number from code above, now
      // get the row number and use the index to get the value we want
      if(row == 0x10){
      
          // If row 1 then parse through row index 0 (first row) and use the column
          // variable to determine which is the value that is pressed and assign variable
          // 'key' which stores the ASCII value of the alphanumeric number pressed
          key = keypad[0][column];
      } else if(row == 0x20){
      
          // If row 2 then parse through row index 1 (second row) and use the column
          // variable to determine which is the value that is pressed and assign variable
          // 'key' which stores the ASCII value of the alphanumeric number pressed      
          key = keypad[1][column];
      } else if(row == 0x40){
      
           // If row 3 then parse through row index 2 (third row) and use the column
          // variable to determine which is the value that is pressed and assign variable
          // 'key' which stores the ASCII value of the alphanumeric number pressed
          key = keypad[2][column];
      }  else if(row == 0x80){
      
           // If row 4 then parse through row index 3 (fourth row) and use the column
          // variable to determine which is the value that is pressed and assign variable
          // 'key' which stores the ASCII value of the alphanumeric number pressed
          key = keypad[3][column];
      }
      
      // Debounce again to ensure that human  is pressing and it is not noise
      do{
          delayby10us(1500); // 15ms debouncing wait
          PORTA = PORTA | 0x0F;
          row = PORTA & 0xF0;    // get row value  for next iteration
      }while(row != 0x00);
      
      return key;     // return the ASCII value representation of button pressed to the main code
  }     
}

// Function that reverses a string, to be used when converting floating or integer to string
void reverse(char *str, int len) 
{ 

     // Set up variables
    int i=0, j=len-1, temp; 
    
    // While the index parsing is less than the total amount of characters in the string,
    // the program will keep on running
    while (i<j) 
    { 
    
        // Store variable in the 'temp' variable that is change every iteration  and 
        // is only used in this while loop as a temporary variable
        temp = str[i]; 
        str[i] = str[j];          // Reverses the n with (length - n)
        str[j] = temp;         // string at index j will be the string previously at index i
        
        // Continuously iterate to meet at middle point, in which program willl stop
        // and the string will already be reversed.
        i++; j--; 
    } 
} 

void itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    int isNegative = 0; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = 1; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 
} 

