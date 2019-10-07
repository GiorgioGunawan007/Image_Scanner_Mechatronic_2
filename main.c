// Includes for .h files containing protoypes for functions for each module
#include <hidef.h>        // Common definitions and types
#include "derivative.h"   // Common definitions and types   
#include "iic.h"          // Header file for iic interface function prototypes
#include "pll.h"          // Header file for the pll function prototypes
#include "sci1.h"         // Header file for the Serial interface function prototypes
#include "LIDAR_V2Func.h" // Header file for the LIDAR function prototypes
#include "ptu.h"          // Header file for the PTU servo control function prototypes
#include "keypad.h"       // Header file for the keypad user interface function prototypes


// Variable Declarations
unsigned int option;       // Option variable to store input method choice ( 1 = Serial input, 2 = Board input)
signed int minAzimuth;     // Signed varaible to store the minimum azimuth angle
signed int maxAzimuth;     // Signed varaible to store the maximum azimuth angle
signed int minElevation;   // Signed varaible to store the minimum elevation angle
signed int maxElevation;   // Signed varaible to store the maximum elevation angle
unsigned short resolution; // Unsigned variable to store the resolution of the scan
unsigned short sampleNum;  // Unsigned variable to store the number of distance samples taken at each step
unsigned short freq;       // Unsigned variable to store the sampling frequency option inputted by the user

// Main Program entry point
void main(void) {
  
  // Defines a string that contains the newline characters for Serial output later in the program
  char* newline = "\r\n";
  
  // Initialises Timer 7 to output compare to be used as a delay at later points in the program
  Init_TC7();
  
  // Initialise Serial Port SCI1 at a Baud rate of 9600 Bauds
  SCI1_Init(BAUD_9600);
  
  // Waits until the option char is sent over serial and reads the user input via SCI1
  option = SCI1_InUDec();
  
  // Switch statement on the option variable
  switch(option){
    // Case 1 indicates the user has chosen to input user settings using the Serial interface and PC console from MATLAB
    case 1:
        // Reads in the minimum azimuth from the serial port
        minAzimuth = SCI1_InSDec();
        
        // Reads in the maximum azimuth from the serial port
        maxAzimuth = SCI1_InSDec();
        
        // Reads in the minimum elevation from the serial port
        minElevation = SCI1_InSDec();
        
        // Reads in the maximum elevation from the serial port
        maxElevation = SCI1_InSDec();
        
        // Reads in the scan resolution from the serial port
        resolution = SCI1_InUDec();
        
        // Reads in the number of distamce samples to be taken at each servo position from the serial port
        sampleNum = SCI1_InUDec();
        
        // Reads in the sampling frequency from the serial port
        freq = SCI1_InUDec();
        break;
    // Case 2 inidicates the user has chosen to use board input    
    case 2:
      // Opens the LCD screen to be used
      openLCD();
      
      // Reads in the minimum azimuth from the keypad
      minAzimuth = askMinAz();
      
      // Opens the LCD screen to be used
      openLCD();
      
      // Reads in the maximum azimuth from the keypad
      maxAzimuth = askMaxAz();
      
      // Opens the LCD screen to be used
      openLCD();
      
      // Reads in the minimum elevation from the keypad
      minElevation = askMinEl();
      
      // Opens the LCD to be written to
      openLCD();
      
      // Reads in the maximum elevation from the keypad
      maxElevation = askMaxEl();
      
      // Opens the LCD screen to be used 
      openLCD();
      
      // Reads in the scan resolution from the keypad
      resolution = askResolution();
      
      // Opens the LCD screen to be used 
      openLCD();
      
      // Reads in the number of distamce samples to be taken at each servo position from the keypad
      sampleNum = askSampleNum();
      
      // Opens the LCD screen to be used
      openLCD();
      
      // Reads in the sampling frequency from the keypad
      freq = askFrequency();
      
      // Outputs the resolution (if the board was used to input the user options) back to MATLAB via serial as the resolution is required in
      // teh plotting and the area calculation
      SCI1_OutChar(0x0A);
      SCI1_OutChar(0x0D);
      SCI1_OutUDec(resolution);
      SCI1_OutChar(0x0A);
      SCI1_OutChar(0x0D);
      
      break;
    default:
      // Displays an error message if the option was not recieved or a timeout occured
      SCI1_OutString("Error\0");
      break;
  }
  
  // Enables the interrupts
	EnableInterrupts;
  
  
  // Begins a scan of the object to be mapped using the paramters input by the user
  ptu(resolution,minAzimuth,maxAzimuth,minElevation,maxElevation,sampleNum,freq);
  
  // Main loop of the program that runs continuously
  for(;;) {     
  } 
}





