#include <HT1632_arduino.h>

// initialisation de l'afficheur
HT1632_arduino afficheur;

Screen* ecran0 = new Screen();

int i = 0;

void setup() {
  afficheur.begin(PWM_5);
  ecran0->setLine("Timer:", FIRST, CENTER, ORANGE);
}

void loop() {
  if (i > 999999) {
    afficheur.clear();
    i = 0 ;
  }
  ecran0->setLine(String(i), SECOND, RIGHT, GREEN);
  afficheur.display(ecran0);
  delay(100);
  i = i + 1;
}
