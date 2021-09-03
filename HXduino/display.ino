
#include "default.h"

Adafruit_SSD1306 disp( 128, 64, &Wire, -1);

#define gLen 40
#define gMax 25
int8_t buf[gLen];//

void ver(){
    disp.setFont(&NotoSans12pt7b);
    disp.setTextSize(1);      // Normal 1:1 pixel scale
    disp.setTextColor(SSD1306_WHITE); // Draw white text
    disp.clearDisplay();
    disp.setCursor(25,20);
    disp.println(VER);
    disp.display();
    memset(buf, 0, sizeof(buf));
}

void printT(int p){//temp
    disp.setFont(&NotoSans22pt7b);
    disp.setCursor(60,1);
    disp.printf("%3d", (int)p);
}

void printP(float p){//pressure
    disp.setFont(&NotoSans22pt7b);
    disp.setCursor(3, 30);
    disp.printf("%1.1f", p);
}

void curtime(int p){//uptime
    disp.setFont(&NotoSans12pt7b);
    disp.setCursor( 4, 4);
    disp.printf("%d", p);
}

void printF(int p){//flow
    disp.setFont(&NotoSans12pt7b);
    disp.setCursor(81, 41);
    disp.printf("%3d", (int)p);
}

void hb(){//heatbead
    static char c=47;//('/');
    disp.setFont();
    disp.setCursor(25, 29);
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
    disp.print(c);
//?    disp.display();
}

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
        disp.writeFastVLine(74+i, 37, gMax, SSD1306_BLACK);
        disp.writeFastVLine(74+i, 37+(gMax-buf[i]), gMax-buf[i], SSD1306_WHITE);
    }
    //disp.setCursor(81, 41);
    //drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) 
}

void set_disp(){
    disp.setTextSize(1);      // Normal 1:1 pixel scale
    disp.setTextColor(SSD1306_WHITE); // Draw white text
    disp.clearDisplay();
    disp.drawRect (0, 0, 127, 63, 1);
    disp.display();
}

void disp_flow(){
    printT((int)Te);
    printP(Pb);
    printF((int)Lw);
    curtime(halfsec);
    disp.display();
}

void disp_heat(){
    printT((int)Te);
    printP(Pb);
    curtime(uptime);
    graph(Pb, 1.5);
    disp.display();
}
