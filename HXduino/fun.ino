
#include "default.h"

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
