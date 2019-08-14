# Home Weather Station [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/sirakberhane/Home-Weather-Station/blob/master/LICENSE)
Using ESP8266 to get data like Temperature, Time, Humidity and display on a LCD screen.

## Hardware Components
* ESP8266 Module (Powered by micro USB, also outputs 3.3V of power to the entire circuit)
* LCD Display (16X2, ~3.3V)
* Breadboard 
* Hook-up Wire
* (1) 1K OHM Resistor
* (1) 470 OHM Resistor 

## Software Used
* Arduino IDE (https://www.arduino.cc/en/main/software)
* OpenWeatherMap API (https://openweathermap.org/api)

## Circuit 

### Breadboard View
![Breadboard View](https://github.com/sirakberhane/Home-Weather-Station/blob/master/Breadboard-Weather.jpg)

### Schematic View
![Schematic View](https://github.com/sirakberhane/Home-Weather-Station/blob/master/Schematic-Weather.jpg)

## Steps
1. Download Arduino IDE and setup your OpenWeatherMap account in order to get your API key.
2. Build the ciruit using the schematic provided or order a PCB part using the Fritzing File.
3. Download the source code.
4. Line 14, type in your Wi-Fi name.
5. Line 15, type in your Wi-Fi password.
6. {Optional} Line 20, you can change the timezone (default is set to -5 or Eastern Standard Time).
7. Line 110, change "API-ID-GOES-HERE" to your API ID you got from Step (1).
8. Plug in your ESP8266 and upload the code. If you have any issues uploading the code, 
    make sure that your code is verified and you have the right COM enabled. 
