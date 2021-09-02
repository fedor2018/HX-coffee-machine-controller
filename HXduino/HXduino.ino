
#include "default.h"

#include <max6675.h>
#include <math.h>
#include <MedianFilter.h>
#include <ESP8266TimerInterrupt.h>
#include <ESP8266_ISR_Timer.h>
#include <ESPeasySoftwareSerial.h>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

//extern Adafruit_SSD1306 display;
MAX6675 Te61(thermoCLK, TeCS, thermoDO);
MedianFilter Te_filter(10);
ESP8266Timer Timer1;
//ESPeasySoftwareSerial swSer( -1, 3, false, 256);

float Tb=-99.9;
float Te=-99.9;
float Pb=0;//Pboiler Bar
float Lw=0;//Lwater mL
volatile char halfsec=0;//seconds counter
volatile unsigned int uptime=0;//uptime in sec
volatile unsigned int pump=0;//pump status
volatile unsigned int press=0;//pressostat status
volatile unsigned int flow=0;//flow counter
bool is_90=false;//over 90*C

void IRAM_ATTR TimingISR();

void beeper(int t=1000, char i=1){//ms
	while (i--){
		digitalWrite(PBEEP, HIGH);
		delay(t);
		digitalWrite(PBEEP, LOW);
		delay(t);
	}
}

void setup(){
//  swSer.begin(9600);
//  swSer.print("start\r\n");

    ESP.wdtEnable(WDTO_8S);
//-------------    
    pinMode(SSR_PIN, OUTPUT);
    digitalWrite(SSR_PIN, SSR_OFF);
    pinMode(PBEEP, OUTPUT);
    digitalWrite(PBEEP, LOW);
//    pinMode (LED, OUTPUT);  
//    digitalWrite(LED, HIGH);
    pinMode (PUMP_ON, INPUT_PULLUP);
    pinMode (PRESS_ON, INPUT_PULLUP);
//-----
    ver();
//-----
    delay(500);
    Timer1.attachInterruptInterval(TIMER1, TimingISR);//1ms
//    digitalWrite(LED, LOW);
    pinMode(PUMP_ON, INPUT_PULLUP);//INT0
    attachInterrupt(PUMP_ON, pumpISR, FALLING );
    attachInterrupt(PRESS_ON, pressISR, FALLING ); //3
    interrupts();
}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{
    Te = (float)Te_filter.process(Te61.readCelsius());
    if(isnan (Te))Te=-99;
  	Pb=pressure(analogRead(PBOILER),MAX_BOILER);
  	if(Pb>=0 && Pb<P_REG){
  		digitalWrite(SSR_PIN, SSR_ON);
  	}else{
  		digitalWrite(SSR_PIN, SSR_OFF);
  	}
    if(pump){//pump on
  		if(halfsec==0)flow=0;
    }else{
  		halfsec=0;
    }
    pump=0;
  	Lw=flow2ml(flow);
//-----
  	if(press){//pressostat on
        disp_flow(Te, Pb, Lw, uptime);
  	}else{
        disp_heat(Te, Pb, Lw, uptime);
    }
  	press=0;
//-----
//-----
    if(Te>90 && is_90==false){
    	is_90=true;
    	beeper(500,3);
    }else{
    	delay(200);
    }
    ESP.wdtFeed();    
}

void pumpISR(){
  pump++;
}

void pressISR(){
  press++;
}

void IRAM_ATTR TimingISR(){
    static int hcnt=TBLINK;
    static char spump=1;
    static char spress=1;
    static char sflow=1;
    
    if(hcnt++<=0){//1sec
    	hcnt=TBLINK;
        uptime++;
      if(pump){
        if(++halfsec>99)halfsec=0;
      }
    //	  digitalWrite( LED, digitalRead( LED ) ^ 1 );
      }
    
    if(digitalRead(PUMP_ON)==LOW){//сработал датчик
    	if(spump==1){//только 1 за период
    		spump=0;
    		pump++;
    	}
    }else{
    	spump=1;
    }
/*    
    if(digitalRead(PRESS_ON)==LOW){//сработал датчик
    	if(spress==1){//только 1 за период
    		spress=0;
    		press++;
    	}
    }else{
    	spress=1;
    }
*/    
    if(digitalRead(LWATER)==LOW){//сработал датчик
    	if(sflow==1){//только 1 за период
    		sflow=0;
    		flow++;
    	}
    }else{
    	sflow=1;
    }
}

