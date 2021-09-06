
#include "default.h"

float flow2ml(int cnt){//counter to mL
	return cnt * Q_WATER/1000;
}

float psi2bar(float psi){
	return (psi/14.5037738);//1 Bar = 14,5037738 Psi
}

#define VIN  (220+51)/51 //1v->5v
float pressure(int adcval,int scale){//adc->bar
    float p;   
    p = ((float)(adcval*VIN/1024.0)); 
    if (p < 0.4){//short
    	p = -98;//
    }else if(p> 4.6){//no sensor
    	p=-99;
    }else{
    	p-=0.5;
    	p*=(scale/4.0);//psi
    	p=psi2bar(p);//bar
    }
    return p;
}

void beeper(int t=1000, char i=1){//ms
	while (i--){
		digitalWrite(PBEEP, HIGH);
		delay(t);
		digitalWrite(PBEEP, LOW);
		delay(t);
	}
}
