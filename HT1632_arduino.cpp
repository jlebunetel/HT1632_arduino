#include "Arduino.h"
#include "HT1632_arduino.h"
#include <font_5x7.h>

HT1632_arduino::HT1632_arduino(void) {
  temp = new Screen();
  init();
}

void HT1632_arduino::init(void) {

  // select microcontroler
  #if defined(ARDUINO_AVR_UNO)

    DDRD |= B11110000; // sets Arduino pins 4 to 7 as outputs

  #elif defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_ADK)

    pinMode(4, OUTPUT); // DATA
    pinMode(5, OUTPUT); // CS
    pinMode(6, OUTPUT); // WR
    pinMode(7, OUTPUT); // CLK

  #elif defined(ESP8266)

    pinMode(14, OUTPUT); // DATA
    pinMode(12, OUTPUT); // CS
    pinMode(13, OUTPUT); // WR
    pinMode(15, OUTPUT); // CLK

  #endif // select microcontroler
}

void HT1632_arduino::setIntensity(uint8_t value) {
  // value between 0 and 15
  if (value > 15) {
    value = 15;
  }

  // on met à jour la variable globale
  intensity = (value << 1) | 0b100101000000;

  // puis on envoie la commande à l'afficheur
  noInterrupts();

  selectAll();
  commandWrite(intensity);
  selectNone();

  interrupts();
}

void HT1632_arduino::begin() {
  // initialisation de la matrice de led
  // apparement les broches OSC et SYNC des quatres HT1632 ne sont pas reliées dans cette matrice
  // on ne peut donc pas définir le premier HT1632 en MASTER et les suivants en SLAVE,
  // tous doivent être déclarés en mode master et utiliser le résonnateur interne (commande RC)

  noInterrupts();

  selectAll();                     // enable all HT1632s
  commandWrite(SYS_DIS);           // sends command
  selectNone();                    // disable all HT1632s

  selectAll();
  commandWrite(N_MOS_COM8);
  selectNone();

  selectAll();
  commandWrite(RC_MASTER_MODE);
  selectNone();

  selectAll();
  commandWrite(SYS_EN);
  selectNone();

  selectAll();
  commandWrite(intensity);
  selectNone();

  selectAll();
  commandWrite(BLINK_OFF);
  selectNone();

  selectAll();
  commandWrite(LED_ON);
  selectNone();

  interrupts();
}

void HT1632_arduino::begin(uint8_t value) {
  // on met à jour la variable globale
  intensity = (value << 1) | 0b100101000000;

  // puis on envoie intialise l'afficheur
  begin();
}

void HT1632_arduino::selectAll() {
  HT1632_CLK_0;  // clock line is 0
  HT1632_CS_0; // send "0" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°1
  CLK_DELAY;
  HT1632_CLK_0;

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°2
  CLK_DELAY;

  HT1632_CLK_0;
  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°3
  CLK_DELAY;

  HT1632_CLK_0;
  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°4
  CLK_DELAY;

  HT1632_CLK_0;
  //HT1632_CS_1; // send "1" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°5
  CLK_DELAY;
}

void HT1632_arduino::selectNone() {
  HT1632_CLK_0;  // clock line is 0
  HT1632_CS_1; // send "1" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°1
  CLK_DELAY;

  HT1632_CLK_0;
  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°2
  CLK_DELAY;

  HT1632_CLK_0;
  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°3
  CLK_DELAY;

  HT1632_CLK_0;
  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°4
  CLK_DELAY;
}

void HT1632_arduino::commandWrite(uint16 command) {
  uint16 j;
  command = command & 0x0fff;   // 12-bit command word, mask upper four bits

  // write command words in HT1632 register :
  for (uint8 i = 0; i < 12; i++) {
    j = command & 0x0800;        // return the MSB
    command = command << 1;      // move the control character to the left one
    j = j >> 11;                 // position the value at the LSB
    HT1632_WR_0;
    HT1632_DATA(j);             // send the value to the data port
    CLK_DELAY;

    HT1632_WR_1;  // clock pulse
    CLK_DELAY;
  }
}

void HT1632_arduino::clear() {
  noInterrupts();

  selectAll();
  // pour un HT1632, soit deux matrice 8x8 bicolores :
  // on selectionne la première adresse 0x00
  // en envoyant la séquence 1010000000

  HT1632_WR_0;  // clock line is 0
  HT1632_DATA_1; // send "1" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  HT1632_WR_0;
  HT1632_DATA_0; // send "0" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  HT1632_WR_0;
  HT1632_DATA_1; // send "1" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  for (int i = 0; i < 7; i++) {
    HT1632_WR_0;
    HT1632_DATA_0; // send "0" to data port
    CLK_DELAY;

    HT1632_WR_1;  // clock pulse
    CLK_DELAY;
  }

  // en envoie ensuite les 32 octets correspondants aux 16 colonnes de 8 leds vertes et aux 16 colonnes de 8 leds rouges

  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 8 ; j++) {
      HT1632_WR_0;
      HT1632_DATA_0; // send "0" to data port
      CLK_DELAY;

      HT1632_WR_1;  // clock pulse
      CLK_DELAY;
    }
  }
  selectNone();

  interrupts();
}

void HT1632_arduino::clearBuffer(uint8 *buffer) {
  for (int i = 0 ; i < BUFFER_LENGHT ; i++) {
    buffer[i] = 0;
  }
}

void HT1632_arduino::dataWrite(uint8 * buffer) {
  // pour un HT1632, soit deux matrice 8x8 bicolores :
  // on selectionne la première adresse 0x00
  // en envoyant la séquence 1010000000
  // puis on envoie les 32 octects

  HT1632_WR_0;  // clock line is 0
  HT1632_DATA_1; // send "1" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  HT1632_WR_0;
  HT1632_DATA_0; // send "0" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  HT1632_WR_0;
  HT1632_DATA_1; // send "1" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  for (int i = 0; i < 7; i++) {
    HT1632_WR_0;
    HT1632_DATA_0; // send "0" to data port
    CLK_DELAY;

    HT1632_WR_1;  // clock pulse
    CLK_DELAY;
  }

  uint8 data = 0b00000000;
  int k;
  for (int i = 0; i < 32; i++) {
    data = buffer[i];
    for (int j = 0; j < 8; j++) {
      k = data & 0x80;          // return the MSB
      data = data << 1;      // move the control character to the left one
      k = k >> 7;                  // position the value at the LSB
      HT1632_WR_0;
      HT1632_DATA(k);             // send the value to the data port
      CLK_DELAY;

      HT1632_WR_1;  // clock pulse
      CLK_DELAY;
    }
  }
}

void HT1632_arduino::dataWriteAddress(uint8 address, uint8 data) {
  // pour un HT1632, soit deux matrice 8x8 bicolores :
  // on selectionne la première adresse 0x00
  // en envoyant la séquence 1010000000
  // puis on envoie les 32 octects

  HT1632_WR_0;  // clock line is 0
  HT1632_DATA_1; // send "1" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  HT1632_WR_0;
  HT1632_DATA_0; // send "0" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  HT1632_WR_0;
  HT1632_DATA_1; // send "1" to data port
  CLK_DELAY;

  HT1632_WR_1;  // clock pulse
  CLK_DELAY;

  // select the address :
  uint8 j;

  for (uint8 i = 0; i < 7; i++) {
    j = address & 0x40;        // return the MSB
    address = address << 1;      // move the control character to the left one
    j = j >> 6;                 // position the value at the LSB
    HT1632_WR_0;
    HT1632_DATA(j);             // send the value to the data port
    CLK_DELAY;

    HT1632_WR_1;  // clock pulse
    CLK_DELAY;
  }

  for (uint8 i = 0; i < 8; i++) {
    j = data & 0x80;       // return the MSB
    data = data << 1;      // move the control character to the left one
    j = j >> 7;            // position the value at the LSB
    HT1632_WR_0;
    HT1632_DATA(j);        // send the value to the data port
    CLK_DELAY;

    HT1632_WR_1;  // clock pulse
    CLK_DELAY;
  }
}

void HT1632_arduino::greenVerticalLineOn(uint8 x) {
  noInterrupts();

  // active la ligne verticale x

  // on active un a un les HT1632

  HT1632_CLK_0;  // clock line is 0
  HT1632_CS_0; // send "0" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°1
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 actif
  if (x >= 0 && x < 16) {
    dataWriteAddress(x*2, B11111111);
  }

  HT1632_CS_1; // send "1" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°2
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 inactif
  // HT1632 #2 actif
  if (x >= 16 && x < 32) {
    dataWriteAddress((x-16)*2, B11111111);
  }


  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°3
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #2 inactif
  // HT1632 #3 actif
  if (x >= 0 && x < 16) {
    dataWriteAddress(x*2, B11111111);
  }

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°4
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #3 inactif
  // HT1632 #4 actif
  if (x >= 16 && x < 32) {
    dataWriteAddress((x-16)*2, B11111111);
  }

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°5
  CLK_DELAY;
  HT1632_CLK_0;

  interrupts();
}

void HT1632_arduino::greenVerticalLineOff(uint8 x) {
  noInterrupts();

  // active la ligne verticale x

  // on active un a un les HT1632

  HT1632_CLK_0;  // clock line is 0
  HT1632_CS_0; // send "0" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°1
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 actif
  if (x >= 0 && x < 16) {
    dataWriteAddress(x*2, B00000000);
  }

  HT1632_CS_1; // send "1" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°2
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 inactif
  // HT1632 #2 actif
  if (x >= 16 && x < 32) {
    dataWriteAddress((x-16)*2, B00000000);
  }


  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°3
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #2 inactif
  // HT1632 #3 actif
  if (x >= 0 && x < 16) {
    dataWriteAddress(x*2, B00000000);
  }

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°4
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #3 inactif
  // HT1632 #4 actif
  if (x >= 16 && x < 32) {
    dataWriteAddress((x-16)*2, B00000000);
  }

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°5
  CLK_DELAY;
  HT1632_CLK_0;

  interrupts();
}

void HT1632_arduino::redVerticalLineOn(uint8 x) {
  noInterrupts();

  // active la ligne verticale x

  // on active un a un les HT1632

  HT1632_CLK_0;  // clock line is 0
  HT1632_CS_0; // send "0" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°1
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 actif
  if (x >= 0 && x < 16) {
    dataWriteAddress(0x20 + x*2, B11111111);
  }

  HT1632_CS_1; // send "1" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°2
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 inactif
  // HT1632 #2 actif
  if (x >= 16 && x < 32) {
    dataWriteAddress(0x20 + (x-16)*2, B11111111);
  }


  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°3
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #2 inactif
  // HT1632 #3 actif
  if (x >= 0 && x < 16) {
    dataWriteAddress(0x20 + x*2, B11111111);
  }

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°4
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #3 inactif
  // HT1632 #4 actif
  if (x >= 16 && x < 32) {
    dataWriteAddress(0x20 + (x-16)*2, B11111111);
  }

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°5
  CLK_DELAY;
  HT1632_CLK_0;

  interrupts();
}

void HT1632_arduino::redVerticalLineOff(uint8 x) {
  noInterrupts();

  // active la ligne verticale x

  // on active un a un les HT1632

  HT1632_CLK_0;  // clock line is 0
  HT1632_CS_0; // send "0" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°1
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 actif
  if (x >= 0 && x < 16) {
    dataWriteAddress(0x20 + x*2, B00000000);
  }

  HT1632_CS_1; // send "1" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°2
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 inactif
  // HT1632 #2 actif
  if (x >= 16 && x < 32) {
    dataWriteAddress(0x20 + (x-16)*2, B00000000);
  }


  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°3
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #2 inactif
  // HT1632 #3 actif
  if (x >= 0 && x < 16) {
    dataWriteAddress(0x20 + x*2, B00000000);
  }

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°4
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #3 inactif
  // HT1632 #4 actif
  if (x >= 16 && x < 32) {
    dataWriteAddress(0x20 + (x-16)*2, B00000000);
  }

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°5
  CLK_DELAY;
  HT1632_CLK_0;

  interrupts();
}

void HT1632_arduino::display(Screen * screen) {
  // de temps en temps, un des quatre groupes de LEDs 16x8 s'éteint
  // pour palier ce bug (en attendant de le résoudre !) on réinitialise
  // l'afficheur avant chaque nouvel affichage
  //begin();
  noInterrupts();

  // on active un a un les HT1632

  HT1632_CLK_0;  // clock line is 0
  HT1632_CS_0; // send "0" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°1
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 actif
  dataWrite(screen->buffer);

  HT1632_CS_1; // send "1" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°2
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 inactif
  // HT1632 #2 actif
  dataWrite(screen->buffer + 32);

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°3
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #2 inactif
  // HT1632 #3 actif
  dataWrite(screen->buffer + 64);

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°4
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #3 inactif
  // HT1632 #4 actif
  dataWrite(screen->buffer + 96);

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°5
  CLK_DELAY;
  HT1632_CLK_0;

  interrupts();
}

void HT1632_arduino::display(Screen * screen1, Screen * screen2) {
  noInterrupts();

  // on active un a un les HT1632

  HT1632_CLK_0;  // clock line is 0
  HT1632_CS_0; // send "0" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°1
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 actif
  dataWrite(screen1->buffer);

  HT1632_CS_1; // send "1" to cs port
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°2
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #1 inactif
  // HT1632 #2 actif
  dataWrite(screen1->buffer + 32);

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°3
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #2 inactif
  // HT1632 #3 actif
  dataWrite(screen2->buffer + 64);

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°4
  CLK_DELAY;
  HT1632_CLK_0;

  // HT1632 #3 inactif
  // HT1632 #4 actif
  dataWrite(screen2->buffer + 96);

  //HT1632_CS unchanged
  CLK_DELAY;

  HT1632_CLK_1;  // clock pulse n°5
  CLK_DELAY;
  HT1632_CLK_0;

  interrupts();
}

void HT1632_arduino::shiftLeft(uint8 * buffer0, uint8 * buffer1, int offset) {
  int n = 0;
  if (offset >= 0 && offset < 16) {
    // ligne 1
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer0[i + 32];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 16];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer0[i + 48];
      n++;
    }

    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 32];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 48];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 16];
      n++;
    }

    // ligne 2
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer0[i + 32 + 64];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 16 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer0[i + 48 + 64];
      n++;
    }

    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 32 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 64];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 48 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 16 + 64];
      n++;
    }

  }
  else if (offset >= 16 && offset < 32) {
    offset = offset - 16;
    // ligne 1
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 32];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 48];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 16];
      n++;
    }

    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer1[i];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 32];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer1[i + 16];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 48];
      n++;
    }

    // ligne 2
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 32 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 64];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 48 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 16 + 64];
      n++;
    }

    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer1[i + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 32 + 64];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer1[i + 16 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 48 + 64];
      n++;
    }
  }
}

void HT1632_arduino::shiftUpperLeft(uint8 * buffer0, uint8 * buffer1, int offset) {
  int n = 0;
  if (offset >= 0 && offset < 16) {
    // ligne 1
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer0[i + 32];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 16];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer0[i + 48];
      n++;
    }

    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 32];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 48];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 16];
      n++;
    }
  }
  else if (offset >= 16 && offset < 32) {
    offset = offset - 16;
    // ligne 1
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 32];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 48];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 16];
      n++;
    }

    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer1[i];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 32];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer1[i + 16];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 48];
      n++;
    }
  }
  // ligne 2 inchangée
  for (int i = 64 ; i < 128 ; i++) {
    temp->buffer[i] = buffer0[i];
  }
}

void HT1632_arduino::shiftDownLeft(uint8 * buffer0, uint8 * buffer1, int offset) {
  // ligne 1 inchangée
  for (int i = 0 ; i < 64 ; i++) {
    temp->buffer[i] = buffer0[i];
  }
  int n = 64;
  if (offset >= 0 && offset < 16) {
    // ligne 2
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer0[i + 32 + 64];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 16 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer0[i + 48 + 64];
      n++;
    }

    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 32 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 64];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 48 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 16 + 64];
      n++;
    }

  }
  else if (offset >= 16 && offset < 32) {
    offset = offset - 16;
    // ligne 2
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 32 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 64];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer0[i + 48 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 16 + 64];
      n++;
    }

    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer1[i + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 32 + 64];
      n++;
    }
    for (int i = offset ; i < 16 ; i++) {
      temp->buffer[n] = buffer1[i + 16 + 64];
      n++;
    }
    for (int i = 0 ; i < offset ; i++) {
      temp->buffer[n] = buffer1[i + 48 + 64];
      n++;
    }
  }
}

void HT1632_arduino::scroll(Screen * screen0, Screen * screen1, int duration, int direction) {
  switch (direction) {
    case 0: // left
      for (int offset = 1 ; offset < 32 ; offset ++) {
        shiftLeft(screen0->buffer, screen1->buffer, offset);
        display(temp);
        delay(duration / 32);
      }
      break;
    case 1: // right
      for (int offset = 31 ; offset > 0 ; offset --) {
        shiftLeft(screen1->buffer, screen0->buffer, offset);
        display(temp);
        delay(duration / 32);
      }
    break;
  }
}

void HT1632_arduino::scrollUpper(Screen * screen0, Screen * screen1, int duration, int direction) {
  switch (direction) {
    case 0: // left
      for (int offset = 1 ; offset < 32 ; offset ++) {
        shiftUpperLeft(screen0->buffer, screen1->buffer, offset);
        display(temp);
        delay(duration / 32);
      }
      break;
    case 1: // right
      for (int offset = 31 ; offset > 0 ; offset --) {
        shiftUpperLeft(screen1->buffer, screen0->buffer, offset);
        display(temp);
        delay(duration / 32);
      }
    break;
  }
}

void HT1632_arduino::scrollDown(Screen * screen0, Screen * screen1, int duration, int direction) {
  switch (direction) {
    case 0: // left
      for (int offset = 1 ; offset < 32 ; offset ++) {
        shiftDownLeft(screen0->buffer, screen1->buffer, offset);
        display(temp);
        delay(duration / 32);
      }
      break;
    case 1:
      for (int offset = 31 ; offset > 0 ; offset --) {
        shiftDownLeft(screen1->buffer, screen0->buffer, offset);
        display(temp);
        delay(duration / 32);
      }
    break;
  }
}

Screen::Screen(void) {
  clear();
}

void Screen::clear() {
  for (int i = 0 ; i < BUFFER_LENGHT ; i++) {
    buffer[i] = 0;
  }
}

void Screen::setPixel(uint8 x, uint8 y, uint8 color) {
  int offset = -1;
  // dans quelle zone de l'écran se trouve le pixel ? quel HT1632 doit-on activer ?
  if (y >= 0 && y < 8) {
    if (x >= 0 && x < 16) {
      // HT1632 #1
      offset = 0;
    }
    else if (x >= 16 && x < 32) {
      // HT1632 #2
      offset = 1;
    }
  }
  else if (y >= 8 && y < 32) {
    if (x >= 0 && x < 16) {
      // HT1632 #3
      offset = 2;
    }
    else if (x >= 16 && x < 32) {
      // HT1632 #4
      offset = 3;
    }
  }
  // si x ou y ne sont pas dans les limites, on abandonne !
  if (offset == -1) {
    return;
  }
  x = x % 16;
  y = y % 8;
  int n = x + offset * 32;
  uint8 mask = (0x80 >> y);

  // green LEDs
  if (color == GREEN || color == ORANGE) {
    buffer[n] = buffer[n] | mask;
  }
  else {
    buffer[n] = buffer[n] & (~mask);
  }

  // red LEDs
  if (color == RED || color == ORANGE) {
    buffer[n + 16] = buffer[n + 16] | mask;
  }
  else {
    buffer[n + 16] = buffer[n + 16] & (~mask);
  }
}

void Screen::setChar(char c, uint8 x, uint8 y, uint8 color) {
  // restrictions :
  // 32 <= c <= 126
  // 0 <= x < 32
  // y=0 ou y=8 (pour le moment !)
  // 1 <= color <= 6

  // 32 <= c <= 126
  if (c < 32 || c > 126) {
    c = '?';
  }
  int char_offset = 32;
  int char_width = 5;

  // 0 <= x < 32
  if (x < 0 || x >= 32) {
    return;
  }

  // y=0 ou y=8 (pour le moment !)
  int y_offset = 0;
  if (y >= 8) {
    y_offset = 64;
  }

  // 1 <= color <= 6
  if (color < 1 || color > 6) {
    return;
  }

  for (int i = 0 ; i < char_width ; i++) {
    // est-on dans la plage de l'afficheur ?
    if (x + i < 0 || x + i >= 32) {
      return;
    }
    // quel driver ?
    int driver_offset = 0;
    if (x + i >= 16) {
      driver_offset = 16;
    }
    // indice du buffer ?
    int n = x + i  + driver_offset + y_offset;
    uint8 column = font_5x7[c - char_offset][i];
    // LEDs vertes ?
    if (color == GREEN || color == ORANGE) {
      buffer[n] = column;
    }
    if (color == GREEN_INVERTED || color == ORANGE_INVERTED) {
      buffer[n] = ~column;
    }
    // LEDs rouges ?
    if (color == RED || color == ORANGE) {
      buffer[n + 16] = column;
    }
    if (color == RED_INVERTED || color == ORANGE_INVERTED) {
      buffer[n + 16] = ~column;
    }
  }
}

void Screen::setString(String string, uint8 x, uint8 y, uint8 color) {
  if (x >= 1 && x < 32) {
    // y=0 ou y=8 (pour le moment !)
    int y_offset = 0;
    if (y >= 8) {
      y_offset = 64;
    }
    // quel driver ?
    int driver_offset = 0;
    if (x - 1 >= 16) {
      driver_offset = 16;
    }
    // indice du buffer ?
    int n = x - 1 + driver_offset + y_offset;
    if (color == GREEN_INVERTED || color == ORANGE_INVERTED) {
      buffer[n] = 0b11111111;
    }
    if (color == RED_INVERTED || color == ORANGE_INVERTED) {
      buffer[n + 16] = 0b11111111;
    }
  }
  int lenght = string.length();
  for (int i = 0 ; i < lenght ; i++) {
    setChar(string[i], x + (5 * i), y, color);
  }
}

void Screen::setLine(String string, uint8 line, uint8 align, uint8 color) {
  int lenght = string.length();
  // fonction limitée à 6 caractères pour le moment
  if (lenght > 6) {
    string = string.substring(0, 6);
    //return;
  }
  int y = 0;
  if (line == SECOND) {
    y = 8;
  }
  int offset = 0;
  switch (align) {
    case LEFT:
      offset = 1;
      break;
    case RIGHT:
      offset = 1 + 5 * (6 - lenght);
      break;
    case CENTER:
      offset = 1 + (1 + 5 * (6 - lenght))/2;
      break;
  }
  setString(string, offset, y, color);
}
