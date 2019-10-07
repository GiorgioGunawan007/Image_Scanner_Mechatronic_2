#include <hidef.h>
#include "derivative.h"
#include "iic.h"
#include "pll.h"
#include "sci1.h"
#include "imu.h"


volatile uint8_t alarmSignaled1 = 0; /* Flag set when alarm 1 signaled */
volatile uint16_t currentTime1 = 0; /* variables private to timeout routines */
uint16_t alarmTime1 = 0;
volatile uint8_t alarmSet1 = 0;

/* put your own code here */

char  myString[20];
char character;

uint8_t D2W, D3R, WHO, who;
int  res1, res2,  res3, *p;
float acc;


char buff[BUFF_SIZE];
int gxraw[BUFF_SIZE];
int gyraw[BUFF_SIZE],gzraw[BUFF_SIZE];

int axraw[BUFF_SIZE];
int ayraw[BUFF_SIZE],azraw[BUFF_SIZE];

int mxraw[BUFF_SIZE];
int myraw[BUFF_SIZE],mzraw[BUFF_SIZE];



void getIMUData(void){

//This program will send the gyro, accelerometer adn magnetometer data
// to a serial port
// You can connect the serial port, set it to 9600 bauds

 Init_TC6();

 iicinit();

// gyro_test(); // make sure a l3g is connected

  gyro_init();     // l3g4200 setup
  accel_init();
  magnet_init();


 //delay1(1000);

 l3g4200d_getrawdata( &gxraw, &gyraw, &gzraw) ;        // read data

 SCI1_OutString("Gyro Gx:");
 SCI1_OutUDec((unsigned short) gxraw[0]);
 SCI1_OutString(" Gy:");
 SCI1_OutUDec((unsigned short) gyraw[0]) ;
 SCI1_OutString(" Gz:");
 SCI1_OutUDec((unsigned short) gzraw[0]) ;

 SCI1_OutString("\r\n");


 // ADCL345 Accelerometer

  adxl345_getrawdata( &axraw, &ayraw, &azraw) ;        // read data
  SCI1_OutString("Accel Ax:");
  SCI1_OutUDec((unsigned short) axraw[0]);
  SCI1_OutString(" Ay:");
  SCI1_OutUDec((unsigned short) ayraw[0]) ;
  SCI1_OutString(" Az:");
  SCI1_OutUDec((unsigned short) azraw[0]) ;

  SCI1_OutString("\r\n");



  // HM5883_magnetometer

  hm5883_getrawdata(&mxraw, &myraw, &mzraw);

  SCI1_OutString("Magn Mx:");
  SCI1_OutUDec((unsigned short) mxraw[0]);
  SCI1_OutString(" My:");
  SCI1_OutUDec((unsigned short) myraw[0]) ;
  SCI1_OutString(" Mz:");
  SCI1_OutUDec((unsigned short) mzraw[0]) ;

  SCI1_OutString("\r\n");
  return;

}


// Magnetometer

void magnet_init(void){

  int  res1;
  res1=iicstart(magnet_wr);
  res1=iictransmit(HM5883_MODE_REG );  //
  res1=iictransmit(0x00 );
  iicstop();

}


void magnet_test(void){

}

void hm5883_getrawdata(int *mxraw, int *myraw, int *mzraw){

 uint8_t i = 0;
 uint8_t buff[6];
 int res1;

 res1=iicstart(magnet_wr);
 res1=iictransmit(HM5883_DATAX0 );
 res1= iicrestart(magnet_rd);
 iicswrcv();

 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }

 buff[i]= iicreceivem1();
 buff[i+1]= iicreceivelast();

	*mxraw = ((buff[0] << 8) | buff[1]);
	*myraw = ((buff[2] << 8) | buff[3]);
	*mzraw = ((buff[4] << 8) | buff[5]);
}

void accel_test(void){}


void accel_init (void){

 int  res1;

 res1=iicstart(accel_wr);
 res1=iictransmit(ADXL345_POWER_CTL );  //
 res1=iictransmit(0x08 );

 res1=iictransmit(ADXL345_DATA_FORMAT );  // ;
 res1=iictransmit(0x08 );

 iicstop();
}


void adxl345_getrawdata(int *axraw, int *ayraw, int *azraw){

 uint8_t i = 0;
 uint8_t buff[6];
 int res1;

 res1=iicstart(accel_wr);
 res1=iictransmit(ADXL345_DATAX0 );
 res1= iicrestart(accel_rd);
 iicswrcv();

 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }

 buff[i]= iicreceivem1();
 buff[i+1]= iicreceivelast();

	*axraw = ((buff[1] << 8) | buff[0]);
	*ayraw = ((buff[3] << 8) | buff[2]);
	*azraw = ((buff[5] << 8) | buff[4]);
}

// test the precense of Gyro , should get 211 on return
//

void gyro_test(void) {
 int res1,who;

 res1=iicstart(0xD2);
 res1=iictransmit(L3G4200D_WHO_AM_I);

 res1=iicrestart(0xD3);
 who=iicreceiveone();
 //who=who & 0x00ff;     Debugging  info
 //PORTB=  who ;

}

 //  Gyro Initialisation

 void gyro_init (void) {

 int  res1;

 res1=iicstart(gyro_wr);
 res1=iictransmit(L3G4200D_CTRL_REG1 );  // ; 100hz, 12.5Hz, Power up
 res1=iictransmit(0x0f );
 iicstop();
 }


// Function to get a set of gyro data
// Eduardo Nebot,30 July 2015

void l3g4200d_getrawdata(int *gxraw, int *gyraw, int *gzraw) {
 	uint8_t i = 0;
	uint8_t buff[6];
	int res1;

   res1=iicstart(gyro_wr);
   res1=iictransmit(L3G4200D_OUT_XYZ_CONT );
   res1= iicrestart(gyro_rd);

 iicswrcv();

 for(i=0; i<4  ;i++) {
  buff[i]=iicreceive();
 }

buff[i]= iicreceivem1();
buff[i+1]= iicreceivelast();

	*gxraw = ((buff[1] << 8) | buff[0]);
	*gyraw = ((buff[3] << 8) | buff[2]);
	*gzraw = ((buff[5] << 8) | buff[4]);
}


void setAlarm1(uint16_t msDelay1)
{
    alarmTime1 = currentTime1 + msDelay1;
    alarmSet1 = 1;
    alarmSignaled1 = 0;
}


void delay1(uint16_t msec)
{
    TC6 = TCNT + 24000; // Set initial time
    setAlarm1(msec);
    while(!alarmSignaled1) {};
}



/*  Interrupt   EMN */

// interrupt(((0x10000-Vtimch7)/2)-1) void TC7_ISR(void){
// the line above is to make it portable between differen
// Freescale processors
// The symbols for each interrupt ( in this case Vtimch7 )'
// are defined in the provided variable definition file
// I am usign a much simpler definition ( vector number)
// that is easier to understand

interrupt 14 void TC6_ISR(void) {

  TC6 =TCNT + 24000;   // interrupt every msec
  TFLG1=TFLG1 | TFLG1_C6F_MASK;
  currentTime1++;
    if (alarmSet1 && currentTime1 == alarmTime1)
    {
        alarmSignaled1 = 1;
        alarmSet1 = 0;
    }
   //PORTB=PORTB+1;        // count   (debugging)
}



void Init_TC6 (void) {

_asm SEI;

TSCR1=0x80;
TSCR2=0x00;   // prescaler 1
TIOS=TIOS | TIOS_IOS6_MASK;
TCTL1=0x40;
TIE=TIE | 0x40;;

 _asm CLI;

}
