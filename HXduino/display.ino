
#include "default.h"

#include <Wire.h>
#include "SH1106.h"
SH1106 disp(0x3c, 4, 5);

#define gLen 40
#define gMax 25
int8_t buf[gLen];//
char str[32];

void ver(){
    disp.init();
    disp.flipScreenVertically();;
    disp.setTextAlignment(TEXT_ALIGN_CENTER);
    disp.setFont(ArialMT_Plain_24);
    disp.drawString(64,12,"v. " VER);
    disp.display();
    memset(buf, 0, sizeof(buf));
}

void printT(int p){//temp
    disp.setTextAlignment(TEXT_ALIGN_RIGHT);
    disp.setFont(ArialMT_Plain_24);
    disp.drawStringf(126,1, str,"T:%3d", p+random(0,100));
}

void printP(float p){//pressure
    disp.setTextAlignment(TEXT_ALIGN_LEFT);
    disp.setFont(ArialMT_Plain_24);
    disp.drawStringf(3, 35, str,"P:%1.1f", p);
}

void curtime(int p){//uptime
    disp.setTextAlignment(TEXT_ALIGN_LEFT);
    disp.setFont(ArialMT_Plain_16);
    disp.drawStringf( 3, 5, str, "%04d", p);
}

void printF(int p){//flow
    disp.setTextAlignment(TEXT_ALIGN_RIGHT);
    disp.setFont(ArialMT_Plain_24);
    disp.drawStringf(126, 41, str, "%3d", (int)p);
}

void hb(){//heatbead
    static char c=47;//('/');
    disp.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    disp.setFont(ArialMT_Plain_16);
    switch(c){
        case 47://'/':
            c=45;//('-');
            break;
        case 45://'-':
            c=92;//('\');
            break;
        case 92://'\':
            c=124;//('|');
            break;
        default:
            c=47;//('/');
            break;
    }
    disp.drawString(128,32, String(c));
//    disp.display();
}
/*
void graph(float val, float max){//74,37,40,25
    static int8_t n=0;//
    int8_t v=(int8_t)((max/val)*gMax);
    buf[n]=v;
    n++;
    if(n>=gLen){
        memcpy(buf,buf+1,sizeof(gLen)-1);
        n=gLen;
    }
    for (int8_t i=0;i<gLen;i++){
        disp.writeFastVLine(74+i, 37, gMax, SH110X_BLACK);
        disp.writeFastVLine(74+i, 37+(gMax-buf[i]), gMax-buf[i], SH110X_WHITE);
    }
    //disp.drawString(81, 41);
    //drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) 
}
*/
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
    disp.display();
}

void disp_heat(){
    set_disp();
    printT((int)Te);
    printP(Pb);
    curtime(uptime);
//    graph(Pb, 1.5);
    disp.display();
}
