
#include "default.h"

#include <max6675.h>
#include <math.h>
//#include <MedianFilter.h>
#include <ESP8266TimerInterrupt.h>
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif
MAX6675 Te61(thermoCLK, TeCS, thermoDO);
//MedianFilter Te_filter(10);
ESP8266Timer Timer1;

float Te=-99.9;
float Pb=0;//Pboiler Bar
float Lw=0;//Lwater mL
volatile char halfsec=0;//seconds counter
volatile unsigned int uptime=0;//uptime in sec
volatile unsigned int loopcnt=0;//loop in sec/5
volatile unsigned int pump=0;//pump status
volatile unsigned int press=0;//pressostat status
volatile unsigned int flow=0;//flow counter
bool is_90=false;//over 90*C

void IRAM_ATTR TimingISR();
void  ICACHE_RAM_ATTR pumpISR();

void setup(){
    Serial.begin(57600, SERIAL_8N1, SERIAL_TX_ONLY);
    Serial.println("\r\nstart");
//-------------    
    pinMode(SSR_PIN, OUTPUT);
    digitalWrite(SSR_PIN, SSR_OFF);
    pinMode(PBEEP, OUTPUT);
    digitalWrite(PBEEP, LOW);
    pinMode (PUMP_ON, INPUT_PULLUP);
    pinMode (PRESS_ON, INPUT_PULLUP);
//-----
    ver();
//    beeper(500,1);
//-----
    ESP.wdtDisable();
    delay(500);
/*    if(ESP.system_get_rst_info()){//error rst
        disp_rst();
        while(1)yield();
    }
*/
//    if(!digitalRead(PRESS_ON)){//ota mode
        ota_loop();
//    }
    Timer1.attachInterruptInterval(TIMER1, TimingISR);//1ms
    attachInterrupt(digitalPinToInterrupt(PUMP_ON), pumpISR, FALLING );
    interrupts();
}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop(){
//    Serial.println(loopcnt);//test!
    loopcnt=0;//reset loop wdt
    char but=digitalRead(PRESS_ON);
    Te = Te61.readCelsius();
    if(isnan (Te))Te=-99;
//    Te = (float)Te_filter.process(Te61);
  	Pb=pressure(analogRead(PBOILER),SCALE_BOILER);
  	if(Pb>=0 && 
        ((but==1&&pump==0) && Pb<P_REG)||((but==0||pump!=0) && Pb<P_FLOW)){
  		digitalWrite(SSR_PIN, SSR_ON);
  	}else{
  		digitalWrite(SSR_PIN, SSR_OFF);
  	}
  	Lw=flow2ml(flow);
    if(pump){//pump on
  		if(halfsec==0)flow=0;
        disp_flow();
    }else{
  		halfsec=0;
        disp_heat();
    }
    pump=0;
//-----
    if(Te>90 && is_90==false){
    	is_90=true;
    	beeper(500,3);
    }else{
    	delay(200);
    }
//    ESP.wdtFeed();    
}

void  ICACHE_RAM_ATTR pumpISR(){
  pump++;
}

//void pressISR(){
//  press++;
//}

void IRAM_ATTR TimingISR(){
    static int hcnt=TBLINK;
    static char spump=1;
//    static char spress=1;
    static char sflow=1;

    if(loopcnt++>40*8){//8s
      digitalWrite(SSR_PIN, SSR_OFF);
    }  
    if(hcnt--<=0){//1sec
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
     
    if(digitalRead(LWATER)==LOW){//сработал датчик
    	if(sflow==1){//только 1 за период
    		sflow=0;
    		flow++;
    	}
    }else{
    	sflow=1;
    }
}
