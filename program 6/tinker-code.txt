/* 2026FA-ENGR-2304-00003-TRAVIS-ILEX-P05 */
/* https://www.tinkercad.com/things/iqbCt0br7lg/editel?returnTo=%2Fdashboard%2Fdesigns%2Fcircuits&sharecode=elKs3Bc3XN_HZ0q1dhvQb_J9z9usO9q0rcn44UUSHkA */


#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello World");
}

void loop() {
}
