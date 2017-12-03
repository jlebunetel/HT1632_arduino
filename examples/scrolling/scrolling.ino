#include <HT1632_arduino.h>

// initialisation de l'afficheur
HT1632_arduino afficheur;

Screen* ecran0 = new Screen();
Screen* ecran1 = new Screen();
Screen* ecran2 = new Screen();

void setup() {
  afficheur.begin(PWM_5);

  // on ajoute des images simples au buffers :
  // ecran0
  for (int x = 5; x < 12 ; x++) {
    for (int y = 2; y < 11; y++) {
      ecran0->setPixel(x, y, GREEN);
    }
  }
  for (int x = 20; x < 23 ; x++) {
    for (int y = 5; y < 9; y++) {
      ecran0->setPixel(x, y, RED);

    }
  }

  // ecran1
  for (int x = 10; x < 30 ; x++) {
    for (int y = 5; y < 7; y++) {
      ecran1->setPixel(x, y, RED);

    }
  }
  for (int x = 7; x < 10 ; x++) {
    for (int y = 11; y < 15; y++) {
      ecran1->setPixel(x, y, GREEN);

    }
  }

  // ecran2
  ecran2->setLine("09:45", FIRST, CENTER, ORANGE);
  ecran2->setString("C3", 1, 8, GREEN_INVERTED);
  ecran2->setString("<1'", 12, 8, RED);

}

void loop() {
  afficheur.display(ecran0);
  delay(3000);

  afficheur.scroll(ecran0, ecran1, 512, LEFT);

  afficheur.display(ecran1);
  delay(3000);

  afficheur.scroll(ecran1, ecran2, 512, LEFT);

  afficheur.display(ecran2);
  delay(3000);

  afficheur.scroll(ecran2, ecran0, 512, LEFT);
}
