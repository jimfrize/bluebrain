# Bluebrain

![bb](https://github.com/user-attachments/assets/3658e646-d5ac-4ee3-abab-b1aa27923d45)

ESP32 based receiver/controller for insect-class combat robot.

In this guide I will show you how I built a compact and low cost “brain” for an ant-weight combat robot. This "brain" lets me control my robot via Bluetooth using an XBox controler.

Specs:

* Weight: 4.63g (Including power leads)
* Size: 19mm x 23mm x 10mm
* Operating Voltage: 7.4V
* Motor Type: Brushed
* Motor Current: 1.5A X 2
* Cost: Around £7 GBP

For the Bluetooth host I used a [Seeed Xiao ESP32C3.](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)

I chose this board as it’s one of the smallest and cheapest ESP32 boards, however, it is not compatible with Classic Bluetooth, so you must use it with a [BLE](https://en.wikipedia.org/wiki/Bluetooth_Low_Energy) compatible controller.

For the motor driver, I chose a cheap and readily available board that uses the [DRV8833 chip.](https://www.ti.com/lit/ds/symlink/drv8833.pdf?ts=1744879168976&ref_url=https%253A%252F%252Fwww.google.com%252F)

In my case I purchased a 5 pack of DollaTek DRV8833 boards from Amazon for around £5 GBP. These boards have a pair of 1.5A H-Bridges that can handle up to 10V.

I intend to use a 7.4V battery in my robot design, so I can get the most out of my [servo](https://shop.bristolbotbuilders.com/product/servo/) and [motors.](https://shop.bristolbotbuilders.com/product/n10/) This means I need a regulator to drop the battery voltage to the required 5V for the ESP32 board. I chose a 1A surface mount regulator from Texas Instruments for this job: [TLV761](https://www.ti.com/lit/ds/symlink/tlv761.pdf?ts=1744875819716&ref_url=https%253A%252F%252Fwww.mouser.de%252F)

To connect to the game controller via Bluetooth I used the [Bluepad32 library for Arduino.](https://bluepad32.readthedocs.io/en/latest/)

It’s important to check for compatibility between the various ESP32 boards and game controllers, here’s [a useful page about this on the Bluepad32 website.](https://bluepad32.readthedocs.io/en/latest/supported_gamepads/)

Lastly, in order to control a servo, I used the [ESP32Servo Library for Arduino.](https://docs.arduino.cc/libraries/esp32servo/)

I only used one servo in my robot design, but it's worth noting that muliple servos can easily be connected to the ESP32.

Wiring Diagram:

![wiring_diagram](https://github.com/user-attachments/assets/29a46df0-f8c6-463e-b914-6bbae29d0d50)

The GPIO labels on the Seeed Xiao refer to the pin number in the Arduino IDE (e.g. GPIO10 is equivalent to pin 10 in the IDE).

Soldering Guide:

![regulator](https://github.com/user-attachments/assets/247bf1c8-e5b7-4605-bee2-b767e5aef9ed)

The surface mount regulator is glued upside-down onto the Seeed Xiao with epoxy. It's output and ground legs can then be bent down slightly and soldered to the Xiao.

![motor_controler](https://github.com/user-attachments/assets/f56de254-1fdb-48e3-8fae-282d3bd18396)

The DollaTek board has pin headers soldered to it's PWM control inputs. The jumper marked "J2" needs to be bridged in order for the motor controller to be enabled!
Plastic from a 4 pin header is epoxied to the board and used as a spacer.

![power_leads](https://github.com/user-attachments/assets/977258ee-c5d6-43e3-975d-5fe33fd15e89)

Silicon insulated multi-strand wire is soldered to the power input of the DollaTek board, this will be the connection to our 7.4V battery. Solid core wires are then soldered to the opposite side of the power input - these wires will be used to connect the battery power through to the voltage regulator.

![sandwich](https://github.com/user-attachments/assets/631930cb-3018-43ff-8106-0d52f199c941)

Here we can see the completed "brain", the headers are soldered to connect the Xiao and DollaTek boards together in a "sandwich". The red solid core wire has been soldered to the input of the voltage regulator, the black solid core wire has been connected to the outside of the USB-C connector - This is essentially a ground connection on the Xaio board.

The code provided is setup for my particular robot design, as such it uses the right analogue joystick to control motor movement and the right analogue paddle to control the servo possition with 100 degrees of movement.

Hope this is useful, let me know if you have success with this setup.






