
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
//    return p; 
    if (p < 0.5){//short
    	p = -8;//
    }else if(p> 4.5){//no sensor
    	p=-9;
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

char state(){
  	if(Pb>=0){
        if(is_90==false && Pb<P_FLOW){
//            disp_state("WARM");
            return 1;
        }
        if (is_90 && Pb<P_REG && but==OFF && pump==0){
//            disp_state("HEAT");
            return 2;
        }
        if(Pb<P_FLOW && but==ON){
//            disp_state("STEAM");
            return 3;
        }
        if(Pb<P_FLOW && pump){
//            disp_state("FLOW");
            return 4;
        }    
    }
//    disp_state("OFF");
    return 0;       
}
