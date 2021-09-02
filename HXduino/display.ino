
#include "default.h"

Adafruit_SSD1306 disp( 128, 64, &Wire, -1);

void ver(){
    disp.setFont(&NotoSans12pt7b);
    disp.setTextSize(1);      // Normal 1:1 pixel scale
    disp.setTextColor(SSD1306_WHITE); // Draw white text
    disp.clearDisplay();
    disp.setCursor(25,20);
    disp.println(VER);
    display.display();
}

void printT(){//temp
}

void printP(){/pressure
}

void curtime(){//uptime
}

void printF(){//flow
}

void disp_flow(float Te, float Pb, float Lw, int tsec){
}

void disp_heat(float Te, float Pb, float Lw, int tsec){
}
