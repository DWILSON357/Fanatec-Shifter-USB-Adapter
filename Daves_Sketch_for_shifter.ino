//#include <stdint.h>

#include "Joystick.h"

Joystick_ g_joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  13, 0,         // Button Count, Hat Switch Count
  false, false, false,   // X, Y, Z
  false, false, false,   // Rx, Ry, Rz
  false, true,          // rudder, throttle
  false, false, false    // accelerator, brake, steering
);

int count = 0;
int g_gear = 0;
// calibration values
const int gate246 = 300; //when y axis is less than, gate is valid;
const int gater1357 = 700; // when y axis is greater than, gate is valid;
const int gater = 870; // Nominal R, X value;
const int gate12 = 700; //Nominal 12, X value;
const int gate34 = 510; //Nominal 34, X value;
const int gate56 = 334; //Nominal 56, X value;
const int gate7 = 164; //Nominal 7, X value;
const int gatewidth = 50; //X gate size tolerance, value+30 or Value-30

int g_current_gear = 0;
int next_gear;
int last_gear;
int gate_x;
int gate_y;
int gate_xp;//gate with tolernce added
int gate_xm;//gate with tolerance subtracted
int gate_xc = 0;//calculated gate postion

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  g_joystick.begin();
}

const int pinToButtonMap = A3;

void loop() {

  { int pot = analogRead(A3);
    int mapped = map(pot, 0, 1023, 0, 255);
    {
      g_joystick.setThrottle(mapped);
    }
  }

  int g_oddgear;
  int g_evengear;

  bool g_shiftplus = false;
  bool g_shiftminus = false;

  int mode = analogRead(A0);
  int x_raw = analogRead(A1);
  int y_raw = analogRead(A2);
  int hbrake = analogRead(A3);

  if ( mode > 200) {           //sequantial start
    Serial.print("S");
    g_joystick.pressButton(11);
    g_joystick.releaseButton(g_current_gear);
    g_joystick.releaseButton(g_gear);


    if (x_raw < 300) {
      g_joystick.pressButton(9);
      delay(5);
    }
    if (x_raw > 300) {
      g_joystick.releaseButton(9);
    }

    if (y_raw < 300) {
      g_joystick.pressButton(10);
      delay(5);
    }
    if (y_raw > 300) {
      g_joystick.releaseButton(10);
    }

    //goto gearexit;

  }                         //sequential end

  if (mode < 200) {            //start h
    //Serial.print("H");
    g_joystick.releaseButton(11);
    ;

    gate_x = x_raw;
    gate_y = y_raw;

    gate_xm = (gate_x - gatewidth);
    gate_xp = (gate_x + gatewidth);

    //Neutral decision
    if ((gate_y > gate246) && (gate_y < gater1357)) {
      g_gear = 0;
      g_joystick.setButton(g_gear, 0);
      goto geardecision;
    }
    // odd gear even gear decide
    if (gate_y < gate246) {
      g_evengear = 5;
      g_oddgear = 0;
    }

    if (gate_y > gater1357) {
      g_oddgear = 5;
      g_evengear = 0;
    }

    //GATE SELECT
    if ((gate_xp > gater) && (gate_xm < gater)) {
      gate_xc = 1;
    }

    if ((gate_xp > gate12) && (gate_xm < gate12)) {
      gate_xc = 2;
    }

    if ((gate_xp > gate34) && (gate_xm < gate34)) {
      gate_xc = 3;
    }

    if ((gate_xp > gate56) && (gate_xm < gate56)) {
      gate_xc = 4;
    }

    if ((gate_xp > gate7) && (gate_xm < gate7)) {
      gate_xc = 5;
    }
    // GEAR SELECT
    if ((gate_xc == 2) && (g_oddgear > 0))
    {
      g_gear = 1;
    }
    ;
    if ((gate_xc == 2) && (g_evengear > 0))
    {
      g_gear = 2;
    }

    if ((gate_xc == 3) && (g_oddgear > 0))
    {
      g_gear = 3;
    }
    ;
    if ((gate_xc == 3) && (g_evengear > 0))
    {
      g_gear = 4;
    }

    if ((gate_xc == 4) && (g_oddgear > 0))
    {
      g_gear = 5;
    }
    ;
    if ((gate_xc == 4) && (g_evengear > 0))
    {
      g_gear = 6;
    }

    if ((gate_xc == 5) && (g_oddgear > 0))
    {
      g_gear = 7;
    }
    ;
    if ((gate_xc == 1) && (g_oddgear > 0))
    {
      g_gear = 8;
    }

geardecision:;

    //Serial.println(g_current_gear);
    //Serial.println(g_gear);

    if (g_current_gear != g_gear) {
      delay(20);
      if (g_current_gear != g_gear) {
        g_joystick.setButton(g_current_gear, 0);
        if (g_gear != 0) {
          g_joystick.setButton(g_gear, 1);
          delay(20);
        }
        g_current_gear = g_gear;
      }
    }
  }    //end h

}
