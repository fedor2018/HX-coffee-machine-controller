/*
Pb - P boiler steam 0-2.5 Bar
Pw - P water pump   0-19  Bar
Te - T in E61       0-120 *C
Tb - T boiler       0-110 *C
t  - timer          0-99  sec
L  - flow           0-200 ml
-----------
давление в бойлере - 1/4"
давление в магистрали - 1/8" тройник
расход воды - трубка d4x7 Q>100

SmallFont - 6x8
MediumNumbers - 12x16
BigNumbers - 14x24
*/
#include <OLED_I2C.h>
#include <max6675.h>
#include <TimerOne.h>
#include <math.h>
#include <MedianFilter.h>

#define SERIAL 1 //debug serial
#define MAX_BOILER	60//psi
#define MAX_WATER 300	//psi
#define Q_WATER	2016 	//Pulse/liter
#define P_REG	0.8		//Bar -> 93*C

#define VIN 5//Vcc
#define PUMP_ON  2	//int 0
#define PRESS_ON 3//int 1
#define SSR_PIN  4 //heater off
#define PBEEP    5 	//buzzer
#define PBOILER  A0//Pboiler 
#define PWATER	 A1//Pwater
#define LWATER	 6	//Lwater
#define SSR_ON LOW
#define SSR_OFF HIGH

int thermoDO = 10;
int thermoCLK = 12;
int TbCS = 11;//boiler
int TeCS = 13;//e61

MAX6675 Tboiler(thermoCLK, TbCS, thermoDO);
MAX6675 Te61(thermoCLK, TeCS, thermoDO);
OLED  display(SDA, SCL);
MedianFilter Te_filter(10);

float Tb=-99.9;
float Te=-99.9;
float	Pb=0;//Pboiler Bar
float Pw=0;//Pwater Bar
float Lw=0;//Lwater mL
#define TIMER1 5000 //timer int 5ms
#define TBLINK 1000000/TIMER1 //1sec
volatile char halfsec=0;//seconds counter
volatile unsigned int pump=0;//pump status
volatile unsigned int press=0;//pressostat status
volatile unsigned int flow=0;//flow counter
bool is_90=false;//over 90*C

void beeper(int t=1000, char i=1){//ms
	while (i--){
		digitalWrite(PBEEP, HIGH);
		delay(t);
		digitalWrite(PBEEP, LOW);
		delay(t);
	}
}

void setup(){
	pinMode(SSR_PIN, OUTPUT);
  digitalWrite(SSR_PIN, SSR_OFF);
  pinMode(PBEEP, OUTPUT);
  digitalWrite(PBEEP, LOW);
  pinMode (13, OUTPUT);  
  digitalWrite(13, HIGH);
  pinMode (PUMP_ON, INPUT_PULLUP);
  pinMode (PRESS_ON, INPUT_PULLUP);
  delay(500);
  Timer1.initialize(TIMER1);//1ms
  Timer1.attachInterrupt(TimingISR);
  digitalWrite(13, LOW);
  pinMode(PUMP_ON, INPUT_PULLUP);//INT0
//  attachInterrupt(0, pumpISR, FALLING );
//  attachInterrupt(1, pressISR, FALLING ); //3
  display.begin();
  interrupts();
#ifdef SERIAL
  Serial.begin(115200);
  Serial.print("start\r\n");
#endif

}

void loop(){
  Tb = Tboiler.readCelsius();
  Te = (float)Te_filter.process(Te61.readCelsius());
  if(isnan (Tb))Tb=-99;
  if(isnan (Te))Te=-99;
	Pb=pressure(analogRead(PBOILER),MAX_BOILER);
	if(Pb>=0 && Pb<P_REG){
		digitalWrite(SSR_PIN, SSR_ON);
	}else{
		digitalWrite(SSR_PIN, SSR_OFF);
	}
	Pw=pressure(analogRead(PWATER),MAX_WATER);
  if(pump){//pump on
		if(halfsec==0)flow=0;
  }else{
		halfsec=0;
  }
  pump=0;
	Lw=flow2ml(flow);
	if(press){//pressostat on
	}
	press=0;

//  display.setFont();

	display.update();
	if(Te>90 && is_90==false){
		is_90=true;
		beeper(500,3);
	}else{
  	delay(200);
  }
}

void pumpISR(){
  pump++;
}

void pressISR(){
  press++;
}

void TimingISR(){
	static int hcnt=TBLINK;
	static char spump=1;
	static char spress=1;
	static char sflow=1;
	
	if(hcnt++<=0){//1sec
		hcnt=TBLINK;
	  if(pump){
	    if(++halfsec>99)halfsec=0;
	  }
	  digitalWrite( 13, digitalRead( 13 ) ^ 1 );
  }

  if(digitalRead(PUMP_ON)==LOW){//сработал датчик
  	if(spump==1){//только 1 за период
  		spump=0;
  		pump++;
  	}
  }else{
  	spump=1;
  }

  if(digitalRead(PRESS_ON)==LOW){//сработал датчик
  	if(spress==1){//только 1 за период
  		spress=0;
  		press++;
  	}
  }else{
  	spress=1;
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

float flow2ml(int cnt){//counter to mL
	return cnt * Q_WATER/1000;
}

float psi2bar(float psi){
	return (psi/14.5037738);//1 Bar = 14,5037738 Psi
}

float pressure(int adcval,int max){//adc->bar
	float p;   
  p = ((float)(adcval*VIN/1024.0)); 
  if (p < 0.1){//short
  	p = -98;//
  }else if(p> 4.8){//no sensor
  	p=-99;
  }else{
  	p-=0.5;
  	p*=(max/4.0);//psi
  	p=psi2bar(p);//bar
  }
  return p;
}
/*
Input: 0-60 psi(Gauge Pressure);
Output: 0.5-4.5V linear voltage output. 0 psi outputs 0.5V, 30 psi outputs 2.5V, 60 psi outputs 4.5V.
Input: 0-300 psi(Gauge Pressure);
Output: 0.5-4.5V linear voltage output. 0 psi outputs 0.5V, 150 psi outputs 2.5V, 300 psi outputs 4.5V.

Accuracy: +/-0.5%FS;
Thread: 1/8” NPT;
Wiring Connector: Packard plug-in unit is included;
Wiring: Red: +5V; Black: ground; Green: signal output;
Overload Capacity: 2-4 times;
Working Temperature: -40—+120ºC;
Compensation Temperature: -20—+80ºC;
*/
