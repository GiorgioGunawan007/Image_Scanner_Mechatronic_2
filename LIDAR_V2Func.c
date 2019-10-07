// Definitions fof all include files
#include <hidef.h>          // Includes common definitions for files and register definitons
#include <math.h>           // Includes the math functions for c programming language
#include "derivative.h"     // Includes common definitions for files and register definitons
#include "iic.h"            // Header file for iic interface function prototypes
#include "LIDAR_V2Func.h"   // Header file for the LIDAR function prototypes
#include "sci1.h"           // Header file for the Serial interface function prototypes
#include "display.h"        // Header file for the 7-seg display interface fucntion prototypes for displaying the distance


// Defines unisgned integer variables for the difference between edges in the PWM wave, the first edge to be captured and the overflow count
unsigned int difference, firstEdge, oflow;

// Defines variables for the pulse width to be measured in clock cycles (An array that stores multiple values)
unsigned long int pulse_width[2];

// Defines the function that will obtain the pulse width measurement
void pulseWidthMeasure(unsigned short nSamples, unsigned short freq){      
 
   // Defines a variable to store the distance to display in mm
   unsigned int dispDist;
   
   // Defines the variable that stores the average distance 
   float avgDist = 0;
   
   // Defines a sample variable that is used in later distance calculations
   float sample = 0;
   
   // Defines two counter variables i and j used later in the laser distance calculations and display
   int i;
   int j;
    
   // Define a variable that is used to control the sampling frequency
   int stepDelay;
   
   // Uses a switch statement to select an appropriate sampling frequency value that is used in a later loop
   switch(freq){
    
    // Case 1 is for the Slow sampling speed
    case 1:
        stepDelay = 1500;
        break;
    // Case 2 is for the medium sampling speed
    case 2:
        stepDelay = 200;
        break;
    // Case 3 is for the fast sampling speed
    case 3:
        stepDelay = 25;
        break;
    // Default case is for the fast sampling speed
    default:
        stepDelay = 25;
        break;
   }
   
   // Loops through the samples selected by the user
   for(i = 0; i < nSamples; i++){
     // Disables interrupts
     DisableInterrupts;
     oflow = 0;             // Initialises overflow counter to 0

     TSCR1 = 0x80;             // Enables the timer
     TSCR2 = 0x05;             // Sets the pre-scaler to 32,no timer overflow interrupt (NOTE: Pre scaler value gives 1.33 microsecond per clock cycle)
     TIOS  |= ~TIOS_IOS1_MASK; // Sets Timer channel 1 to input capture
     TFLG1 = TFLG1_C1F_MASK;   // Clears the channel event occur flag
     TCTL4 = 0x04;             // Prepares channel 1 to recieve a rising edge
     while(!(TFLG1 & TFLG1_C1F_MASK));    // Wait for the rising edge to be recieved
     TFLG2 = 0x80;             // Clears the flag that indicates an interrupt has occured
     TSCR2 |= 0x80;            // Enables the Timer interrupt function (Uses OR to make sure current settings not changed)
     TFLG1 = TFLG1_C1F_MASK;   // Clears the channel event occur flag
     
     // Enables interrupts
     EnableInterrupts;

     firstEdge = TC1;              // Saves the Timer count at the first edge
     TCTL4 = 0x08;             // Prepares the timer to capture on a falling edge
     while(!(TFLG1 & TFLG1_C1F_MASK));    // Waits until the edge has been captured
     difference = TC1 - firstEdge; // Obtains the difference in timer count
     if(TC1 < firstEdge){          // Checks if the current count is less than the first edge captured
       oflow -= 1;          // If it is, subtacts one from the overflow variable
     }

     // Obtains the pulse width using a mulpitle of the overflow variable to obtain the pulse_width
     pulse_width[i] = (unsigned long)oflow * 65536 + (unsigned long)difference;
   }
   
   // Loops thorugh the array to calculate the average distance 
   for(j = 0; j < i; j++){
    
    // Converts measurement to mm and adds the pulsewidth samples to a total average distance variable
    sample = (float)pulse_width[j] * 1.33;
    avgDist += sample;
   }
   
   // Calulcates the average distance
   avgDist /= j;
   
   // Rounds the distance to the nearest mm to send to MATLAB
   avgDist = floor(avgDist);
   
   // Sends the distance to MATLAB via the SCI1 port
   SCI1_OutUDec((unsigned short)avgDist);
   // Outputs the comma as a delimiter for the data stream sent to MATLAB
   SCI1_OutChar(0x2C);

   // Stores the typecasted average distance to the dispDist variable to be displayed to the seven segment 
   dispDist = (unsigned int)avgDist;

  // Loops through and displays the distance in the seven segment display using the stepDelay variable to control the 
  // length of the delay and hence the sampling frequency
   for(i = 0; i < stepDelay; i++){
    // Displays the value
    displayDist(dispDist);
    // Clear Port B
    PORTB = 0x00;
   }

   // Disables the edge capture
   TCTL4 = 0x00;
   
   // Resets the timer control registers to match the delay
   TSCR1 = 0x80;
   TSCR2 = 0x00; // pre scaler of 1 again
   return;

}

// Sets up a timer overflow interrupt that will affect the value of the overflow count variable
interrupt 16 void timov(void){

  // Clears the Interrupt occur flag
  TFLG2 |= 0x80;
  oflow++;              // Increments the overflow variable
}

// Request an aquisition
void getSample(void){
  
  // Configures port H for output
  DDRH = 0xFF;
  PTH = 0xFF;
  PTH = 0x00;
  return;
}
  
