// https://github.com/jlebunetel/HT1632_arduino/blob/master/examples/serial_communication/README.md

// led matrix driver
#include <HT1632_arduino.h> // https://github.com/jlebunetel/HT1632_arduino/

// display initialization
HT1632_arduino afficheur;
Screen* ecran0 = new Screen();
Screen* ecran_vide = new Screen();

// variables for serial reading
int inByte = 0;         // incoming serial byte
char receivedChar;
String receivedString;

// default values for the cursor
int align = LEFT;
int color = RED;
int line = FIRST;

// commands
#define ALIGN     1
#define BLINK     2
#define COLOR     3
#define ICON      4
#define FIXED     5
#define INTENSITY 6
#define LED       7
#define LINE      8
#define SCROLL    9
bool command_flag = false;
int command = 0;
bool value_flag = false;
int value = 0;

#define SCROLLING_STOP  0
#define SCROLLING_RIGHT 1
#define SCROLLING_LEFT  2
int scrolling = SCROLLING_STOP;
int scrolling_speed = 2048; // multiple de 32 / durée d'un cycle en millisecondes

#define FIXED_NONE   0
#define FIXED_LINE_1 1
#define FIXED_LINE_2 2
#define FIXED_BOTH   3
int fixed = FIXED_NONE;

#define BLINK_OFF   0
#define BLINK_ON    1
int blink = BLINK_OFF;

int offset = 0; // for scrolling

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize display
  afficheur.begin(0);
  ecran_vide->clear();
  ecran0->setLine("Wait", FIRST, LEFT, GREEN);
  ecran0->setLine("Serial", SECOND, LEFT, RED);
  afficheur.display(ecran0);
  delay(2000);
}

void loop() {
  // Cursor position
  int x = 0;
  int y = 0;
  if (line == SECOND) {
    y = 8;
  }
  if (align == CENTER) {
    x = 12;
  }
  else if (align == RIGHT) {
    x = 24;
  }

  // Listening to serial
  while (Serial.available() > 0) {
    receivedChar = Serial.read();
    if (receivedChar == '/') {
      // Command recieved !
      command_flag = true;
    }
    else if (receivedChar == '\n' || receivedChar == ' ') {
      // Message processing

      if (command_flag) {
        // Command prossessing

        if (receivedString == "align" || receivedString == "ALIGN") {
          command = ALIGN;
          value_flag = true; // Waiting for a following argument
        }

        if (receivedString == "blink" || receivedString == "BLINK") {
          command = BLINK;
          value_flag = true; // Waiting for a following argument
        }

        if (receivedString == "clear" || receivedString == "CLEAR") {
          ecran0->clear();
          afficheur.clear();
        }

        if (receivedString == "color" || receivedString == "COLOR") {
          command = COLOR;
          value_flag = true; // Waiting for a following argument
        }

        if (receivedString == "fixed" || receivedString == "FIXED") {
          command = FIXED;
          value_flag = true; // Waiting for a following argument
        }

        if (receivedString == "icon" || receivedString == "ICON") {
          command = ICON;
          value_flag = true; // Waiting for a following argument
        }

        if (receivedString == "intensity" || receivedString == "INTENSITY" || receivedString == "int" || receivedString == "INT") {
          command = INTENSITY;
          value_flag = true; // Waiting for a following argument
        }

        if (receivedString == "led" || receivedString == "LED") {
          command = LED;
          value_flag = true; // Waiting for a following argument
        }

        if (receivedString == "line" || receivedString == "LINE") {
          command = LINE;
          value_flag = true; // Waiting for a following argument
        }

        if (receivedString == "scroll" || receivedString == "SCROLL") {
          command = SCROLL;
          value_flag = true; // Waiting for a following argument
        }

        command_flag = false; // Command processed!
      }
      else if (value_flag) {
        // Argument processiong
        switch (command) {

          case ALIGN:
            if (receivedString == "left" || receivedString == "LEFT") {
              align = LEFT;
            }
            else if (receivedString == "center" || receivedString == "CENTER") {
              align = CENTER;
            }
            else if (receivedString == "right" || receivedString == "RIGHT") {
              align = RIGHT;
            }
            break;

          case BLINK:
            scrolling = SCROLLING_STOP; // Blinking disable scrolling
            if (receivedString == "on" || receivedString == "ON") {
              blink = BLINK_ON;
            }
            else if (receivedString == "off" || receivedString == "OFF") {
              blink = BLINK_OFF;
            }
            break;

          case COLOR:
            if (receivedString == "red" || receivedString == "RED") {
              color = RED;
            }
            else if (receivedString == "green" || receivedString == "GREEN") {
              color = GREEN;
            }
            else if (receivedString == "orange" || receivedString == "ORANGE") {
              color = ORANGE;
            }
            else if (receivedString == "-red" || receivedString == "-RED") {
              color = RED_INVERTED;
            }
            else if (receivedString == "-green" || receivedString == "-GREEN") {
              color = GREEN_INVERTED;
            }
            else if (receivedString == "-orange" || receivedString == "-ORANGE") {
              color = ORANGE_INVERTED;
            }
            break;

          case ICON:
            for (int i = 0; i < 8; i++) {
              if (receivedString == String(i)) {
                ecran0->setIcon(i, x, y, color);
              }
            }
            afficheur.display(ecran0);
            break;

          case FIXED:
            if (receivedString == "none" || receivedString == "NONE") {
              fixed = FIXED_NONE;
            }
            else if (receivedString == "1") {
              fixed = FIXED_LINE_1;
            }
            else if (receivedString == "2") {
              fixed = FIXED_LINE_2;
            }
            else if (receivedString == "both" || receivedString == "BOTH") {
              fixed = FIXED_BOTH;
            }
            break;

          case INTENSITY:
            afficheur.setIntensity(receivedString.toInt());
            break;

          case LINE:
            if (receivedString == "1") {
              line = FIRST;
            }
            else if (receivedString == "2") {
              line = SECOND;
            }
            break;

          case SCROLL:
            blink = BLINK_OFF; // Scrolling disable blinking
            if (receivedString == "stop" || receivedString == "STOP") {
              scrolling = SCROLLING_STOP;
            }
            else if (receivedString == "left" || receivedString == "LEFT") {
              scrolling = SCROLLING_LEFT;
            }
            else if (receivedString == "right" || receivedString == "RIGHT") {
              scrolling = SCROLLING_RIGHT;
            }
            break;
        }
        value_flag = false; // Argument processed!
      }
      else {
        // Message displaying
        ecran0->setLine(receivedString, line, align, color);
        afficheur.display(ecran0);
      }
      // Empty the message buffer
      receivedString = "";
    }
    else {
      // Addition of a character to the message buffer
      receivedString += receivedChar;
    }
    delay(1); // A little delay for the microcontroler stability

  }

  // Should we srcoll or blink?
  if (scrolling) {
    int direction = LEFT;
    if (scrolling == SCROLLING_RIGHT) {
      direction = RIGHT;
    }
    switch (fixed) {

      case FIXED_NONE:
        switch (direction) {
          case LEFT:
            afficheur.shiftLeft(ecran0->buffer, ecran0->buffer, offset);
            break;
          case RIGHT:
            afficheur.shiftLeft(ecran0->buffer, ecran0->buffer, 31 - offset);
            break;
        }
        afficheur.display();
        offset++;
        if (offset > 31) {
          offset = 0;
        }
        delay(scrolling_speed / 32);
        break;

      case FIXED_LINE_1:
        switch (direction) {
          case LEFT:
            afficheur.shiftDownLeft(ecran0->buffer, ecran0->buffer, offset);
            break;
          case RIGHT:
            afficheur.shiftDownLeft(ecran0->buffer, ecran0->buffer, 31 - offset);
            break;
        }
        afficheur.display();
        offset++;
        if (offset > 31) {
          offset = 0;
        }
        delay(scrolling_speed / 32);
        break;

      case FIXED_LINE_2:
        switch (direction) {
          case LEFT:
            afficheur.shiftUpperLeft(ecran0->buffer, ecran0->buffer, offset);
            break;
          case RIGHT:
            afficheur.shiftUpperLeft(ecran0->buffer, ecran0->buffer, 31 - offset);
            break;
        }
        afficheur.display();
        offset++;
        if (offset > 31) {
          offset = 0;
        }
        delay(scrolling_speed / 32);
        break;
    }
  }
  else if (blink) {
    switch (fixed) {

      case FIXED_NONE:
        // the entire screen blink or scroll
        afficheur.clear();
        delay(scrolling_speed / 8);
        afficheur.display(ecran0);
        delay(scrolling_speed / 8);
        break;

      case FIXED_LINE_1:
        // only the lower line blink or scroll
        afficheur.display(ecran0, ecran_vide);
        delay(scrolling_speed / 8);
        afficheur.display(ecran0);
        delay(scrolling_speed / 8);
        break;

      case FIXED_LINE_2:
        // only the upper line blink or scroll
        afficheur.display(ecran_vide, ecran0);
        delay(scrolling_speed / 8);
        afficheur.display(ecran0);
        delay(scrolling_speed / 8);
        break;
    }

  }

  delay(1); // A little delay for the microcontroler stability
}
