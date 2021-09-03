#ifndef _DEF_
#define _DEF_

#define VER 1.0

//#define SERIAL 1 //debug serial
#define MAX_BOILER	60 //psi
//#define MAX_WATER 300	//psi
#define Q_WATER	2016 	//Pulse/liter
#define P_REG	0.8		//Bar -> 93*C
#define P_FLOW  1.3     //flow mode

#define VIN  (220+51)/51 //5v->1v
#define PUMP_ON  13      //gpio13, pump on counter
#define PRESS_ON 1      //gpio1 tx, flow button 
#define SSR_PIN  0      //gpio0,  ssr out
#define PBEEP    15 	//gpio15, beeper out
#define PBOILER  A0     //adc,   pressude analog
#define LWATER	 2	    //gpio2, flow counter
#define SSR_ON   LOW
#define SSR_OFF  HIGH
#define SDA      4
#define SCL      5 
#define LED      3      //rx??

#define TIMER1 5000 //timer int 5ms
#define TBLINK 1000000/TIMER1 //1sec

#define thermoDO  12 //gpio12
#define thermoCLK 16 //gpio16
#define TeCS      14 //gpio14

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "NotoSans12pt7b.h"
#include "NotoSans22pt7b.h"

void beeper(int t, char i);
void set_disp();
void disp_flow();
void disp_heat();
void ver();

#endif
