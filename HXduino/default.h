#ifndef _DEF_
#define _DEF_

//#define USE_OTA //OTA or HTTP

#define VER "1.0"

//#define SERIAL 1 //debug serial
#define SCALE_BOILER	60 //psi
//#define MAX_WATER 300	//psi
#define Q_WATER	2016 	//Pulse/liter
//-- 
#define P_REG	0.8		//standby mode Bar -> 93*C
#define P_FLOW  1.3     //flow    mode 107*C
#define T_BEEP  90      //T hx ready
#define P_MIN   0.1     //P min steam

//#define VIN  (220+51)/51 //1v->5v
#define PUMP_ON  13     //gpio13, pump on counter
#define PRESS_ON 3      //gpio1 rx, pressure up button 
#define SSR_PIN  0      //gpio0,  ssr out
#define PBEEP    15 	//gpio15, beeper out
#define PBOILER  A0     //adc,   pressude analog
#define LWATER	 2	    //gpio2, flow counter
#define SSR_ON   LOW
#define SSR_OFF  HIGH
#define SDA      4
#define SCL      5
#define ON      LOW
#define OFF     HIGH
 
//#define LED      1      //Tx??

#define TIMER1 5000 //timer int 5ms
#define TBLINK 1000000/TIMER1 //1sec

#define thermoDO  12 //gpio12
#define thermoCLK 16 //gpio16
#define TeCS      14 //gpio14

#define WIFI_SSID "HX"
//graph
#define gLen 40
#define gMax 25
#define gX 74
#define gY 37

void beeper(int t, char i);
void set_disp();
void disp_flow();
void disp_heat();
void ver();
void ota_loop();
//void ota();
void ota_prog(unsigned int progress,unsigned int total);
char state();

#endif
