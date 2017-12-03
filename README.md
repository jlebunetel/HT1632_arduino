# HT1632_arduino

This is a C++ library to use a HT1632 LED matrix with an Arduino or a NodeMCU (ESP8266).

## Installation

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

## Notes

Tested with Arduino IDE 1.8.1

Supports the following microcontrollers :

 * Arduino/Genuino Uno
 * Arduino/Genuino Mega or Mega 2560
 * Arduino Mega ADK
 * NodeMCU 1.0 (ESP-12E Module)

## Connexions

### Arduino Uno, Mega, Mega 2560, ADK

<table>
  <tr>
    <th>Arduino</th><th colspan=2>LED Matrix</th>
  </tr>
  <tr>
    <td>Pin</td><td>Pin</td><td>Function</td>
  </tr>
  <tr>
    <td>5V</td><td>1</td><td>+12V</td>
  </tr>
  <tr>
    <td>GND</td><td>4</td><td>GND</td>
  </tr>
  <tr>
    <td>4</td><td>8</td><td>CLK</td>
  </tr>
  <tr>
    <td>5</td><td>7</td><td>CS</td>
  </tr>
  <tr>
    <td>6</td><td>10</td><td>WR</td>
  </tr>
  <tr>
    <td>7</td><td>9</td><td>DATA</td>
  </tr>
</table>

### ESP8266

<table>
  <tr>
    <th colspan=2>NodeMCU</th><th colspan=2>LED Matrix</th>
  </tr>
  <tr>
    <td>Pin</td><td>GPIO</td><td>Pin</td><td>Function</td>
  </tr>
  <tr>
    <td>Vin (+5V)</td><td>-</td><td>1</td><td>+12V</td>
  </tr>
  <tr>
    <td>GND</td><td>-</td><td>4</td><td>GND</td>
  </tr>
  <tr>
    <td>D5</td><td>GPIO14</td><td>9</td><td>DATA</td>
  </tr>
  <tr>
    <td>D</td><td>GPIO12</td><td>7</td><td>CS</td>
  </tr>
  <tr>
    <td>D7</td><td>GPIO13</td><td>10</td><td>WR</td>
  </tr>
  <tr>
    <td>D8</td><td>GPIO15</td><td>8</td><td>CLK</td>
  </tr>
</table>
