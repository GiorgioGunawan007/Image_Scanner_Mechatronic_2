#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sci1.h"            /* Serial Functions */
#include "pll.h"                /* macro _SYSCLOCK */

// String Declarations
char* scan1_msg;
char* scan2_msg;
char* scan3_msg;
char* scan4_msg;
char* res_msg;
char* sample_msg;
char* freq_msg;
char* newline = "\r\n";

signed int getminAz(){
   
   signed int minAzimuth;
   
   // Output message to get min azimuth
   scan1_msg = "Enter scan range:\r\nmin azimuth: \0";
   SCI1_OutString(scan1_msg);
   
   // Get min azimuth as a signed integer from serial
   minAzimuth = SCI1_InSDec();
   SCI1_OutString(newline); 
   
   return minAzimuth;
}


signed int getmaxAz(){
   
   signed int maxAzimuth;
   
   // Output message to get max azimuth
   scan2_msg = "max azimuth: \0";
   SCI1_OutString(scan2_msg);
   
   // Get max azimuth as a signed integer from serial
   maxAzimuth = SCI1_InSDec();
   SCI1_OutString(newline);
   
   return maxAzimuth;
}


signed int getminEl(){
   
   signed int minElevation;
   
   // Output message to get min elevation
   scan3_msg = "min elevation: \0";
   SCI1_OutString(scan3_msg);
   
   // Get min elevation as a signed integer from serial
   minElevation = SCI1_InSDec();
   SCI1_OutString(newline);
   
   return minElevation;
}

signed int getmaxEl(){
  
  signed int maxElevation;
  
  // Output message to get max elevation
  scan4_msg = "max elevation: \0";
  SCI1_OutString(scan4_msg);
  
  // Get max elevation as a signed integer from serial 
  maxElevation = SCI1_InSDec();
  SCI1_OutString(newline);
  SCI1_OutString(newline);
  
  return maxElevation;  
}

unsigned short getres(){
  
  unsigned short resolution;
  
  // Output message to get resolution
  res_msg = "Enter resolution: (1=0.125 degrees, 2=0.25 degrees, 3=0.375 degress,...)\r\n\0";
  SCI1_OutString(res_msg);
  
  // Get resolution from serial input
  resolution = SCI1_InUDec();
  SCI1_OutString(newline);
  SCI1_OutString(newline); 
  
  return resolution; 
}

unsigned short getsampleNum(){
  
  unsigned short sampleNum;
  
  // Output message to get number of samples
  sample_msg = "Enter number of samples: \0";
  SCI1_OutString(sample_msg);
  
  // Get number of samples from serial
  sampleNum = SCI1_InUDec();
  SCI1_OutString(newline);
  SCI1_OutString(newline);
  
  return sampleNum;
    
}

unsigned short getfreq(){
  
  unsigned short freq;
  
  // Output message to get frequency
  freq_msg = "Enter sample frequency: \0";
  SCI1_OutString(freq_msg);
  
  // Get sample frequency from serial
  freq = SCI1_InUDec();
  SCI1_OutString(newline);
  SCI1_OutString(newline);
  
  return freq;
}

