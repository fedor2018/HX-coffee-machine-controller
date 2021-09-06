/*
Input: 0-60 psi(Gauge Pressure);
Output: 0.5-4.5V linear voltage output. 0 psi outputs 0.5V, 30 psi outputs 2.5V, 60 psi outputs 4.5V.
Input: 0-300 psi(Gauge Pressure);
Output: 0.5-4.5V linear voltage output. 0 psi outputs 0.5V, 150 psi outputs 2.5V, 300 psi outputs 4.5V.

Accuracy: +/-0.5%FS;
Thread: 1/8Ф NPT;
Wiring Connector: Packard plug-in unit is included;
Wiring: Red: +5V; Black: ground; Green: signal output;
Overload Capacity: 2-4 times;
Working Temperature: -40Ч+120?C;
Compensation Temperature: -20Ч+80?C;
*/
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
*/

arduino esp8266 v.2.7.4
max6675
ESP8266TimerInterrupt
ThingPulse/esp8266-oled-ssd1306
