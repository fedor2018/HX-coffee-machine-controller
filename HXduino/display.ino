
#include "default.h"

#include <Wire.h>
#include "SH1106.h"
SH1106 disp(0x3c, 4, 5);

int8_t buf[gLen];//graph buffer
char str[32];

void ver(){
    disp.init();
    disp.flipScreenVertically();;
    disp.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    disp.setFont(ArialMT_Plain_24);
    disp.drawString(disp.getWidth() / 2, disp.getHeight() / 2 ,"v. " VER);
    disp.display();
    Serial.println(F("firmware ver. " VER));
    memset(buf, 0, sizeof(buf));
}

void ota(const char *s){
    disp.clear();
    disp.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    disp.setFont(ArialMT_Plain_16);
    disp.drawString(disp.getWidth() / 2, disp.getHeight() / 2 , s);
    disp.display();
    Serial.println(s);
}

void otaf(PGM_P s){
    strcpy_P(str, s);
    ota((const char*)str);
}

void ota_prog(unsigned int progress, unsigned int total){
    disp.drawProgressBar(4, 40, 120, 8, progress / (total / 100) );
    disp.display();
}

void disp_state(){
    disp.setTextAlignment(TEXT_ALIGN_CENTER);
    disp.setFont(ArialMT_Plain_10);
    switch(st){
        case 1:
            strcpy_P(str, PSTR("WARM"));
            break;
        case 2:
            strcpy_P(str, PSTR("HEAT"));
            break;
        case 3:
            strcpy_P(str, PSTR("STEAM"));
            break;
        case 4:
            strcpy_P(str, PSTR("FLOW"));
            break;
        case 0:
            strcpy_P(str, PSTR("OFF")); 
            break;
        default:
            strcpy_P(str, PSTR("ERR")); 
    }
    disp.drawString(32,10, str);
}

void printT(int p){//temp
    disp.setTextAlignment(TEXT_ALIGN_RIGHT);
    disp.setFont(ArialMT_Plain_24);
    if(p<0)
      disp.drawString(126,1, "Tbad");
    else
      disp.drawStringf(126,1, str,"T %3d", p);
}

void printP(float p){//pressure
    disp.setTextAlignment(TEXT_ALIGN_LEFT);
    disp.setFont(ArialMT_Plain_24);
    if(p<0)
      disp.drawString(3, 35, F("P:bad"));
    else if(digitalRead(PRESS_ON))
      disp.drawStringf(3, 35, str,"P %1.1f", p);
      else
      disp.drawStringf(3, 35, str,"P+%1.1f", p);
}

void curtime(int p){//uptime
    disp.setTextAlignment(TEXT_ALIGN_LEFT);
    disp.setFont(ArialMT_Plain_16);
    disp.drawStringf( 3, 5, str, "t%3d", p/60);
}

void printF(int p){//flow
    disp.setTextAlignment(TEXT_ALIGN_RIGHT);
    disp.setFont(ArialMT_Plain_24);
    disp.drawStringf(126, 41, str, "%3d", (int)p);
}

void hb(){//heatbead
    static char c='/';
    disp.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    disp.setFont(ArialMT_Plain_16);
      switch(c){
          case '/'://47://'/':
              c='-';//45;//('-');
              break;
          case '-'://45://'-':
              c='\\';//92;//('\');
              break;
          case '\\'://92://'\':
              c='|';//124;//('|');
              break;
          default:
              c='/';//47;//('/');
              break;
      }
    disp.drawString(/*128,32*/ disp.getWidth() / 2, disp.getHeight() / 2 , String(c));
}

void graph(float val, float max){//74,37,40,25
    static int8_t n=0;//
    if(val<=0)val=1;
    if(val>max)val=max;
    int8_t v=(int8_t)((val/max)*gMax);//graph val
    n++;
    if(n>=gLen){//shift left
        memcpy(buf,buf+1,sizeof(gLen)-1);
        n=gLen;
    }
    buf[n]=v;
    disp.drawRect (74, 37, 114, 63);
    for (int8_t i=0;i<gLen;i++){
        //disp.drawVerticalLine(74+i, 37, gMax, SH110X_BLACK);
        disp.drawVerticalLine(gX+i, gY+(gMax-buf[i]), gMax-buf[i]);//x y h
    }
    //disp.drawString(81, 41);
    //drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) 
}

void set_disp(){
    disp.clear();
    disp.drawRect (0, 0, 127, 63);
    hb();
}

void disp_flow(){
    set_disp();
    printT((int)Te);
    printP(Pb);
    printF((int)Lw);
    curtime(halfsec);
    disp_state();
    disp.display();
}

void disp_heat(){
    set_disp();
    printT((int)Te);
    printP(Pb);
    curtime(uptime);
    disp_state();
//    graph(Pb, 1.5);
//disp.drawStringf(81, 41, str, "%d", loopcnt);
    disp.display();
}

void disp_rst(){
    disp.clear();
    disp.setTextAlignment(TEXT_ALIGN_CENTER);
    disp.setFont(ArialMT_Plain_16);
    disp.drawStringf(disp.getWidth() / 2, 20, str, "ERROR: %d", system_get_rst_info()->reason);
    disp.drawString (disp.getWidth() / 2, 40, F("STOP") );
    disp.invertDisplay();
    disp.display();
}