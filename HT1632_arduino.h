#ifndef HT1632_arduino_h
#define HT1632_arduino_h

#define uint8 uint8_t
#define uint16 uint16_t

// NO-OP Definition
// The HT1632 requires a 1MHz clock.
// On a 80MHz processor, __asm__("nop\n\t"); provides 12.5ns per NOP.
// 40 times provides 500 ns -> half a period
// On a 16MHz processor, __asm__("nop\n\t"); provides 62.5ns per NOP.
// 8 times provides 500 ns -> half a period
#define CLK_DELAY; __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");



// D7 = Blue = DATA
#define HT1632_DATA_1     PORTH |= B00010000
#define HT1632_DATA_0     PORTH &= B11101111
#define HT1632_DATA(x)    ((x)?HT1632_DATA_1:HT1632_DATA_0)

// D5 = Green = CS
#define HT1632_CS_1       PORTE |= B00001000
#define HT1632_CS_0       PORTE &= B11110111

// D6 = Yellow = WR
#define HT1632_WR_1       PORTH |= B00001000
#define HT1632_WR_0       PORTH &= B11110111

// D4 = Orange = CLK
#define HT1632_CLK_1      PORTG |= B00100000
#define HT1632_CLK_0      PORTG &= B11011111



//Following definition facilitates compilation of HT1632C control commands.
#define SYS_DIS        0b100000000000 //Turn off both system oscillator and LED duty cycle generator
#define SYS_EN         0b100000000010 //Turn on system oscillator
#define LED_OFF        0b100000000100 //Turn off LED duty cycle generator
#define LED_ON         0b100000000110 //Turn on LED duty cycle generator
#define BLINK_OFF      0b100000010000 //Turn off blinking function
#define BLINK_ON       0b100000010010 //Turn on blinking function
#define SLAVE_MODE     0b100000100000 // Set slave mode and clock source from external clock
#define MASTER_MODE    0b100000101000 // Set master mode and clock source on-chip RC oscillator, the system clock output to OSC pin
#define RC_MASTER_MODE 0b100000110000 // System clock source, on-chip RC oscillator
#define EXT_CLK        0b100000111000 // System clock source, external clock source

#define N_MOS_COM8     0b100001000000 //N-MOS open drain output and 8 common option
#define N_MOS_COM16    0b100001001000 //N-MOS open drain output and 16 common option
#define P_MOS_COM8     0b100001010000 //P-MOS open drain output and 8 common option
#define P_MOS_COM16    0b100001011000 //P-MOS open drain output and 16 common option

#define PWM_1          0b100101000000 //PWM 1/16 duty
#define PWM_2          0b100101000010 //PWM 2/16 duty
#define PWM_3          0b100101000100 //PWM 3/16 duty
#define PWM_4          0b100101000110 //PWM 4/16 duty
#define PWM_5          0b100101001000 //PWM 5/16 duty
#define PWM_6          0b100101001010 //PWM 6/16 duty
#define PWM_7          0b100101001100 //PWM 7/16 duty
#define PWM_8          0b100101001110 //PWM 8/16 duty
#define PWM_9          0b100101010000 //PWM 9/16 duty
#define PWM_10         0b100101010010 //PWM 10/16 duty
#define PWM_11         0b100101010100 //PWM 11/16 duty
#define PWM_12         0b100101010110 //PWM 12/16 duty
#define PWM_13         0b100101011000 //PWM 13/16 duty
#define PWM_14         0b100101011010 //PWM 14/16 duty
#define PWM_15         0b100101011100 //PWM 15/16 duty
#define PWM_16         0b100101011110 //PWM 16/16 duty


#define BUFFER_LENGHT 128

#define BLACK 0
#define GREEN 1
#define RED 2
#define ORANGE 3
#define GREEN_INVERTED 4
#define RED_INVERTED 5
#define ORANGE_INVERTED 6

#define LEFT 0
#define RIGHT 1
#define CENTER 2

#define FIRST 0
#define SECOND 1

class Screen {
  public:
    Screen(void);

    void clear();
    void setPixel(uint8 x, uint8 y, uint8 color);
    void setChar(char c, uint8 x, uint8 y, uint8 color);
    void setString(String string, uint8 x, uint8 y, uint8 color);
    void setLine(String string, uint8 line, uint8 align, uint8 color);

  //private:
    uint8 buffer[BUFFER_LENGHT];
};

// library interface description
class HT1632_arduino {
  public:
    // constructors
    HT1632_arduino(void);

    void init();
    void begin(uint16 intensity);
    void selectAll();
    void selectNone();
    void commandWrite(uint16 command);
    void clear();
    void clearBuffer(uint8 *buffer);
    void dataWrite(uint8 * buffer);
    void dataWriteAddress(uint8 address, uint8 data);
    void greenVerticalLineOn(uint8 x);
    void greenVerticalLineOff(uint8 x);
    void redVerticalLineOn(uint8 x);
    void redVerticalLineOff(uint8 x);
    void display(Screen * screen);
    void display(Screen * screen1, Screen * screen2);
    void shiftLeft(uint8 * buffer0, uint8 * buffer1, int offset);
    void shiftUpperLeft(uint8 * buffer0, uint8 * buffer1, int offset);
    void shiftDownLeft(uint8 * buffer0, uint8 * buffer1, int offset);
    void scroll(Screen * screen0, Screen * screen1, int duration, int direction);
    void scrollUpper(Screen * screen0, Screen * screen1, int duration, int direction);
    void scrollDown(Screen * screen0, Screen * screen1, int duration, int direction);


  private:
//    uint8 buffer[BUFFER_LENGHT]; // utilisé pour le scroll
    Screen* temp; // utilisé pour le scroll
};

#endif
