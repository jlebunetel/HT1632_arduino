#include <HT1632_arduino.h>

// initialisation de l'afficheur
HT1632_arduino afficheur;

Screen* ecran0 = new Screen();

void setup() {
  afficheur.begin(PWM_10);
  ecran0->setLine("012345", FIRST, CENTER, RED);
  ecran0->setLine("678910", SECOND, CENTER, GREEN);
  afficheur.display(ecran0);
}

void loop() {
  delay(1);
}
